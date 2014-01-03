/**
 * @file	MobileInfos.java
 * @author	Sebastien GUILLERM
 * @brief	Get mobile information.
 */

package com.mobii.features.infos;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import android.content.Context;
import android.os.Build;
import android.telephony.TelephonyManager;
import android.util.Log;

public class MobileInfos {

	public Context			mCtx;
	public TelephonyManager	mTm;

	public MobileInfos(Context ctx) {
		this.mCtx = ctx;
		this.mTm = (TelephonyManager) this.mCtx.getSystemService(Context.TELEPHONY_SERVICE);
	}

	public MobileInfos(MobileInfos ti) {
		this.mCtx = ti.mCtx;
		this.mTm = ti.mTm;
	}

	public String getBoard() {
		return Build.BOARD;
	}

	public String getBootloader() {
		return Build.BOOTLOADER;
	}

	public String	getBrand() {
		return Build.BRAND;
	}

	public String	getDevice() {
		return Build.DEVICE;
	}

	public String	getHardware() {
		return Build.HARDWARE;
	}

	public String	getManufacturer() {
		return Build.MANUFACTURER;
	}

	public String	getModel() {
		return Build.MODEL;
	}

	public String	getProduct() {
		return Build.PRODUCT;
	}

	public String	getImei() {
    	return this.mTm.getDeviceId();
	}

    public String	getSoftwareVersion() {
    	return this.mTm.getDeviceSoftwareVersion();
    }

    public String	getLine1Number() {
    	return this.mTm.getLine1Number();
    }

    public String	getNetworkCountryIso() {
    	return this.mTm.getNetworkCountryIso();    	
    }

    public String	getNetworkOperator() {
    	return this.mTm.getNetworkOperator();
    }

    public String	getNetworkOperatorName() {
    	return this.mTm.getNetworkOperatorName();
    }

    public int		getNetworkType() {
    	return this.mTm.getNetworkType();
    }

    public int		getPhoneType() {
    	return this.mTm.getPhoneType();
    }

    public String	getSimCountryIso() {
    	return this.mTm.getSimCountryIso();
    }

    public String	getSimOperator() {
    	return this.mTm.getSimOperator();
    }

    public String	getSimOperatorName() {
    	return this.mTm.getSimOperatorName();
    }

    public String	getSimSerialName() {
    	return this.mTm.getSimSerialNumber();
    }

    public int		getSimState() {
    	return this.mTm.getSimState();
    }

    public String	getSubscriberId() {
    	return this.mTm.getSubscriberId();
    }

    public String	getVoiceMailNumber() {
    	return this.mTm.getVoiceMailNumber();
    }

    public int		getConfigMcc() {
    	return this.mCtx.getResources().getConfiguration().mcc;
    }
    public int		getConfigMnc() {
    	return this.mCtx.getResources().getConfiguration().mnc;
	}

    public String getVersionName() {
    	String versionName = "";
    	try {
    		versionName = this.mCtx.getPackageManager().getPackageInfo(this.mCtx.getPackageName(),0).versionName;
    	} catch (Exception e) {
    		Log.e("Version Name", e.toString());
    	}
    	return versionName;
    }

    public int 	getVersionCode() {
    	int versionCode = 0;
    	try {
    		versionCode = this.mCtx.getPackageManager().getPackageInfo(this.mCtx.getPackageName(), 0).versionCode;
    	} catch (Exception e) {
    		Log.e("Version Code", e.toString());
    	}
    	return versionCode;
    }

	/**
	 * @brief	Gets mobile information.
	 * @details	Gets mobile information and put it in a string.
	 * @return	Returns a JSon string containing mobile information.
	 */
    public String	getMobileInfosJson() {
    	JSONObject	jObj = new JSONObject(), jObjArgs = new JSONObject();

    	try {
    		jObj.put("action", "mobile_infos");
    		//jObjArgs.put("board", getBoard());
    		//jObjArgs.put("bootloader", getBootloader());
    		jObjArgs.put("brand", getBrand());
    		//jObjArgs.put("device", getDevice());
    		//jObjArgs.put("hardware", getHardware());
    		jObjArgs.put("manufacturer", getManufacturer());
    		jObjArgs.put("model", getModel());
    		//jObjArgs.put("product", getProduct());
    		jObjArgs.put("imei", getImei());
    		//jObjArgs.put("software version", getSoftwareVersion());
    		//jObjArgs.put("line1 number", getLine1Number());
    		//jObjArgs.put("network country iso", getNetworkCountryIso());
    		//jObjArgs.put("network operator", getNetworkOperator());
    		//jObjArgs.put("network operator name", getNetworkOperatorName());
    		//jObjArgs.put("network type", getNetworkType());
    		//jObjArgs.put("phone type", getPhoneType());
    		jObjArgs.put("sim country iso", getSimCountryIso());
    		//jObjArgs.put("sim operator", getSimOperator());
    		jObjArgs.put("sim operator name", getSimOperatorName());
    		//jObjArgs.put("sim serial name", getSimSerialName());
    		//jObjArgs.put("sim state", getSimState());
    		//jObjArgs.put("subscriber id", getSubscriberId());
    		jObjArgs.put("voice mail number", getVoiceMailNumber());
    		//jObjArgs.put("config mcc", getConfigMcc());
    		//jObjArgs.put("config mnc", getConfigMnc());

    		//jObjArgs.put("version name", getVersionName());
    		//jObjArgs.put("version code", getVersionCode());

    		/*
    		 * Add available features
    		 */
    		jObj.accumulate("args", jObjArgs);
    		JSONArray jArrayFeat = new JSONArray();
    		jArrayFeat.put("sms");
    		jArrayFeat.put("contacts");
    		jArrayFeat.put("calendar");
    		//jArrayFeat.put("files");
    		jObj.accumulate("features", jArrayFeat);

    		} catch (JSONException e) {
    			Log.e("MobileInfos JSon", e.toString());
    		}
    	Log.i("Mobiles infos", jObj.toString());
    	return jObj.toString();
    }
}
