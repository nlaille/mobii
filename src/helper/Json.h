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
	namespace Json
	{
		boost::property_tree::ptree	parse(const std::string& json);
		const std::string			produce(const boost::property_tree::ptree& jsonPtree);
	}
}