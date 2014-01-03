/**
 * @file	IProtocolCom.java
 * @author	Sebastien GUILLERM
 * @brief	Communication protocol interface.
 */

package com.mobii.protocol;

public interface IProtocolCom {

	/*
	 * Size (s) and position (p)
	 */
	public int		s_source					= 6;
	public int		s_dest						= 6;
	public int		s_dispatch					= 1;
	public int		s_opcode					= 1;
	public int		s_msgSize					= 4;

	public int		s_empty						= 0;
	public int		s_id						= 6;

	public int		s_header					= s_source + s_dest + s_dispatch + s_opcode + s_msgSize;

	public int		p_source					= 0;
	public int		p_dest						= p_source + s_source; //6;
	public int		p_dispatch					= p_dest + s_dest; //12;
	public int		p_opcode					= p_dispatch + s_dispatch; //13;
	public int		p_msgSize					= p_opcode + s_opcode; //14;
	public int		p_msg						= p_msgSize + s_msgSize; //18;


	/*
	 * Source and destination for server connection
	 */
	public String	initSource					= "\u0000\u0000\u0000\u0000\u0000\u0000";
	public String	initDest					= "\u0000\u0000\u0000\u0000\u0000\u0000";


	/*
	 * Dispatch
	 */
	public String	d_srvReport					= "\u0000";
	public int		d_srvReportVal				= 0;

	public String	d_srvConnect				= "\u0005";
	public int		d_srvConnectVal				= 5;

	public String	d_cliConnect				= "\u0010";
	public int		d_cliConnectVal				= 16;

	public String	d_cliConnectReq				= "\u0020";
	public int		d_cliConnectReqVal			= 32;

	public String	d_com						= "\u0030";
	public int		d_comVal					= 48;

	public String	d_udpHp						= "\u0040";
	public int		d_udpHpVal					= 64;

	public String	d_auth						= "\u0050";
	public int		d_authVal					= 80;


	/*
	 * Opcode
	 */
	public String	o_srvError					= "\u00ff";
	public int		o_srvErrorVal				= 255;

	public String	o_mobToSrvConnect			= "\u0000";
	public int		o_mobToSrvConnectVal		= 0;
	public String	o_cliToSrvConnect			= "\u0001";
	public int		o_cliToSrvConnectVal		= 1;
	public String	o_srvConnectNotif			= "\u0011";
	public int		o_srvConnectNotifVal		= 17;

	public String	o_cliConnectAccept			= "\u0010";
	public int		o_cliConnectAcceptVal		= 16;
	public String	o_cliConnectDenied			= "\u0011";
	public int		o_cliConnectDeniedVal		= 17;

	public String	o_cliConnectReq				= "\u0010";
	public int		o_cliConnectReqVal			= 16;

	public String	o_cliDisconnected			= "\u0000";
	public int		o_cliDisconnectedVal		= 0;
	public String	o_cliConnectedNotif			= "\u0001";
	public int		o_cliConnectedNotifVal		= 1;
	public String	o_jsonMsg					= "\u0010";
	public int		o_jsonMsgVal				= 16;
	public String	o_hp						= "\u0020";
	public int		o_hpVal						= 32;

	public String	o_udpHp						= "\u0000";
	public int		o_udpHpVal					= 0;
	public String	o_udpNotif					= "\u0001";
	public int		o_udpNotifVal				= 1;

	public String	o_authReq					= "\u0000";
	public int		o_authReqVal				= 0;
	public String	o_authSuccess				= "\u0001";
	public int		o_authSuccessVal			= 1;
	public String	o_authFailure				= "\u0002";
	public int		o_authFailureVal			= 2;
	public String	o_uidReservReq				= "\u0010";
	public int		o_uidReservReqVal			= 16;
	public String	o_uidReserved				= "\u0011";
	public int		o_uidReservedVal			= 17;
	public String	o_uidExists					= "\u0012";
	public int		o_uidExistsVal				= 18;
}
