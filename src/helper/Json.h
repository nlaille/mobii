#pragma once

/**
 * \file Json.h
 * \brief Declaration of of Json Helpers
 * \author Nicolas Laille
 * \version 1.0
 * \date 24 Octobre 2013
 */

#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace Helper
{
	/**
	 * \namespace Json
	 * \brief Json Helper
	 */
	namespace Json
	{
		/**
		 * \fn boost::property_tree::ptree	parse(const std::string& json)
		 * \brief Parse string json to boost::property_tree::ptree
		 * \param json
		 * \return boost::property_tree::ptree
		 */
		boost::property_tree::ptree	parse(const std::string& json);

		/**
	     * \fn const std::string			produce(const boost::property_tree::ptree& jsonPtree)
	     * \brief Convert boost::property_tree::ptree to string
	     * \param jsonPtree
	     * \return string
	     */
		const std::string			produce(const boost::property_tree::ptree& jsonPtree);
	}
}