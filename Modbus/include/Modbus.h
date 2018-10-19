#ifndef MODBUS_H
#define MODBUS_H

#include <string>
#include <vector>

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
    // member variables
    modbus_t *context_ptr_;
    std::vector <std::shared_ptr <SunSpecModel>> models_;
};

#endif // MODBUS_H
