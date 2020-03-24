// INCLUDES
#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <vector>
#include <map>
#include "include/logger.h"
#include "include/SunSpecModbus.h"


// NAMESPACES
using namespace std;

// GLOBALS
bool done = false;              // signal program to stop

// Main
// ----
int main (int argc, char** argv) {
    cout
        << "\n**********************************"
        << "\n*** OUTBACK LOGGER ***"
        << "\n**********************************\n";

    // SETUP INVERTER
    map <string, string> outback_settings;
    outback_settings["key"] = "1850954613";
    outback_settings["did"] = "1";
    outback_settings["path"] = "../data/model/smdx/";
    outback_settings["ip"] = "192.168.0.100";
    outback_settings["port"] = "502";

    // PASS SETTINGS AND CONNECT TO INVERTER
    SunSpecModbus* inverter_ptr(outback_settings)

    // SETUP TIMING LOOP FOR READING AND LOGGING
    unsigned int time_remaining, time_past;
    unsigned int time_wait = 1000;  // milliseconds
    auto time_start = chrono::high_resolution_clock::now();
    auto time_end = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> time_elapsed;

    while (!done) {
        time_start = chrono::high_resolution_clock::now();
        // time since last control call;
        time_elapsed = time_start - time_end;
        time_past = time_elapsed.count();

        // READ REGISTER BLOCK
        block_map ss_inverter = inverter_.ReadBlock (102);

        // CONVERT BLOCK TO REGISTER POINTS
        float freq = stof (radian_split["GS_Split_AC_Input_Frequency"]);
        float volt = stof (radian_split["GS_Split_AC_Input_Voltage"]);

        // LOG DATA
        Logger ("DATA", GetLogPath ())
            << GetExportWatts () << "\t"
            << GetExportPower ();

        // DETERMINE PROCESS TIME
        time_end = chrono::high_resolution_clock::now();
        time_elapsed = time_end - time_start;

        // SLEEP FOR REMAINING WAIT TIME
        if (time_wait - time_elapsed.count() > 0) {
            time_remaining = time_wait - time_elapsed.count();
            this_thread::sleep_for (chrono::milliseconds (time_remaining));
        }
    }

    // return exit status
    return 0;
} // end main
