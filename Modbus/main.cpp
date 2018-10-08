#include <iostream>
#include <limits>

#include <modbus/modbus-tcp.h>

using namespace std;

int main()
{
    modbus_t *mb;
    uint16_t tab_reg[6];

    const char *addr = "192.168.0.10";
    mb = modbus_new_tcp(addr, 502);

    /* Read 5 registers from the address 0 */
    try {
        modbus_connect(mb);
    } catch(const std::exception& e) {
        cout << e.what () << endl;
    }

    /* Read 5 registers from the address 0 */
    try {
        modbus_read_registers(mb, 11, 6, tab_reg);
    } catch(const std::exception& e) {
        cout << e.what () << endl;
    }

    cout
        << "SOC: " << (float)tab_reg[0]/10 << '\n'
        << "VDC: " << (float)tab_reg[2]*1000/65536 << endl;


    modbus_close(mb);
    modbus_free(mb);
}
