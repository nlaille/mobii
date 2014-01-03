/**
 * @file	TcpClient.java
 * @author	Sebastien GUILLERM
 * @brief	TCP packets functions.
 */

package com.mobii.protocol;

import java.io.UnsupportedEncodingException;
import java.util.Locale;

import org.json.JSONException;
import org.json.JSONObject;

import com.mobii.MobiiConnectionActivity;
import com.mobii.connection.TcpClient;
import com.mobii.connection.Uid;
import com.mobii.features.calendar.Calendar;
import com.mobii.features.contacts.Contacts;
import com.mobii.features.infos.MobileInfos;
import com.mobii.features.sms.Sms;
import com.mobii.utils.ByteArrays;
import com.mobii.utils.JsonParser;
import com.mobii.utils.Md5;

import android.content.Context;
import android.content.SharedPreferences;
import android.util.Log;

public class TcpPacket {

	public SharedPreferences	mSharedMobii = MobiiConnectionActivity.getSharedPreferences();
	public Context				mCtx = MobiiConnectionActivity.getContextApplication();

	private ByteArrays			mByteArrays = new ByteArrays();
	private byte[]				mRecvSource;
	private String				mRecvDispatch, mRecvOpcode, mRecvMsg;
	private int 				mRecvPacketSize;

	public TcpPacket() {}
	public TcpPacket(TcpPacket p) {}

	/**
	 * @brief	Gets packets.
	 * @details	Creates a packet to send.
	 * @param	src A byte array corresponding to the user id.
	 * @param	dest A byte array corresponding to the client id.
	 * @param	dispatch Dispatch.
	 * @param	opcode Opcode.
	 * @param	data Message.
	 * @return	Returns a byte array which is the created packet to send.
	 */
	public byte[]	getPacket(byte[] src, byte[] dest, String dispatch, String opcode, String data) {
		byte[] header = "".getBytes(),
				msg = "".getBytes();
		int size = 0;
		try {
			msg = data.getBytes("UTF-8");
			size = msg.length;
		} catch (UnsupportedEncodingException e) {
			Log.e("TcpPacket", "getPacket", e);
		}
		header = new Header(this.mCtx).getHeader(src, dest, dispatch, opcode, size);
		return new ByteArrays().concatByteArrays(header, msg);
	}

	/**
	 * @brief	Packets parsing.
	 * @details	Parses the received packets to get destination, dispatch and opcode.
	 * @param	packet A received packet.
	 */
	public void		parsePacket(byte[] packet) {
		Log.i("TcpPacket", "parsePacket");
		String sourceStr = this.mByteArrays.byteArrayToHex(packet).substring(IProtocolCom.p_source * 2,
																			(IProtocolCom.p_source + IProtocolCom.s_source) * 2);
		byte[] source = new byte[IProtocolCom.s_source];
		for (int i = 0, j = 0; i < IProtocolCom.s_source; i++, j += 2) {
			String str = sourceStr.substring(j, j+2);
			source[i] = ((Integer) Integer.parseInt(str, 16)).byteValue();
		}
		this.mRecvSource = source;
		this.mRecvDispatch = Integer.toHexString((int) packet[IProtocolCom.p_dispatch]);
		if ((this.mRecvOpcode = Integer.toHexString((int) packet[IProtocolCom.p_opcode])).equals("fffffff0"))
			this.mRecvOpcode = "ff";
		String sizeStr = this.mByteArrays.byteArrayToHex(packet).substring(IProtocolCom.p_msgSize * 2,
																  (IProtocolCom.p_msgSize + IProtocolCom.s_msgSize) * 2);
		byte[]	msgSize = new byte[IProtocolCom.s_msgSize];
		for (int i = 0, j = 0; i < IProtocolCom.s_msgSize; i++, j += 2) {
			String str = sizeStr.substring(j, j+2);
			msgSize[i] = ((Integer) Integer.parseInt(str, 16)).byteValue();
		}
		this.mRecvPacketSize = this.mByteArrays.byteArrayToint_le(msgSize);

		Log.i("recvSource", this.mByteArrays.byteArrayToHex(this.mRecvSource));
		Log.i("recvDispatch", this.mRecvDispatch);
		Log.i("recvOpcode", this.mRecvOpcode);
		Log.i("recvPacketSize byteArrayToInt", String.valueOf(this.mRecvPacketSize));
	}

	/**
	 * @brief	Messages parsing.
	 * @details	Parses the messages of received packets.
	 * @param	packet A received packet.
	 */
	public void		parseMsg(byte[] packet) {
		Log.i("TcpPacket", "parseMsg");
		StringBuilder sb = new StringBuilder();
		if (this.mRecvPacketSize == 0) {
			sb.append("");
			Log.i("recvMsg", "No Message");
		} else {
			String msg = this.mByteArrays.byteArrayToHex(packet)
										 .substring(IProtocolCom.p_msg * 2, (IProtocolCom.p_msg + this.mRecvPacketSize) * 2);
			for (int i = 0; i < msg.length(); i += 2) {
				int c = Integer.parseInt(msg.substring(i, i + 2), 16);
				sb.append(Character.toChars(c));
			}
		}
		this.mRecvMsg = sb.toString();
		Log.i("recvMsg", this.mRecvMsg);
	}

	/**
	 * @brief	Destination getter.
	 * @return	Returns the client id.
	 */
	public byte[]	getDest() {
		this.mSharedMobii.edit().putString("dest_uid", new ByteArrays().byteArrayToHex(this.mRecvSource)).commit();
		return this.mRecvSource;
	}

	/**
	 * @brief	Dispatch getter.
	 * @return	Returns the dispatch of the received packet.
	 */
	public String	getDispatch() {
		return this.mRecvDispatch;
	}

	/**
	 * @brief	Opcode getter.
	 * @return	Returns the opcode of the received packet.
	 */
	public String	getOpcode() {
		return this.mRecvOpcode;
	}

	/**
	 * @brief	Message getter.
	 * @return	Returns the message of the received packet.
	 */
	public String	getMsg() {
		return this.mRecvMsg;
	}

	/**
	 * @brief	Client connection request.
	 * @details	Checks if a client is already connected and if the password is valid.
	 * @param	client TcpClient object.
	 * @param	tcpPacket TcpPacket object.
	 * @param	cliConnected A boolean which is true if a client is connected.
	 * @return	Returns true if no client is connected and password is valid; otherwise returns false.
	 */
	public boolean	recvCliConnectReq(TcpClient client, TcpPacket tcpPacket, boolean cliConnected) {
		Log.i("recvCliConnectReq", "Client Connection Request");

		String dispatch = IProtocolCom.d_cliConnect, opcode = IProtocolCom.o_cliConnectDenied,
				code = "", password = this.mSharedMobii.getString("password", "");
		byte[] packet, recvPacket,
				src = new Uid().parseUid(this.mSharedMobii.getString("user_id", "")),
				dest = tcpPacket.getDest();
		boolean connected = false;

		try {
			code = new JSONObject(this.mRecvMsg).getString("code");
		} catch (JSONException e) {
			Log.e("TcpPacket", "recvCliConnectReq", e);
		}
		if (cliConnected == true || code.compareTo(new Md5().encode(password)) != 0)
			Log.i("opcode", "refuse");
		else {
			Log.i("dest", this.mByteArrays.byteArrayToHex(dest));
			opcode = IProtocolCom.o_cliConnectAccept;
			Log.i("opcode", "accept");
		}
		packet = getPacket(src, dest, dispatch, opcode, "");
		Log.i("Client Connection Response Packet", new ByteArrays().byteArrayToHex(packet));
		client.sendPacket(packet);
		Log.i("recvCliConnectReq", "Client connection response packet sent");
		recvPacket = client.recvPacket();
		parsePacket(recvPacket);
		parseMsg(recvPacket);
		if (Integer.parseInt(this.mRecvDispatch, 16) == IProtocolCom.d_comVal &&
				Integer.parseInt(this.mRecvOpcode, 16) == IProtocolCom.o_cliConnectedNotifVal) {
			Log.i("recvCliConnectReq", "Client connected to the mobile");
			connected = true;
			sendInfos(client, src);
		 }
		return connected;
	}

	/**
	 * @brief	Sending infos.
	 * @details	Sends mobile information.
	 * @param	client TcpClient object.
	 * @param	src A byte array corresponding to the user id.
	 */
	public void		sendInfos(TcpClient client, byte[] src) {
		MobileInfos mb = new MobileInfos(this.mCtx);
		String msg = mb.getMobileInfosJson();
		byte[] packet = getPacket(src, this.mRecvSource, IProtocolCom.d_com, IProtocolCom.o_jsonMsg, msg);
		client.sendPacket(packet);
		Log.i("Mobile infos packet sent", this.mByteArrays.byteArrayToHex(packet));
	}

	/**
	 * @brief	Packets handling.
	 * @details	Handles receiving packets.
	 * @param	client TcpClient object.
	 * @param	tcpPacket TcpPacket object.
	 * @param	cliConnected A boolean which is true if a client is connected.
	 * @return	Returns false if a client disconnection packet has been received; otherwise returns true.
	 */
	public boolean	recvCom(TcpClient client, TcpPacket tcpPacket, boolean cliConnected) {
		Log.i("recvCom", "Communication");

		Sms sms = new Sms();
		Contacts contacts = new Contacts();
		Calendar calendar = new Calendar();
		byte[]	src = new Uid().parseUid(this.mSharedMobii.getString("user_id", "")),
				dest = tcpPacket.getDest(),
				packet;

		switch (Integer.parseInt(this.mRecvOpcode, 16)) {

		case IProtocolCom.o_cliDisconnectedVal:
			Log.i("recvCom", "Client disconnected Packet");
			if (cliConnected == true) {
				cliConnected = false;
				Log.i("CliDeco", "Client disconnected");
			}
			else
				Log.i("CliDeco", "No client connected");
			break;

		case IProtocolCom.o_jsonMsgVal:
			Log.i("recvCom", "Json Packet");

			String recvAction = new JsonParser().getAction(this.mRecvMsg);
			switch (IProtocolAction.cliToMob.valueOf(recvAction.toUpperCase(new Locale("us")))) {

			case REQ_SMS:
				Log.i("JSon", "REQ_SMS");
				try {
					String msg = sms.getSms(this.mRecvMsg);
					Log.i("Action SMS", msg);
					Log.i("Action SMS size", String.valueOf(msg.length()));
					packet = getPacket(src, dest, IProtocolCom.d_com, IProtocolCom.o_jsonMsg, msg);
					Log.i("JSon Packet", this.mByteArrays.byteArrayToHex(packet));
					client.sendPacket(packet);
					Log.i("recvCom", "Sms packet sent");
				} catch (Exception e) {
					Log.e("TcpPacket", "recvCom : REQ_SMS", e);
				}
				break;

			case REQ_SENDSMS:
				Log.i("JSon", "REQ_SENDSMS");
				String smsPhone = "", smsMsg = "";
				try {
					smsPhone = new JSONObject(this.mRecvMsg).getString("dest");
					smsMsg = new JSONObject(this.mRecvMsg).getString("body");
				} catch (JSONException e) {
					Log.e("TcpPacket", "recvCom : REQ_SENDSMS", e);
				}
				sms.sendSms(smsPhone, smsMsg);
				break;

			case REQ_CONTACTS:
				Log.i("JSon", "REQ_CONTACTS");
				try {
					String msg = contacts.getContacts();
					Log.d("CONTACTS MSG", msg);
					packet = getPacket(src, dest, IProtocolCom.d_com, IProtocolCom.o_jsonMsg, msg);
					client.sendPacket(packet);
					Log.i("recvCom", "Contacts packet sent");
				} catch (Exception e) {
					Log.e("TcpPacket", "recvCom : REQ_CONTACTS", e);
				}
				break;

			case NEW_CONTACT:
				Log.i("JSon", "NEW_CONTACT");
				try {
					contacts.addContact(this.mRecvMsg);
				} catch (Exception e) {
					Log.e("TcpPacket", "recvCom : NEW_CONTACT", e);
				}
				break;

			case DEL_CONTACT:
				Log.i("JSon", "DEL_CONTACT");
				try {
					contacts.delContact(this.mRecvMsg);
				} catch (Exception e) {
					Log.e("TcpPacket", "recvCom : DEL_CONTACT", e);
				}
				break;

			case UPT_CONTACTS:
				Log.i("JSon", "UPT_CONTACTS");
				try {
					contacts.uptContact(this.mRecvMsg);
				} catch (Exception e) {
					Log.e("TcpPacket", "recvCom : UPT_CONTACTS", e);
				}
				break;

			case REQ_CALENDAR:
				Log.i("JSon", "REQ_CALENDAR");
				try {
					String msg = calendar.getEvents();
					Log.d("CALENDAR MSG", msg);
					packet = getPacket(src, dest, IProtocolCom.d_com, IProtocolCom.o_jsonMsg, msg);
					client.sendPacket(packet);
					Log.i("recvCom", "Calendar packet sent");
				} catch (Exception e) {
					Log.e("TcpPacket", "recvCom : REQ_CALENDAR", e);
				}
				break;

			case NEW_EVENT:
				Log.i("JSon", "NEW_EVENT");
				try {
					calendar.addEvent(this.mRecvMsg);
				} catch (Exception e) {
					Log.e("TcpPacket", "recvCom : NEW_EVENT", e);
				}
				break;

			case DEL_EVENT:
				Log.i("JSon", "DEL_EVENT");
				try {
					calendar.delEvent(this.mRecvMsg);
				} catch (Exception e) {
					Log.e("TcpPacket", "recvCom : DEL_EVENT", e);
				}
				break;

			case UPT_EVENTS:
				Log.i("JSon", "UPT_EVENTS");
				try {
					calendar.uptEvent(this.mRecvMsg);
				} catch (Exception e) {
					Log.e("TcpPacket", "recvCom : UPT_EVENTS", e);
				}
				break;

			case REQ_IMAGES:
				Log.i("JSon", "REQ_IMAGES");
				try {
				} catch (Exception e) {
					Log.e("TcpPacket", "recvCom : REQ_IMAGES", e);
				}
				break;

			case DEL_IMAGE:
				Log.i("JSon", "DEL_IMAGE");
				try {
				} catch (Exception e) {
					Log.e("TcpPacket", "recvCom : DEL_IMAGE", e);
				}
				break;

			default:
				break;
			}
			break;
		default:
			break;
		}
		return cliConnected;
	}
}
