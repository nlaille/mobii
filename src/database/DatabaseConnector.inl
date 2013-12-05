#include "Model.h"
#include "exception/NoDataFound.h"

template <typename T>
T     DatabaseConnector::select(const std::string& selectQuery)
{
   soci::session	session(m_poolConnection);
   T				result;

   session << selectQuery, soci::into(result);

   if (!session.got_data())
	   throw NoDataFound("[DatabaseConnector] No data found.");
   return result;
}

template <typename T, typename U>
T		DatabaseConnector::select(const std::string& selectQuery, const U& condition)
{
   soci::session	session(m_poolConnection);
   T				result;

   session << selectQuery, soci::use(condition), soci::into(result);

   if (!session.got_data())
	   throw NoDataFound("[DatabaseConnector] No data found.");
   return result;
}

template <typename T>
std::vector<T>		DatabaseConnector::selects(const std::string& selectQuery)
{
   soci::session	session(m_poolConnection);
   std::vector<T>	result;
   soci::rowset<soci::row>		rowset = (session.prepare << selectQuery);

   for (soci::rowset<soci::row>::const_iterator it = rowset.begin(); it != rowset.end(); ++it)
   {
	  T   data;

	  data << (*it);
	  result.push_back(data);
	}
   return result;
}

template <typename T, typename U>
std::vector<T>		DatabaseConnector::selects(const std::string& selectQuery, const U& condition)
{
   soci::session	session(m_poolConnection);
   std::vector<T>	result;
   soci::rowset<soci::row>		rowset = (session.prepare << selectQuery, soci::use(condition));

   for (soci::rowset<soci::row>::const_iterator it = rowset.begin(); it != rowset.end(); ++it)
   {
	  T   data;

	  data << (*it);
	  result.push_back(data);
	}
   return result;
}

template <typename T>
void     DatabaseConnector::update(const std::string& updateQuery, T& model)
{
   soci::session	session(m_poolConnection);

   session << updateQuery, soci::use(model);
}

template <typename T>
void     DatabaseConnector::insert(const std::string& insertQuery, T& model)
{
   soci::session	session(m_poolConnection);

   session << insertQuery, soci::use(model);
}
