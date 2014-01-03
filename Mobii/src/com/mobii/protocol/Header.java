/**
 * @file	Header.java
 * @author	Sebastien GUILLERM
 * @brief	Creates headers.
 */

package com.mobii.protocol;

import com.mobii.utils.ByteArrays;

import android.content.Context;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;

public class Header {

	private Context		mCtx;
	private ByteArrays	mByteArrays;

	public Header(Context ctx) {
		this.mCtx = ctx;
		this.mByteArrays = new ByteArrays();
	}

	public Header(Header h) {
		this.mCtx = h.mCtx;
		this.mByteArrays = h.mByteArrays;
	}

	/**
	 * @brief	Parses a MAC address.
	 * @details	Deletes the characters ":" from a MAC address and convert it from hexadecimal to String.
	 * @param	str	The MAC address string.
	 * @return	A byte array which is the modified MAC address.
	 */
	private byte[]	parseMacAddr(String str) {
		String[] mac = str.split(":");
		byte[] macAddr = new byte[6];
		for (int i = 0; i < mac.length; i++) {
			macAddr[i] = ((Integer) Integer.parseInt(mac[i], 16)).byteValue();
		}
		return macAddr;
	}

	public String	parseDestAddr(byte[] packet) {
		StringBuffer sb = new StringBuffer();
		for (int i = IProtocolCom.p_source; i < IProtocolCom.p_dest; i++) {
			String str = Integer.toHexString((int) packet[i]);
			if (str.length() < 2) {
				sb.append("0" + str);
			} else if (str.length() > 2) {
				sb.append(str.substring(6));
			} else {
				sb.append(str);
			}
			if (i < 5) {
				sb.insert(i+2*(i+1), ':');
			}
		}
		return sb.toString();
	}

	public byte[]	getDest(String addr) {
		return parseMacAddr(addr);
	}

	public String	getIp() {
		WifiManager wifiManager = (WifiManager) this.mCtx.getSystemService(Context.WIFI_SERVICE);
		WifiInfo wifiInfo = wifiManager.getConnectionInfo();
		final int ip = wifiInfo.getIpAddress();
		final String ipAddr = ((ip >> 0) & 0xFF) + "." + ((ip >> 8) & 0xFF) + "." +
							  ((ip >> 16) & 0xFF) + "." + ((ip >> 24) & 0xFF);		
		return ipAddr;
	}

	public byte[]	getPacketSize() {
		final String packetSize = "0";
		return this.mByteArrays.intToByteArray(packetSize);
	}

	public byte[]	getHeader(byte[] source, byte[] dest, String dispatch, String opcode, int size) {
		return this.mByteArrays.concatByteArrays(source, dest, (dispatch + opcode).getBytes(),
										 this.mByteArrays.intToByteArray_le(size));
	}	
}
