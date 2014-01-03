/**
 * @file	Wifi.java
 * @author	Sebastien GUILLERM
 * @brief	Check Wifi connection.
 */

package com.mobii.connection;

import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.SharedPreferences;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;

public class Wifi {

	private Context				ctx_;
	private SharedPreferences	settings_;
	private WifiManager			wifiManager_;
	private WifiInfo			wifiInfo_;
	private boolean				settingWifi_;

	public	Wifi(Context ctx, SharedPreferences settings) {
		this.ctx_ = ctx;
		this.settings_ = settings;
		this.wifiManager_ = (WifiManager) this.ctx_.getSystemService(Context.WIFI_SERVICE);
		this.wifiInfo_ = this.wifiManager_.getConnectionInfo();
		this.settingWifi_ = this.settings_.getBoolean("option_wifi", false);
	}

	public	Wifi(Wifi n) {
		this.ctx_ = n.ctx_;
		this.settings_ = n.settings_;
		this.wifiManager_ = n.wifiManager_;
		this.wifiInfo_ = n.wifiInfo_;
		this.settingWifi_ = n.settingWifi_;
	}

	/**
	 * @brief	Checks Wi-Fi connection if "Use only Wi-Fi connections" setting is checked.
	 * @param	boot	A boolean that is true if the application is launched on startup; otherwise false.
	 * @return	Returns true if "Use only Wi-Fi connections" setting is checked but Wi-Fi is disabled; otherwise returns false.
	 */
	public boolean	checkWifiOnly(boolean boot) {
		if (this.settingWifi_ && !checkWifi()) {
        	if (!boot) {
        		AlertDialog.Builder requestWifi = new AlertDialog.Builder(this.ctx_);
        		requestWifi.setTitle("Wi-Fi Only Setting");
        		requestWifi
        			.setMessage("Wi-Fi connection disabled.\nTo connect, please enable Wi-Fi or disable settings > \"Allow connections only when Wi-Fi is enabled\".")
        			.setCancelable(false)
        			.setPositiveButton("Ok", new DialogInterface.OnClickListener() {
        				public void onClick(DialogInterface dialog, int id) {
        				}
        			});
        		AlertDialog alertWifi = requestWifi.create();
        		alertWifi.show();
        	}
        	return true;
        }
        return false;
	}

	/**
	 * @brief	Checks if Wi-Fi is enabled and connected.
	 * @return	Returns true if Wi-Fi is enabled and connected; otherwise false.
	 */
	public boolean	checkWifi() {
		return (this.wifiManager_.getWifiState() == WifiManager.WIFI_STATE_ENABLED && this.wifiInfo_.getSSID() != null) ? true : false;
	}
}
