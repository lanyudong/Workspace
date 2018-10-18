#include <iostream>
// Modbus Includes
#include <modbus/modbus-tcp.h>

class Modbus {
public:
    Modbus (std::string ip, unsigned int port) {
        // create modbus context pointer and connect to device at
        // given ip address and port number.
        context_ptr_ = modbus_new_tcp(ip.c_str(), port);
        if (modbus_connect(context_ptr_) == -1) {
            std::cout << "[ERROR] : " << modbus_strerror(errno) << '\n';
        }
    };

    ~Modbus () {
        modbus_close(context_ptr_);
        modbus_free(context_ptr_);
    };

    // Read Registers
    // - the register array is passed to the function as a pointer so the
    // - modbus method call can operate on them.
    void ReadRegisters (unsigned int offset,
                        unsigned int length,
                        uint16_t *reg_ptr) {
        int status = modbus_read_registers (context_ptr_,
                                            offset,
                                            length,
                                            reg_ptr);
        if (status == -1) {
            std::cout << "[ERROR] : " << modbus_strerror(errno) << '\n';
        }
    };

    // Write Registers
    // - the registers to write are passed by reference to reduce memory
    void WriteRegisters (unsigned int offset,
                         unsigned int length,
                         const uint16_t *reg_ptr) {
        int status = modbus_write_registers (context_ptr_,
                                             offset,
                                             length,
                                             reg_ptr);
        if (status == -1) {
            std::cout << "[ERROR] : " << modbus_strerror(errno) << '\n';
        }
    };

private:
    // utility methods

private:
    // member variables
    modbus_t *context_ptr_;
};

using namespace std;

int main()
{
    Modbus mb("127.0.0.1",5020);

    uint16_t block[20];
    mb.ReadRegisters(0,20,block);

    for (auto &reg : block) {
        cout << reg << '\n';
    }
    return 0;
}
