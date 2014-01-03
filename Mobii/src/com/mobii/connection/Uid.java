/**
 * @file	Uid.java
 * @author	Sebastien GUILLERM
 * @brief	User id functions.
 */

package com.mobii.connection;

import java.util.Locale;
import java.util.Random;

import org.json.JSONException;
import org.json.JSONObject;

import com.mobii.MobiiConnectionActivity;
import com.mobii.features.infos.MobileInfos;
import com.mobii.protocol.IProtocolCom;
import com.mobii.protocol.TcpPacket;

import android.content.Context;
import android.content.SharedPreferences;
import android.util.Log;

public class Uid {

	public static SharedPreferences	mSharedMobii = MobiiConnectionActivity.getSharedPreferences();
	public static Context			mCtx = MobiiConnectionActivity.getContextApplication();

	private TcpPacket				mTcpPacket = new TcpPacket();

	public Uid() {}
	public Uid(Uid uid) {}

	/**
	 * @brief	Change user id string to display it.
	 * @details	Add a space each two characters to the user id string.
	 * @param	uid	The user id string.
	 * @return	Returns the modified uid.
	 */
	public String	displayUid(String uid) {
		String affUid = "";
		if (uid.isEmpty() == true)
			affUid = "UID";
		else
			for (int i = 0; i < uid.length(); i += 2) {
				affUid += uid.substring(i, i + 2).toUpperCase(new Locale("us"));
				if (i + 2 < uid.length())
					affUid += " ";
			}
		return affUid;
	}

	/**
	 * @brief	Generate uid.
	 * @details	Generate a random user id.
	 * @return	Returns the generated string.
	 */
	public String	generateUid() {
		Random rand = new Random();
		String uid = "", tmp = "";
		for (int i = 0; i < IProtocolCom.s_id; i++) {
			tmp = Integer.toHexString(rand.nextInt(256));
			uid += tmp.length() == 2 ? tmp : "0" + tmp;
		}
		return uid;
	}

	/**
	 * @brief	Reserves a uid.
	 * @details	Verifies if a user id already exists. If not, requests the server to reserve it.
	 * @param	client	A TcpClient object.
	 * @param	uid	The user id string.
	 * @return	Returns true if user id already exists; otherwise returns false.
	 */
	public boolean	reserveUid(TcpClient client, String uid) {
		boolean isReserved = false;
		String pwd = "";

		if (uid.isEmpty() == false) {
			Log.i("UID", "Already reserved");
			isReserved = true;
		}
		else {
			Log.i("UID", "Not yet reserved");
			uid = new Uid().generateUid();
			MobileInfos mb = new MobileInfos(mCtx);
			JSONObject jObj = new JSONObject();
			try {
				jObj.put("uid", uid);
				jObj.put("imei", mb.getImei());
				jObj.put("model", mb.getModel());
				jObj.put("manufacturer", mb.getManufacturer());
			} catch (JSONException e) {
				Log.e("Uid", "reservUid", e);
			}
			String msg = jObj.toString();
			byte[] packet = this.mTcpPacket.getPacket(IProtocolCom.initSource.getBytes(), IProtocolCom.initDest.getBytes(), 
														IProtocolCom.d_auth, IProtocolCom.o_uidReservReq, msg);
			client.sendPacket(packet);
			Log.i("TcpClient: reservUid", "ID reservation packet sent");

			byte[] recvPacket = client.recvPacket();
			this.mTcpPacket.parsePacket(recvPacket);
			this.mTcpPacket.parseMsg(recvPacket);

			if (Integer.parseInt(this.mTcpPacket.getDispatch(), 16) == IProtocolCom.d_authVal &&
					Integer.parseInt(this.mTcpPacket.getOpcode(), 16) == IProtocolCom.o_uidReservedVal) {
				Log.i("ID Reservation packet", uid + " : ID reserved");
				mSharedMobii.edit().putString("user_id", uid).commit();
				msg = this.mTcpPacket.getMsg().replaceAll("/ |\\{|\\}|\"|\\n", "");
				String[] tab = msg.split(","), passwd;
				if (!tab[1].isEmpty() && tab[1].contains("password")) {
					passwd = tab[1].split(":");
					if (!passwd[1].isEmpty())
						pwd = passwd[1].trim();
				}
				mSharedMobii.edit().putString("user_pwd", pwd).commit();
				isReserved = true;
			}
			else if (Integer.parseInt(this.mTcpPacket.getDispatch(), 16) == IProtocolCom.d_authVal &&
					Integer.parseInt(this.mTcpPacket.getOpcode(), 16) == IProtocolCom.o_uidExistsVal) {
				Log.i("ID Reservation packet", "ID already exists");
			}
			else
				Log.i("uidReserv()", "Packet Error");
			}
		return isReserved;
	}

	/**
	 * @brief	Parse uid.
	 * @details	Convert the user id string to a byte array.
	 * @param	uid	The user id string.
	 * @return	Returns the modified uid.
	 */
	public byte[]	parseUid(String uid) {
		byte[] bUid = new byte[IProtocolCom.s_id];
		String tmp = "";
		for (int i = 0, j = 0; i < uid.length(); i += 2, j++) {
			tmp = uid.substring(i, i+2);
			bUid[j] = ((Integer) Integer.parseInt(tmp, 16)).byteValue();
		}
		return bUid;
	}
}
