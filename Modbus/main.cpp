#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <string>
#include <map>

// Modbus Includes
#include <modbus/modbus-tcp.h>

// BOOST Libs
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>

class SunSpecModel {
public:
    SunSpecModel (const unsigned int did, const unsigned int offset)
        : offset_(offset) {
        // create filename using a base path, then pad the did number and append
        // to the base path. The sunspec models are provided as xml so that will be
        // the file type that is appended ot the end of the filename.
        std::stringstream ss;
        ss << "./models/smdx/smdx_";
        ss << std::setfill ('0') << std::setw(5) << 102;
        ss << ".xml";
        std::string filename = ss.str();

        // Use boosts xml parser to read file and store as member variable.
        boost::property_tree::xml_parser::read_xml(filename, smdx_);
    };

    ~SunSpecModel () {};


private:
    boost::property_tree::ptree smdx_;
    unsigned int offset_;
    unsigned int length_;
    std::map <std::string, std::string> points_;
};

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

    ~Modbus () {};

    void ReadBlock (std::string name);
    void WriteBlock (std::string name);

private:
    // utility methods
    void BlockToModel () {};
    void ModelToBlock () {};
    void ScaleModel () {};
    modbus_t *context_ptr_;
    std::vector <std::shared_ptr <SunSpecModel>> models_;
};

using namespace std;

int main()
{
    modbus_t *mb;
    uint16_t tab_reg[10];
    unsigned int tab_max = sizeof(tab_reg);
    cout << tab_max << endl;

    const char *addr = "127.0.0.1";
    mb = modbus_new_tcp(addr, 5020);

    int dec[] = {18533,27756,28448,22383,29292,100};
    for (auto &each : dec) {
        cout << static_cast<char*>(static_cast<void*>(&each));
    }
    cout << endl;

    if (modbus_connect(mb) == -1) {
        cout << "[ERROR] : " << modbus_strerror(errno) << '\n';
    }

    /* Read 5 registers from the address 0 */
    if (modbus_read_registers(mb, 0, 5, tab_reg) == -1) {
        cout << "[ERROR] : " << modbus_strerror(errno) << '\n';
    }

    for (unsigned int i = 0; i < tab_max; i++)  {
        tab_reg[i] = i;
    }

    if (modbus_write_registers(mb, 5, tab_max, tab_reg) == -1) {
        cout << "[ERROR] : " << modbus_strerror(errno) << '\n';
    }

    for (auto &reg : tab_reg) {
        cout << reg << '\n';
    }

    modbus_close(mb);
    modbus_free(mb);
}
