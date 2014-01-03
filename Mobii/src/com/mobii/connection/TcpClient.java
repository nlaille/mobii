/**
 * @file	TcpClient.java
 * @author	Sebastien GUILLERM
 * @brief	TCP client functions.
 */

package com.mobii.connection;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.InetAddress;
import java.net.Socket;

import com.mobii.MobiiConnectionActivity;
import com.mobii.protocol.IProtocolCom;
import com.mobii.protocol.TcpPacket;
import com.mobii.utils.ByteArrays;

import android.content.Context;
import android.content.SharedPreferences;
import android.util.Log;

public class TcpClient {

	public static SharedPreferences	mSharedMobii = MobiiConnectionActivity.getSharedPreferences();
	public static Context			mCtx = MobiiConnectionActivity.getContextApplication();

	public boolean					mRun = false, mLog = false;

	private static final String		mServerIp = IConnection.serverIp;
	private static final int		mServerPort = IConnection.serverPortTcp;
	private byte[]					mRecvPacket;
	private Socket					mSocket;
	private DataOutputStream		mOutStream;
	private DataInputStream			mInStream;
	private TcpPacket				mTcpPacket;

	public TcpClient() {}

	public TcpClient(TcpClient c) {}

	/**
	 * @brief	Client launching.
	 * @details	Creates a SSL socket and connects the server.
	 */
	public void		run() {
		this.mSocket = null;
		try {
			Log.i("TCPClient", "Trying to connect the server...");
			InetAddress serverAddr = InetAddress.getByName(mServerIp);
			this.mSocket = new SslContext(serverAddr, TcpClient.mServerPort).getSslSocket();
			Log.i("SSL Socket", "Opened");
			this.mOutStream = new DataOutputStream(this.mSocket.getOutputStream());
			this.mInStream = new DataInputStream(this.mSocket.getInputStream());
			this.mRun = true;
			mSharedMobii.edit().putBoolean("isConnected", true).commit();
			Log.i("Client", "Connected");
		}
		catch (Exception e) {
			this.mRun = false;
			Log.i("Client", "Not connected");
			Log.i("SSL Socket", "Not opened");
		}
	}

	/**
	 * @brief	Server login.
	 * @details	Logs in to the server.
	 */
	public void 	log() {
		if (mRun) {
			try {
				Log.i("TCPClient", "Trying to login to the server...");
				while (new Uid().reserveUid(this, mSharedMobii.getString("user_id", "")) == false);
				Log.i("Reserved UID", mSharedMobii.getString("user_id", ""));
				Log.i("Password", mSharedMobii.getString("user_pwd", "Empty password"));

				if (!new ServerAuth().servAuth(this, mSharedMobii.getString("user_id", ""), mSharedMobii.getString("user_pwd", "")))
					stop();
				else {
					this.mLog = true;
					mSharedMobii.edit().putBoolean("isLogged", true).commit();
					Log.i("Client", "Logged");
				}
			} catch (Exception e) {
				mRun = false;
				Log.e("TcpClient", "log", e);
				Log.i("Client", "Not logged");
				Log.i("SSL Socket", "Closed");
			}
		}
	}

	/**
	 * @brief	Listening loop.
	 * @details	Listens for incoming connections and packets.
	 */
	public void		listen() {
		boolean cliConnected = false;
		while (this.mRun) {
			Log.i("tcpListen", cliConnected ? "Waiting for a packet..." : "Waiting for a client...");

			this.mRecvPacket = recvPacket();
			this.mTcpPacket = new TcpPacket();
			this.mTcpPacket.parsePacket(this.mRecvPacket);
			this.mTcpPacket.parseMsg(this.mRecvPacket);

			try {
				switch (Integer.parseInt(this.mTcpPacket.getDispatch(), 16)) {

					case IProtocolCom.d_srvReportVal:
						if (Integer.parseInt(this.mTcpPacket.getOpcode(), 16) == IProtocolCom.o_srvErrorVal)
							Log.i("Server report packet", "Server error");
						break;

					case IProtocolCom.d_cliConnectReqVal:
						if (Integer.parseInt(this.mTcpPacket.getOpcode(), 16) == IProtocolCom.o_cliConnectReqVal)
							cliConnected = this.mTcpPacket.recvCliConnectReq(this, this.mTcpPacket, cliConnected);
						break;

					case IProtocolCom.d_comVal:
						if (Integer.parseInt(this.mTcpPacket.getOpcode(), 16) != IProtocolCom.o_cliConnectedNotifVal &&
							cliConnected == true)
							cliConnected = this.mTcpPacket.recvCom(this, this.mTcpPacket, cliConnected);
						break;

					default:
						break;
				}
			} catch (Exception e) {
				Log.e("TcpClient", "listen", e);
			}
		}
	}

	/**
	 * @brief	Client stopping.
	 * @details	Stops the client by closing the socket.
	 */
	public void 	stop() {
		try {
			this.mSocket.close();
			if (this.mOutStream != null) {
				this.mOutStream.flush();
				this.mOutStream.close();
			}
			this.mRun = false;
			this.mInStream = null;
			this.mOutStream = null;
			this.mRecvPacket = null;

			mSharedMobii.edit().putBoolean("isConnected", false).commit();
			mSharedMobii.edit().putBoolean("isLogged", false).commit();
			Log.i("Client", "Stopped");
		} catch (IOException e) {
			//e.printStackTrace();
			Log.e("TcpClient", "stopClient", e);
		}
	}

	/**
	 * @brief	Sending packets.
	 * @details	Sends packets to the server.
	 * @param	msg	The packet to send.
	 */
	public void 	sendPacket(String msg) {
		if (this.mOutStream != null) {
			try {
				this.mOutStream.write(msg.getBytes("UTF-8"));
				this.mOutStream.flush();
			} catch (IOException e) {
				//e.printStackTrace();
				Log.i("TcpClient", "sendPacket", e);
			}
		}
	}

	/**
	 * @brief	Sending packets.
	 * @details	Sends packets to the server.
	 * @param	msg	The packet to send.
	 */
	public void 	sendPacket(byte[] message) {
		if (this.mOutStream != null) {
			try {
				this.mOutStream.write(message);
				this.mOutStream.flush();
			} catch (IOException e) {
				//e.printStackTrace();
				Log.i("TcpClient", "sendPacket", e);
			}
		}
	}

	/**
	 * @brief	Receiving packets.
	 * @details	Receives packets from the server.
	 * @return	Returns a byte array corresponding to the received packet.
	 */
	public byte[]	recvPacket() {
		byte[] recvPacket = new byte[512];
		try {
			this.mOutStream.flush();
			if (this.mInStream.read(recvPacket) != -1) {
				Log.i("recvPacket", "A packet has been received");
				Log.i("recvPacket", new ByteArrays().byteArrayToHex(recvPacket));
			}
		} catch (IOException e) {
			Log.i("TcpClient", "recvPacket", e);
		}
		return recvPacket;
	}

	/**
	 * @brief	Socket getter.
	 * @return	Returns the created socket.
	 */
	public Socket	getSocket() {
		return this.mSocket;
	}
}
