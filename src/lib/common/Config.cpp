/*
 * Config.cpp
 *
 * Copyright (c) 2014, Alessandro Pezzato
 */

#include "Config.h"

#include <boost/property_tree/json_parser.hpp>

namespace threescanner {

using namespace boost::property_tree;
using namespace boost::property_tree::json_parser;

Config::Config(const std::string& filename) :
				pt_() {
	read_json(filename.c_str(), pt_);
}

Config::Config(const boost::property_tree::ptree& pt) :
				pt_(pt) {
}

Config Config::getChild(const std::string& path) const {
	return Config(pt_.get_child(path));
}

Config::~Config() {

}

} /* namespace threescanner */
