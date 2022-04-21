//
// Created by Han Zhang on 6/8/21.
//

#include "test_decaf.hpp"



#include <decaf.hxx>
#include <decaf/spongerng.hxx>
#include <decaf/eddsa.hxx>
#include <decaf/shake.hxx>

#include <iostream>

using std::cout;
using std::endl;
using std::hex;

#include "native-lib.hpp"

using decaf::SpongeRng;
using decaf::Block;
using decaf::SecureBuffer;
using decaf::FixedBlock;
using decaf::SHA3;

using std::string;

typedef typename decaf::Ed448Goldilocks TEO_EC_Group;
typedef typename TEO_EC_Group::Scalar Scalar;
typedef typename TEO_EC_Group::Point Point;
typedef typename TEO_EC_Group::DhLadder DhLadder;
typedef typename TEO_EC_Group::Precomputed Precomputed;

static uint64_t leint(const SecureBuffer &xx) {
    uint64_t out = 0;
    for (unsigned int i = 0; i < xx.size() && i < sizeof(out); i++) {
        out |= uint64_t(xx[i]) << (8 * i);
    }
    return out;
}

static void print(const char *name, const Scalar &x) {
    unsigned char buffer[Scalar::SER_BYTES];
    x.serialize_into(buffer);
    printf("  %s = 0x", name);
    for (int i=sizeof(buffer)-1; i>=0; i--) {
        printf("%02x", buffer[i]);
    }
    printf("\n");
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
            (TEO_EC_Group::Scalar::SER_BYTES * 3) % (2 * TEO_EC_Group::Scalar::SER_BYTES);
    SecureBuffer xx = rng.read(sob), yy = rng.read(sob), zz = rng.read(sob);

    x = Scalar(xx);
    y = Scalar(yy);
    z = Scalar(zz);

    print_xyz(x, y, z, "right after RNG");

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
bool test_decaf() {
    test_scalar_arithmetic();
    return true;
}