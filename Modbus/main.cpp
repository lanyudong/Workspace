#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <string>
#include <map>

#include "include/Modbus.h"

using namespace std;

int main () {
    Modbus mb ("192.168.0.64", 502);
    uint16_t block[200];
    mb.ReadRegisters(40000,68,block);

    for (auto &reg : block) {
        cout << reg << "\n";
    }

    return 0;
}
