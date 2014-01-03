/**
 * @file	JsonParser.java
 * @author	Sebastien GUILLERM
 * @brief	JSon functions.
 */

package com.mobii.utils;

import org.json.*;

import android.util.Log;

public class JsonParser {

	public static JSONObject	mJObj = null;
	public String				mJson = null;

	public JsonParser() {}
	public JsonParser(JsonParser jp) {}
/*
	public String	readJson(String data) {
		try {
			mJObj = new JSONObject(data);
		} catch (JSONException e) {
			Log.e("readJson", e.toString());
		}
		return mJObj.toString();
	}

	public String	writeJson(String action, String data) {
		try {
			mJObj.put(action, data);
		} catch (JSONException e) {
			Log.e("writeJson", e.toString());
		}
		return new JSONArray().put(mJObj).toString();
	}
*/

	/**
	 * @brief	Gets action.
	 * @details	Gets the field "action" of a JSon object.
	 * @param	data	A string.
	 * @return	A string which is the content of the "action" field the JSon object.
	 */
	public String	getAction(String data) {
		String action = "";
		try {
			action = new JSONObject(data).getString("action");
		} catch (JSONException e) {
			Log.e("getAction", e.toString());
		}
		return action;
	}
}
