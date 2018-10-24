#include <iostream>
#include <string>
#include <vector>

#include "tsu.h"

struct ScheduleHeader {
    int time;
    std::string control;
    unsigned int setting;
    ScheduleHeader (int time, std::string control, unsigned int setting)
        : time(time), control(control), setting(setting) {
    };
};

using namespace std;

int main()
{
    tsu::string_matrix matrix = tsu::FileToMatrix("data.csv", ',', 3);

    // reserve memory for each row within the matrix
    vector <ScheduleHeader> schedule;
    schedule.reserve(matrix.size());

    // loop through each row of the matrix and set each column to it's
    // header data type
    for (const auto &row : matrix) {
        schedule.emplace_back (stoi(row[0]), row[1], stoul(row[2]));
    }

    for (const auto &entry : schedule) {
        cout << "Time:\t" << entry.time << "\t" << entry.control << " at "
             << entry.setting << '\n';
    }

    string test = "10.0000";
    float val = stof(test);
    cout << val << endl;
    return 0;
}
