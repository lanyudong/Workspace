#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <string>
#include <map>

#include "include/Modbus.h"

using namespace std;

int main () {
    Modbus mb ("127.0.0.1", 5020);
    mb.Querry();
    uint16_t block[10];
    mb.ReadRegisters(0,10,block);

    for (auto &reg : block) {
        cout << reg << '\n';
    }

    int dec[6] = {18533,27756,28448,22383,29292,100};
    for (auto &each : dec) {
        cout << static_cast<char*>(static_cast<void*>(&each));
    }
    return 0;
}
