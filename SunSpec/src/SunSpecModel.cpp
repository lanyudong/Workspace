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
    name_ = smdx_.get <std::string> ("sunSpecModels.model.<xmlattr>.name", "");
    length_ = smdx_.get <unsigned int> ("sunSpecModels.model.<xmlattr>.len", 0);
    std::cout << "SunSpec Model Found"
        << "\n\tName: " << name_
        << "\n\tLength: " << length_ << std::endl;
}

SunSpecModel::~SunSpecModel() {
};
