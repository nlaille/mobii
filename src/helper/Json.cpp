#include "Json.h"

boost::property_tree::ptree	Helper::Json::parse(const std::string& json)
{
	std::stringstream jsonStringStream(json);
    boost::property_tree::ptree jsonPtree;

    boost::property_tree::read_json(jsonStringStream, jsonPtree);
	return jsonPtree;
}

const std::string			Helper::Json::produce(const boost::property_tree::ptree& jsonPtree)
{
   std::ostringstream   jsonStringStream; 

   boost::property_tree::write_json(jsonStringStream, jsonPtree);
   return jsonStringStream.str();
}