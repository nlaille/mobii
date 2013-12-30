#pragma once

/**
 * \file Random.h
 * \brief Declaration of Random Helpers
 * \author Nicolas Laille
 * \version 1.0
 * \date 24 Octobre 2013
 */
#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace Helper
{
	namespace Random
	{
		/**
		 * \fn int			generate(int min = 1, int max = 255255)
		 * \brief Generate a random number
		 * \param min
		 * \param max
		 * \return random number
		 */
		int			generate(int min = 1, int max = 255255);
	}
}