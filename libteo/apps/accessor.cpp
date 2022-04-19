#include <teo/teo.hpp>
#include <fmt/format.h>
#include <argparse/argparse.hpp>
#include <string>
#include <iostream>
#include <curl/curl.h>
#include <thread>

using std::cout;
using std::endl;
using std::string;

const int SLEEP_INTERVAL = 500; // in milliseconds

int main(int argc, char *argv[])
{
    argparse::ArgumentParser program("TOT Accessor");

    program.add_argument("storage_ip")
        .help("IP address of the storage provider.");

    program.add_argument("storage_port")
        .help("Port number of the storage provider.")
        .action([](const std::string &value)
                { return std::stoi(value); });

    program.add_argument("google_sheet_id")
        .help("For result collection");

    program.add_argument("apps_script_url")
        .help("For result collection.");

    program.add_argument("reps")
        .help("Test repetitions.")
        .action([](const std::string &value)
                { return std::stoi(value); });

    program.add_argument("data_block_UUID")
        .help("UUID for request.");

    try
    {
        program.parse_args(argc, argv);
    }
    catch (const std::runtime_error &err)
    {
        std::cout << err.what() << std::endl;
        std::cout << program;
        exit(0);
    }

    std::string sheet_id = program.get("google_sheet_id");
    std::string sheet_script = program.get("apps_script_url");
    int reps = program.get<int>("reps");
    std::string data_block_uuid = program.get("data_block_UUID");

    std::string storage_ip = program.get("storage_ip");
    int storage_port = program.get<int>("storage_port");

    fmt::print("Google sheet ID: {}\n", sheet_id);
    fmt::print("Google script at: {}\n", sheet_script);

    fmt::print("\nRunning Accessor node...\n\n");

    teo::api_initialize();

    teo::Accessor acc(storage_ip, storage_port);

    teo::UUID dataUUID(data_block_uuid);

    std::string res = "";
    res += "sieve_dec_timer,sym_dec_timer,download_timer,total_timer\n";

    for (int i = 0; i < reps; i++)
    {
        int sieve_dec_timer, sym_dec_timer, download_timer;
        std::chrono::high_resolution_clock::time_point timer_start, timer_stop;

        timer_start = std::chrono::high_resolution_clock::now();

        try {
        acc.request_access(dataUUID, "", false, false,
                           &sieve_dec_timer, &sym_dec_timer, &download_timer);
        
        timer_stop = std::chrono::high_resolution_clock::now();
        int total_timer = std::chrono::duration_cast<std::chrono::milliseconds>(timer_stop - timer_start).count();

        res += fmt::format("{},{},{},{}\n",
                           sieve_dec_timer,
                           sym_dec_timer,
                           download_timer,
                           total_timer);
        }
        catch (...) {
            fmt::print("encounter error...\n");
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_INTERVAL));
    }

    fmt::print("Result:\n\n{}", res);

    return 0;
}