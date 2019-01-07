#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <bitset>

#include "include/SunSpecModbus.h"

using namespace std;

int main()
{
    map <string,string> configs;
    configs["key"] = "1850954613";
    configs["did"]="1";
    configs["path"]="../data/models/smdx/";
    configs["ip"]="192.168.0.64";
    configs["port"]="502";
    SunSpecModbus ssmb(configs);
    map <string, string> props;
    vector <uint16_t> register_block (100,1);

    props = ssmb.ReadBlock(64110);
    for (auto &cell : props) {
        cout << cell.first << " : " << cell.second << std::endl;
    }
    return 0;
}
