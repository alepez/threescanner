/*
 * Config.h
 *
 * Copyright (c) 2014, Alessandro Pezzato
 */

#ifndef threescanner_CONFIG_H_
#define threescanner_CONFIG_H_

#include <string>

#include <boost/lexical_cast.hpp>
#include <boost/property_tree/ptree.hpp>

#include <map>

namespace threescanner {

class Config {
public:
	Config(const std::string& filename);
	virtual ~Config();
	template<typename T> T get(const std::string& path, T defaultValue) const {
		return pt_.get<T>(path, defaultValue);
	}
	template<typename T> T get(const std::string& path) const {
		return pt_.get<T>(path);
	}
	template<typename T> std::map<std::string, T> getAll() const {
		std::map<std::string, T> ret;
		for (boost::property_tree::ptree::const_iterator i = pt_.begin(); i != pt_.end(); ++i) {
			ret[i->first] = boost::lexical_cast<T>(i->second.get_value<T>());
		}
		return ret;
	}
	Config getChild(const std::string& path) const;
private:
	Config(const boost::property_tree::ptree& pt);
	boost::property_tree::ptree pt_;
};

} /* namespace threescanner */

#endif /* threescanner_CONFIG_H_ */
