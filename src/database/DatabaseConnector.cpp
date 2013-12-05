#include "DatabaseConnector.h"

DatabaseConnector::DatabaseConnector(const std::size_t numberOfConnection)
   : m_poolConnection(numberOfConnection), m_numberOfConnection(numberOfConnection)
{
}

DatabaseConnector::~DatabaseConnector()
{
}

#include <iostream>
void		DatabaseConnector::setConnection(const std::string& connexion)
{
	for (size_t i = 0; i != m_numberOfConnection; ++i)
	{
		try
		{
			soci::session& sql = m_poolConnection.at(i);

			sql.open(connexion);
		}
		catch (const soci::soci_error& sociError)
		{
			std::cout << sociError.what() << std::endl;
		}
	}
}
