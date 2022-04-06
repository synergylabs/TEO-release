//
// Created by han on 10/28/20.
//
#include <libtot/libtot.hpp>

#include <iostream>

using std::cout;
using std::endl;
using std::hex;

#include <cstdio>
#include <cassert>
#include <string>

using std::string;

#include <ctime>
#include <chrono>

#include <decaf.hxx>
#include <decaf/ed448.hxx>
#include <decaf/eddsa.hxx>
#include <decaf/secure_buffer.hxx>
#include <decaf/shake.hxx>
#include <decaf/spongerng.hxx>
#include <iomanip>


using decaf::SpongeRng;
using decaf::Block;
using decaf::SecureBuffer;
using decaf::FixedBlock;
using decaf::SHA3;

namespace libtot {

    typedef typename LIBTOT_EC_Group::Scalar Scalar;
    typedef typename LIBTOT_EC_Group::Point Point;
    typedef typename LIBTOT_EC_Group::DhLadder DhLadder;
    typedef typename LIBTOT_EC_Group::Precomputed Precomputed;

    static uint64_t leint(const SecureBuffer &xx) {
        uint64_t out = 0;
        for (unsigned int i = 0; i < xx.size() && i < sizeof(out); i++) {
            out |= uint64_t(xx[i]) << (8 * i);
        }
        return out;
    }


    static void print_xyz(
            const Scalar &x,
            const Scalar &y,
            const Scalar &z,
            const string &announcement = ""
    ) {
        if (!announcement.empty()) {
            cout << announcement << endl;
        }
        print("x", x);
        print("y", y);
        print("z", z);
    }

    static bool arith_check(
            const Scalar &x,
            const Scalar &y,
            const Scalar &z,
            const Scalar &l,
            const Scalar &r,
            const char *name
    ) {
        if (l == r) return true;
        printf("FAILED:  %s\n", name);
        print_xyz(x, y, z);
        print("lhs", l);
        print("rhs", r);
        assert(false);
    }

    void test_scalar_arithmetic() {
        cout << "Testing arithmetic" << endl;

        SpongeRng rng(Block("test_arithmetic"), SpongeRng::DETERMINISTIC);

        Scalar x(0), y(0), z(0);
        arith_check(x, y, z, x, x, "equality on x");

        arith_check(x, y, z, INT_MAX, (decaf_word_t) INT_MAX, "cast from max");
        arith_check(x, y, z, INT_MIN, -Scalar(1 + (decaf_word_t) INT_MAX), "cast from min");

        // Question: why do we need to mod 2x the scalar size?
        size_t sob =
                (LIBTOT_EC_Group::Scalar::SER_BYTES * 3) % (2 * LIBTOT_EC_Group::Scalar::SER_BYTES);
        SecureBuffer xx = rng.read(sob), yy = rng.read(sob), zz = rng.read(sob);

        x = Scalar(xx);
        y = Scalar(yy);
        z = Scalar(zz);

        print_xyz(x, y, z, "right after RNG");
        hexprint("hex xx", xx);

        arith_check(x, y, z, x + y, y + x, "commute add");
        arith_check(x, y, z, x, x + 0, "ident add");
        arith_check(x, y, z, x, x - 0, "ident sub");
        arith_check(x, y, z, x + -x, 0, "inverse add");
        arith_check(x, y, z, x - x, 0, "inverse sub");
        arith_check(x, y, z, x - (x + 1), -1, "inverse add2");
        arith_check(x, y, z, x + (y + z), (x + y) + z, "assoc add");
        arith_check(x, y, z, x * (y + z), x * y + x * z, "distributive mul/add");
        arith_check(x, y, z, x * (y - z), x * y - x * z, "distributive mul/add");
        arith_check(x, y, z, x * (y * z), (x * y) * z, "assoc mul");
        arith_check(x, y, z, x * y, y * x, "commute mul");
        arith_check(x, y, z, x, x * 1, "ident mul");
        arith_check(x, y, z, 0, x * 0, "mul by 0");
        arith_check(x, y, z, -x, x * -1, "mul by -1");
        arith_check(x, y, z, x + x, x * 2, "mul by 2");
        arith_check(x, y, z, -(x * y), (-x) * y, "neg prop mul");
        arith_check(x, y, z, x * y, (-x) * (-y), "double neg prop mul");
        arith_check(x, y, z, -(x + y), (-x) + (-y), "neg prop add");
        arith_check(x, y, z, x - y, (x) + (-y), "add neg sub");
        arith_check(x, y, z, (-x) - y, -(x + y), "neg add");

        if (sob <= 4) {
            uint64_t xi = leint(xx), yi = leint(yy);
            arith_check(x, y, z, x, xi, "parse consistency");
            arith_check(x, y, z, x + y, xi + yi, "add consistency");
            arith_check(x, y, z, x * y, xi * yi, "mul consistency");
        }

        if (y != 0) arith_check(x, y, z, x * y / y, x, "invert");

    }

    static bool point_check(
            const Point &p,
            const Point &q,
            const Point &R,
            const Scalar &x,
            const Scalar &y,
            const Point &l,
            const Point &r,
            const char *name
    ) {
        bool good = l == r;
        if (!p.validate()) {
            good = false;
            printf("  p invalid\n");
        }
        if (!q.validate()) {
            good = false;
            printf("  q invalid\n");
        }
        if (!r.validate()) {
            good = false;
            printf("  r invalid\n");
        }
        if (!l.validate()) {
            good = false;
            printf("  l invalid\n");
        }
        if (good) return true;

        printf("FAILED  %s\n", name);
        print("x", x);
        print("y", y);
        print("p", p);
        print("q", q);
        print("r", R);
        print("lhs", r);
        print("rhs", l);
        assert(false);
    }


    static void print_pqxy(
            const Point &p,
            const Point &q,
            const Scalar &x,
            const Scalar &y,
            const string &announcement = ""
    ) {
        if (!announcement.empty()) {
            cout << announcement << endl;
        }
        print("p", p);
        print("q", q);
        print("x", x);
        print("y", y);
    }


    void test_ec_points() {
        cout << "Testing elliptic curve point operations" << endl;

        SpongeRng rng(Block("test_ec_points"), SpongeRng::DETERMINISTIC);

        Point id = Point::identity(), base = Point::base();
        print("id", id);
        print("base", base);

        point_check(id, id, id, 0, 0, Point::from_hash(""), id, "from_hash_0");

        unsigned char enc[Point::SER_BYTES] = {0};
        point_check(id, id, id, 0, 0, Point(FixedBlock<sizeof(enc)>(enc)), id, "decode [0]");

        Point p(rng);
        Point q(rng);
        Scalar x(rng);
        Scalar y(rng);

        print_pqxy(p, q, x, y, "After RNG");

        Point d1, d2;

        SecureBuffer buffer(2 * Point::HASH_BYTES);
        rng.read(buffer);
        Point r = Point::from_hash(buffer);

        // Explore round trip
        cout << endl;
        print("p value", p);
        hexprint("p serialize()", p.serialize());
        point_check(p, q, r, 0, 0, p, Point(p.serialize()), "round-trip");

        // Compute torsion element of p
        Point pp = p.debugging_torque().debugging_pscale(rng);

        point_check(p, q, r, 0, 0, p.times_two(), p + p, "dbl add");
        point_check(p, q, r, 0, 0, p.times_two(), p * Scalar(2), "add times two");

    }

    void test_hash() {
        cout << "Testing SHA implementation..." << endl;

        SecureBuffer hash_val = SHA3<512>::hash(Block("abc"));
        hexprint("SHA3-512", hash_val);
    }

    void test_ec() {
        test_scalar_arithmetic();
        test_ec_points();
        test_hash();
    }

    void test_ec_elligator() {
        SpongeRng rng(Block("Test ec elligator hints"), SpongeRng::DETERMINISTIC);

        uint32_t NTESTS = 10000;

        std::clock_t c_start = std::clock();
        auto t_start = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < NTESTS; i++) {
            SecureBuffer buffer(Point::HASH_BYTES);
            rng.read(buffer);
            Point r = Point::from_hash(buffer);

            SecureBuffer storage(Point::HASH_BYTES);
            int count = -1;
            do {
                count++;
                r.invert_elligator(storage, count);
            } while (storage != buffer);

            Scalar k = sieve_keygen();
            SecureBuffer hash_val = SHA3<512>::hash(Block("abc"));
            Point hp = Point::from_hash(hash_val);

            Point encode = (k * hp) + r;
            Point decode = (-k * hp) + encode;

            decode.invert_elligator(storage, count);
            assert(storage == buffer);

        }

        std::clock_t c_end = std::clock();
        auto t_end = std::chrono::high_resolution_clock::now();

        cout << std::fixed << std::setprecision(2)
             << "Time for " << NTESTS << " point conversion" << endl
             << "CPU time used: "
             << 1000.0 * (c_end - c_start) / CLOCKS_PER_SEC
             << "ms" << endl
             << "Wall clock time passed: "
             << std::chrono::duration<double, std::milli>(t_end - t_start).count()
             << " ms" << endl;
    }
} // namespace libtot