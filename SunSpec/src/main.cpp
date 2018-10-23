#include <iostream>
#include <vector>
#include <string>
#include <map>

#include "include/SunSpecModel.h"

using namespace std;

int main()
{
    SunSpecModel model(102, 2);
    bool match = model == 1;
    cout << match << endl;

    map <string, string> props;
    vector <uint16_t> register_block (1000,1);
    props = model.BlockToPoints(register_block);
    model.GetScalers();
    for (auto &cell : model.scalers_) {
        cout << cell.first << " : " << cell.second << std::endl;
    }
    return 0;
}
