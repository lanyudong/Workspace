#include <iostream>

// The modbus block structure is used to reduce modbus method call arguments
struct Block {
    Block (unsigned int offset, unsigned int length)
        : offset(offset), length(length) {
        registers[length];
        for (unsigned int i = 0; i < length; i++){
            registers[i] = 0;
        }
    };

    unsigned int offset;
    unsigned int length;
    uint16_t registers[];
};

using namespace std;

int main()
{
    Block block(0,10);
    return 0;
}
