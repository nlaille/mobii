/**
 * @file	IConnection.java
 * @author	Sebastien GUILLERM
 * @brief	Connection interface.
 */

package com.mobii.connection;

public interface IConnection {

	// Serveur distant
	final public String	serverIp = "163.5.84.178";

	// Serveur distant epitech
	//final public String	serverIp = "10.224.9.178";
	//final public String	serverIp = "10.41.176.105";

	// Serveur local
	//final public String	serverIp = "192.168.1.26";

	final public int	serverPortTcp = 3689;
	final public int	serverPortUdp = 53789;
}
