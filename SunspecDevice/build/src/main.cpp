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

    map <string, string> point;
    point["GSconfig_ReFloat_Volts"] = "48";
    ssmb.WritePoint(64116, point);
    return 0;
}
