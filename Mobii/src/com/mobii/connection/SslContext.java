/**
 * @file	SslContext.java
 * @author	Sebastien GUILLERM
 * @brief	Gets SSL socket.
 */

package com.mobii.connection;

import java.io.IOException;
import java.net.InetAddress;
import java.net.Socket;
import java.net.UnknownHostException;
import java.security.KeyManagementException;
import java.security.NoSuchAlgorithmException;
import java.security.cert.CertificateException;
import java.security.cert.X509Certificate;

import javax.net.ssl.SSLContext;
import javax.net.ssl.TrustManager;
import javax.net.ssl.X509TrustManager;

import android.util.Log;

public class SslContext {

	private Socket		mSocket;
	private InetAddress	mAddr;
	private String		mIp;
	private int			mPort;

	public SslContext(String ip, int port) {
		this.mSocket = null;
		this.mIp = ip;
		this.mAddr = null;
		this.mPort = port;

		try {
			this.mAddr = InetAddress.getByName(this.mIp);
		} catch (UnknownHostException e) {
			//e.printStackTrace();
			Log.i("SslContext: Constructor", e.toString());
		}
	}

	public SslContext(InetAddress addr, int port) {
		this.mSocket = null;
		this.mIp = null;
		this.mAddr = addr;
		this.mPort = port;
	}

	public SslContext(SslContext ctx) {
		this.mSocket = ctx.mSocket;
		this.mIp = ctx.mIp;
		this.mAddr = ctx.mAddr;
		this.mPort = ctx.mPort;
	}

	/**
	 * @brief	Gets SSL socket.
	 * @details	Create a SSL context and a SSL socket.
	 * @return	Returns the SSL socket if it was successful created; otherwise null.
	 */
	public Socket	getSslSocket() {
		TrustManager[] tm = new TrustManager[] {
				new X509TrustManager() {
					public X509Certificate[] getAcceptedIssuers() {
						return new X509Certificate[] {};
					}
					public void checkClientTrusted(X509Certificate[] chain, String authType)
							throws CertificateException {}
					public void checkServerTrusted(X509Certificate[] chain, String authType)
							throws CertificateException {}
				}
		};

		try {
			SSLContext sslCtx = SSLContext.getInstance("TLS");
			try {
				sslCtx.init(null, tm, null);
			} catch (KeyManagementException e) {
				//e.printStackTrace();
				Log.e("KeyManagerException sslConnect", e.toString());
			}
			try {
				this.mSocket = sslCtx.getSocketFactory().createSocket(this.mAddr, this.mPort);
			} catch (UnknownHostException e) {
				//e.printStackTrace();
				Log.e("UnknownHostException sslConnect", e.toString());
			} catch (IOException e) {
				//e.printStackTrace();
				Log.e("IOException sslConnect", e.toString());
			}
		} catch (NoSuchAlgorithmException e) {
			//e.printStackTrace();
			Log.e("NoSuchAlgorithmException sslConnect", e.toString());
		}
		return this.mSocket;
	}
}
