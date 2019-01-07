#include "tsu.h"

#include <iostream>
#include <vector>
#include <string>

class Operator {
    tsu::string_matrix schedule_;

    public:
        Operator (std::string filename) {
            char delim = ',';
            unsigned int cols = 3;
            schedule_ = tsu::FileToMatrix(filename, delim, cols);
        };

        void Print () {
            std::cout << schedule_[0][0] << std::endl;
        };
};

using namespace std;

int main()
{
    tsu::string_matrix str_mat = tsu::FileToMatrix("data.csv", ',', 3);

    for (const auto &row : str_mat) {
        for (const auto &col : row) {
            cout << col << '\t';
        }
        cout << endl;
    }

    Operator op("data.csv");
    op.Print();

    return 0;
}
