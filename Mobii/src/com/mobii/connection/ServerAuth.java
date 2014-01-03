/**
 * @file	ServerAuth.java
 * @author	Sebastien GUILLERM
 * @brief	Authentication on the server.
 */

package com.mobii.connection;

import org.json.JSONException;
import org.json.JSONObject;

import com.mobii.protocol.IProtocolCom;
import com.mobii.protocol.TcpPacket;

import android.util.Log;

public class ServerAuth {

	public ServerAuth() {}
	public ServerAuth(ServerAuth sa) {}

	/**
	 * @brief	Server authentication.
	 * @details	Authenticates on the server.
	 * @param	client	A TcpPacket object.
	 * @param	uid	The user id string.
	 * @param	pwd	The user password string (password received by the server at the first connection).
	 * @return	Returns true if the authentication was successful; otherwise returns false.
	 */
	public boolean	servAuth(TcpClient client, String uid, String pwd) {
		TcpPacket tcpPacket = new TcpPacket();
		JSONObject jObj = new JSONObject();
		boolean isAuth = false;

		try {
			jObj.put("uid", uid);
			jObj.put("password", pwd);
		} catch (JSONException e) {
			Log.e("JSon UID", e.toString());
		}
		String msg = jObj.toString();
		byte[] packet = tcpPacket.getPacket(IProtocolCom.initSource.getBytes(), IProtocolCom.initDest.getBytes(),
											IProtocolCom.d_srvConnect, IProtocolCom.o_mobToSrvConnect, msg);
		client.sendPacket(packet);
		Log.i("servConnect", "Authentication packet sent");

		byte[] recvPacket = client.recvPacket();
		tcpPacket.parsePacket(recvPacket);
		tcpPacket.parseMsg(recvPacket);

		if (Integer.parseInt(tcpPacket.getDispatch(), 16) == IProtocolCom.d_srvConnectVal &&
				Integer.parseInt(tcpPacket.getOpcode(), 16) == IProtocolCom.o_srvConnectNotifVal) {
			Log.i("Server connection packet", "Phone now logged in to the server");
			isAuth = true;
		} else {
			Log.i("Server connection packet", "Phone not logged in to the server");
		}
		return isAuth;
	}
}
