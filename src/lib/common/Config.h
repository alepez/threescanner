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

/**
 * Handles configuration. Configuration is read from a json file, so
 * it's accessible as a property tree.
 *
 * \see http://www.boost.org/doc/libs/1_56_0/doc/html/property_tree.html
 */
class Config {
public:
	/**
	 * Constructor
	 * \param filename absolute or relative path to the json
	 */
	Config(const std::string& filename);
	/**
	 * Destructor
	 */
	virtual ~Config();
	/**
	 * Getter with default value
	 * \param path the path where the value is stored. We construct the path by separating
   *     the individual keys with dots.
   * \param defaultValue the default value to return if none is found at the specified path
   * \return the value if found, otherwise defaultValue
	 */
	template<typename T> T get(const std::string& path, T defaultValue) const {
		return pt_.get<T>(path, defaultValue);
	}
	/**
	 * Getter without default value
	 * \param path the path where the value is stored. We construct the path by separating
   *     the individual keys with dots.
   * \return the value if found
   *
	 */
	template<typename T> T get(const std::string& path) const {
		return pt_.get<T>(path);
	}
	/**
	 * Getter for all children
	 *
	 * \return a map of values (casted to T) of direct children
	 */
	template<typename T> std::map<std::string, T> getAll() const {
		std::map<std::string, T> ret;
		for (boost::property_tree::ptree::const_iterator i = pt_.begin(); i != pt_.end(); ++i) {
			ret[i->first] = boost::lexical_cast<T>(i->second.get_value<T>());
		}
		return ret;
	}
	/**
	 * Child getter
	 * \param path the path where the child is stored.
	 * \return a Config object
	 */
	Config getChild(const std::string& path) const;
	/**
	 * Setter
	 * \param path the path to the value to create (or change)
	 * \param value the value to create (or change)
	 */
	template<typename T> void set(const std::string& path, const T& value) {
		pt_.put(path.c_str(), value);
	}
private:
	Config(const boost::property_tree::ptree& pt);
	boost::property_tree::ptree pt_;
};

} /* namespace threescanner */

#endif /* threescanner_CONFIG_H_ */
