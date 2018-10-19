#ifndef SUNSPECMODEL_H
#define SUNSPECMODEL_H

#include <string>
#include <map>

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
    SunSpecModel (unsigned int did, unsigned int offset);
    virtual ~SunSpecModel ();

    // this operator will be used when looking for specific models
    bool operator == (const std::string &name) {
        return name_ == name;
    };

private:
    unsigned int offset_;
    unsigned int length_;
    std::string name_;
    boost::property_tree::ptree smdx_;
    std::map <std::string, std::string> points_;
};

#endif // SUNSPECMODEL_H
