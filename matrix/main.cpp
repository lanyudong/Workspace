#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main()
{
    vector <string> test(100, "duhh");
    vector <vector <string>> matrix (10, vector <string> (10, "old"));
    unsigned int cnt = 0;
    for (auto &row : matrix) {
        for (auto &col : row) {
            col = test[cnt];
            cnt++;
        }
    }

    for (const auto &row : matrix) {
        for (const auto &col : row) {
            cout << col << '\t';
        }
        cout << endl;
    }

    return 0;
}