#ifndef MODBUS_H
#define MODBUS_H

#include <string>
#include <vector>

// Modbus Includes
#include <modbus/modbus-tcp.h>

class Modbus {
public:
    Modbus (std::string ip, unsigned int port);
    ~Modbus ();
    void Querry ();
    void ReadRegisters (unsigned int offset,
                        unsigned int length,
                        uint16_t *reg_ptr);
    void WriteRegisters (unsigned int offset,
                         unsigned int length,
                         const uint16_t *reg_ptr);

    void ReadBlock (std::string name);

    void WriteBlock (std::string name);

private:

};

#endif // MODBUS_H
