#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <bitset>

#include "include/SunSpecModel.h"

using namespace std;

int main()
{
    SunSpecModel model(64201, 2);
    map <string, string> props;
    vector <uint16_t> register_block (100,1);

    props = model.BlockToPoints(register_block);
    for (auto &cell : props) {
        cout << cell.first << " : " << cell.second << std::endl;
    }

    vector <uint16_t> regs = model.PointsToBlock(props);
    for (auto &reg : regs) {
        cout << reg << std::endl;
    }
    return 0;
}
