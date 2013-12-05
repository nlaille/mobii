#pragma once

/**
 * \file Acceptor.h
 * \brief Declaration of Acceptor
 * \author Nicolas Laille
 * \version 1.0
 * \date 24 Octobre 2013
 */

/**
 * \class Acceptor
 * \brief Interface of socket acceptor
 */
class Acceptor
{
public:
   virtual ~Acceptor(void) {}

   /**
	* \fn void start(void)
	* \brief Enable acceptor to accept new connection
	*/
   virtual void start(void) = 0;
   /**
	* \fn void start(void)
	* \brief Disable acceptor to accept new connection
	*/
   virtual void stop(void) = 0;
};
