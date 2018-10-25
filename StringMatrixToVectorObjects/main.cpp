#include <iostream>
#include <string>
#include <vector>

#include "tsu.h"

class Operator {
public:
    Operator (const std::string& filename)
        : index_(0), file_path_(filename) {
        tsu::string_matrix matrix = tsu::FileToMatrix("data.csv", ',', 3);

        // reserve memory for each row within the matrix
        schedule_.reserve(matrix.size());

        // loop through each row of the matrix and set each column to it's
        // header data type
        for (const auto &col : matrix) {
            schedule_.emplace_back (stoi(col[0]), col[1], stoul(col[2]));
        }
    };

    void Loop (const unsigned int time) {

        // loop through each row of schedule looking for current time
        for (unsigned int i = index_; i < schedule_.size(); i++) {
            ScheduleHeader& row = schedule_[i];

            if (row.time == time && index_ != i) {

                // if the time is found check control type
                if (row.control == "import") {
                    std::cout << "importing at... " << row.setting << std::endl;
                } else if (row.control == "export") {
                    std::cout << "exporting at... " << row.setting << std::endl;
                } else {
                    std::cout << "idleing" << std::endl;
                }

                // log index so multiple controls are not sent and to reduce
                // search time.
                index_ = i;
            }
        }
    };

private:
    // since the file columns are known we can create and object to represent
    // the values within the column.
    struct ScheduleHeader {
        int time;
        std::string control;
        unsigned int setting;

        ScheduleHeader (int time, std::string control, unsigned int setting)
            : time(time), control(control), setting(setting) {
        };
    };

private:
    unsigned int index_;
    std::string file_path_;
    std::vector <ScheduleHeader> schedule_;
};

using namespace std;

int main()
{
    Operator OP("data.csv");
    OP.Loop(10);
    return 0;
}
