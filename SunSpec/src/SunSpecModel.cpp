#include <iostream>

#include "include/SunSpecModel.h"

SunSpecModel::SunSpecModel (unsigned int did, unsigned int offset)
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
    did_ = smdx_.get <unsigned int> ("sunSpecModels.model.<xmlattr>.id", 0);
    std::string name;
    name = smdx_.get <std::string> ("sunSpecModels.model.<xmlattr>.name", "");
    length_ = smdx_.get <unsigned int> ("sunSpecModels.model.<xmlattr>.len", 0);

    std::cout << "SunSpec Model Found"
        << "\n\tDID: " << did_
        << "\n\tName: " << name
        << "\n\tLength: " << length_ << std::endl;

    SunSpecModel::GetScalers ();
}

SunSpecModel::~SunSpecModel() {
};


// Block To Points
// - convert raw modbus register block to it's corresponding SunSpec points
std::map <std::string, std::string> SunSpecModel::BlockToPoints (
    const std::vector <uint16_t>& register_block) {
    std::map <std::string, std::string> point_map;
    std::string id, type, scaler;
    unsigned int offset;

     // Traverse property tree
    BOOST_FOREACH (boost::property_tree::ptree::value_type const& node,
                   smdx_.get_child ("sunSpecModels.model.block")) {
        boost::property_tree::ptree subtree = node.second;
        if( node.first == "point" ) {
            id = subtree.get <std::string> ("<xmlattr>.id", "");
            type = subtree.get <std::string> ("<xmlattr>.type", "");
            scaler = subtree.get <std::string> ("<xmlattr>.sf", "default");
            offset = subtree.get <unsigned int> ("<xmlattr>.offset", 0);

            // TODO (TS): this should be configured by the smdx file
            if (type == "int16") {
                int16_t value = register_block[offset];
                value = value * register_block[scalers_[scaler]];
                point_map[id] = std::to_string(value);
            } else if (type == "uint16") {
                uint16_t value = register_block[offset];
                value = value * register_block[scalers_[scaler]];
                point_map[id] = std::to_string(value);
            } else if (type == "count") {
                uint16_t value = register_block[offset];
                value = value * register_block[scalers_[scaler]];
                point_map[id] = std::to_string(value);
            } else if (type == "acc16") {
                uint16_t value = register_block[offset];
                value = value * register_block[scalers_[scaler]];
                point_map[id] = std::to_string(value);
            } else if (type == "int32") {
                int32_t value = SunSpecModel::GetUINT32(register_block,offset);
                value = value * register_block[scalers_[scaler]];
                point_map[id] = std::to_string(value);
            } else if (type == "float32") {
                float value = SunSpecModel::GetUINT32(register_block,offset);
                value = value * register_block[scalers_[scaler]];
                point_map[id] = std::to_string(value);
            } else if (type == "acc32") {
                uint32_t value = SunSpecModel::GetUINT32(register_block,offset);
                value = value * register_block[scalers_[scaler]];
                point_map[id] = std::to_string(value);
            } else if (type == "enum16") {
                BOOST_FOREACH (boost::property_tree::ptree::value_type const& subsubtree,
                               subtree) {
                    boost::property_tree::ptree symbol = subsubtree.second;
                    std::string id;
                    unsigned int offset;
                    uint16_t property =
                    if( subsubtree.first == "symbol" && symbol.data() == "3") {
                        std::string symb;
                        symb = symbol.get <std::string> ("<xmlattr>.id", "");
                        std::cout << symbol.data() << " : " << symb << std::endl;
                    }
                }

                uint16_t value = register_block[offset];
                value = value * register_block[scalers_[scaler]];
                point_map[id] = std::to_string(value);
            } else if (type == "enum32") {
                int16_t value = register_block[offset];
                value = value * register_block[scalers_[scaler]];
                point_map[id] = std::to_string(value);
            } else if (type == "bitfield16") {
                int16_t value = register_block[offset];
                value = value * register_block[scalers_[scaler]];
                point_map[id] = std::to_string(value);
            } else if (type == "bitfield32") {
                int16_t value = register_block[offset];
                value = value * register_block[scalers_[scaler]];
                point_map[id] = std::to_string(value);
            } else if (type == "string") {
                int16_t value = register_block[offset];
                value = value * register_block[scalers_[scaler]];
                point_map[id] = std::to_string(value);
            } else if (type == "pad") {
                int16_t value = register_block[offset];
                value = value * register_block[scalers_[scaler]];
                point_map[id] = std::to_string(value);
            } else if (type == "ipaddr") {
                int16_t value = register_block[offset];
                value = value * register_block[scalers_[scaler]];
                point_map[id] = std::to_string(value);
            } else if (type == "ipv6addr") {
                int16_t value = register_block[offset];
                value = value * register_block[scalers_[scaler]];
                point_map[id] = std::to_string(value);
            } else if (type == "eui48") {
                int16_t value = register_block[offset];
                value = value * register_block[scalers_[scaler]];
                point_map[id] = std::to_string(value);
            }
        }
    }

    return point_map;
};


void SunSpecModel::GetScalers() {
    scalers_["default"] = 1;
     // Traverse property tree
    BOOST_FOREACH (boost::property_tree::ptree::value_type const& node,
                   smdx_.get_child ("sunSpecModels.model.block")) {
        boost::property_tree::ptree subtree = node.second;
        std::string id;
        unsigned int offset;
        if( node.first == "point" ) {

            if (subtree.get <std::string> ("<xmlattr>.type") == "sunssf") {
                id = subtree.get <std::string> ("<xmlattr>.id");
                offset = subtree.get <unsigned int> ("<xmlattr>.offset");
                scalers_[id] = offset;
            }
        }
    }
};


void SunSpecModel::DescalePoints (std::map <std::string, std::string>* points) {

};


uint32_t SunSpecModel::GetUINT32 (const std::vector <uint16_t>& block,
                                  const unsigned int index) {
    // bitshift the first register 16 bits and then append second register
    // this process is dependant on modbus
    return (block[index] << 16) | block[index];
};


uint64_t SunSpecModel::GetUINT64 (const std::vector <uint16_t>& block,
                                  const unsigned int index) {
    uint64_t value = static_cast<uint64_t>(block[index]) << 48
                     | static_cast<uint64_t>(block[index+1]) << 32
                     | static_cast<uint64_t>(block[index+2]) << 16
                     | block[index+3];
    return value;
};


std::string SunSpecModel::GetString (const std::vector <uint16_t>& block,
                                     const unsigned int index,
                                     const unsigned int length) {
    std::stringstream ss;
    for (unsigned int i = index; i < length; i++) {
        ss << static_cast <char> (block[i] >> 8);
        ss << static_cast <char> (block[i]);
    }
    return ss.str();
};


void SunSpecModel::SetUINT32 (std::vector <uint16_t>* block,
                              const unsigned int index,
                              const uint32_t value) {
    std::vector <uint16_t>& deref_block = *block;
    deref_block[index] = static_cast <uint16_t> (value >> 16);
    deref_block[index+1] = static_cast <uint16_t> (value);
};


void SunSpecModel::SetUINT64 (std::vector <uint16_t>* block,
                              const unsigned int index,
                              const uint64_t value) {
    std::vector <uint16_t>& deref_block = *block;
    deref_block[index] = static_cast <uint16_t> (value >> 48);
    deref_block[index+1] = static_cast <uint16_t> (value >> 32);
    deref_block[index+2] = static_cast <uint16_t> (value >> 16);
    deref_block[index+3] = static_cast <uint16_t> (value);
};

