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

// SunSpec Model
// - This model uses boosts property tree to parse the xml models for
// - modbus register blocks.
// - https://www.technical-recipes.com/2014/using-boostproperty_tree/
class SunSpecModel {
public:
    SunSpecModel (unsigned int did, unsigned int offset)
        : offset_(offset) {
        // create filename using a base path, then pad the did number and append
        // to the base path. The sunspec models are provided as xml so that will be
        // the file type that is appended ot the end of the filename.
        std::stringstream ss;
        ss << "./models/smdx/smdx_";
        ss << std::setfill ('0') << std::setw(5) << did;
        ss << ".xml";
        std::string filename = ss.str();

        // Use boosts xml parser to read file and store as member variable.
        boost::property_tree::xml_parser::read_xml(filename, smdx_);
        name_ = smdx_.get <std::string> ("sunSpecModels.model.<xmlattr>.name", "");
        length_ = smdx_.get <unsigned int> ("sunSpecModels.model.<xmlattr>.len", 0);
        std::cout << "SunSpec Model Found"
            << "\n\tName: " << name_
            << "\n\tLength: " << length_ << std::endl;
    };

    ~SunSpecModel () {};

    std::string GetName () {
        return name_;
    };

private:
    unsigned int offset_;
    unsigned int length_;
    std::string name_;
    boost::property_tree::ptree smdx_;
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

    ~Modbus () {
        models_.clear ();
        modbus_close(context_ptr_);
        modbus_free(context_ptr_);
    };

    // Querry
    // - Read all available registers to find sunspec compliant blocks.
    // - The first step is to read the first register and see if the modbus
    // - device is a sunspec complient device.
    // - Then traverse register array and find sunspec DID values and create a
    // - sunspec model.
    void Querry () {
        unsigned int sunspec_key_ = 1850954613;
        uint16_t sunspec_id[2];
        // TODO (TS): sunspec states the holding register start can be
        // - 30, 40, or 50,000.
        // - 40000 is the preferece starting location
        unsigned int id_offset = 0;
        ReadRegisters(id_offset, 2, sunspec_id);
        std::cout << MODBUS_GET_INT32_FROM_INT16(sunspec_id,0) << std::endl;
        std::shared_ptr <SunSpecModel> model (new SunSpecModel (1, 2));
        models_.push_back (std::move (model));
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

    void ReadBlock (std::string name) {
    };

    void WriteBlock (std::string name) {
    };

private:
    // utility methods

private:
    // member variables
    modbus_t *context_ptr_;
    std::vector <std::shared_ptr <SunSpecModel>> models_;
};

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
