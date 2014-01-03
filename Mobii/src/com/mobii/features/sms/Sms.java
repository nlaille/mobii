/**
 * @file	Sms.java
 * @author	Sebastien GUILLERM
 * @brief	Manages sms.
 */

package com.mobii.features.sms;

import java.text.DateFormat;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.Locale;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import com.mobii.MobiiConnectionActivity;

import android.content.ContentResolver;
import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.net.Uri;
import android.telephony.SmsManager;
import android.util.Log;

public class Sms {

	private Context			mCtx;
	private JSONObject		mJObj, mJObjSms;
	private JSONArray		mJArray;
	private Uri				mSmsInbox, mSmsSent;
	private ContentResolver	mCr;
	private Cursor			mCInbox, mCSent;
	private DateFormat		mDf;
	private Date			mD, mDMin;
	private long			mDVal, mDMinVal;

	public Sms() {
		this.mCtx = MobiiConnectionActivity.getContextApplication();
		this.mJObj = new JSONObject();
		this.mJObjSms = null;
		this.mJArray = new JSONArray();
		this.mSmsInbox = Uri.parse("content://sms/inbox");
		this.mSmsSent = Uri.parse("content://sms/sent");
		this.mCr = this.mCtx.getContentResolver();
		this.mCInbox = this.mCr.query(mSmsInbox, null, null, null, null);
	    this.mCSent = this.mCr.query(mSmsSent, null, null, null, null);
	    this.mDf = new SimpleDateFormat("yyyy-MM-dd'T'HH:mm:ss", Locale.FRANCE);
	    this.mD = new Date();
	    this.mDMin = new Date();
		this.mDVal = 0;
		this.mDMinVal = 0;
	}

	public Sms(Sms sms) {
		this.mCtx = sms.mCtx;
		this.mJObj = sms.mJObj;
		this.mJObjSms = sms.mJObjSms;
		this.mJArray = sms.mJArray;
		this.mSmsInbox = sms.mSmsInbox;
		this.mSmsSent = sms.mSmsSent;
		this.mCr = sms.mCr;
		this.mCInbox = sms.mCInbox;
	    this.mCSent = sms.mCSent;
	    this.mDf = sms.mDf;
	    this.mD = sms.mD;
	    this.mDMin = sms.mDMin;
	    this.mDVal = sms.mDVal;
	    this.mDMinVal = sms.mDMinVal;
	}

	/**
	 * @brief	Gets sms synchronization date.
	 * @details	Gets the field corresponding to the date of the last text message synchronized.
	 * @param	msg	A text message JSon string.
	 */
	private void	smsDate(String msg) {
		try {
			String dateMin = new JSONObject(msg).getString("from");
			if (dateMin.isEmpty() == false) {
				this.mDMin = this.mDf.parse(dateMin);
				this.mDMinVal = this.mDMin.getTime();
			}
		} catch (ParseException e) {
			Log.e("Sms", "smsDate", e);
		} catch (JSONException e) {
			Log.e("Sms", "smsDate", e);
		}
	}

	/**
	 * @brief	Gets all text messages.
	 * @details	Gets all text messages and add it to the JSon object if they are not yet synchronized.
	 * @param	sms	The cursor corresponding to the inbox sms path or to the outbox sms path.
	 * @param	sentByPhone	A boolean allowing the client to know if the text message has been sent or received by the phone.
	 */
	private void	getAllSms(Cursor sms, boolean sentByPhone) {
		Log.i("Sms", "getSmsInbox");

		try {
			while (sms.moveToNext()) {
				this.mDVal = 0;
    			this.mJObjSms = new JSONObject();
    			this.mJObjSms.put("sent_by_phone", sentByPhone);
		        for (int i = 0; i < sms.getColumnCount(); i++) {
		        	if (sms.getColumnName(i).equals("date")) {
		        		this.mD.setTime(Long.valueOf(sms.getString(i)));
		        		this.mDVal = this.mD.getTime();
		        		this.mJObjSms.put("date", this.mDf.format(this.mD));
		        	}
		        	if (sms.getColumnName(i).equals("address"))
		        		this.mJObjSms.put("sender", sms.getString(i));
		        	if (sms.getColumnName(i).equals("body"))
		        		this.mJObjSms.put("content", sms.getString(i));
		        }

		        /*
		         * Add to JSon only if date of text message is after the last text messages synchronization date.
		         * 
		         * +1000 : Add one second to the last synchronisation date.
		         */
		        if (this.mDVal > this.mDMinVal+1000) {
			        Log.i(this.mDMin.toString(), this.mD.toString());
			        Log.i(String.valueOf(this.mDVal), String.valueOf(this.mDMinVal));
			        this.mJArray.put(this.mJObjSms);
		        }
			}
		} catch (JSONException e) {
			Log.e("Sms : sms : JSONSException", e.toString());
		}
	}

	/**
	 * @brief	Gets text messages.
	 * @details	Calls getAllSms function.
	 * @param	msg	A text message JSon string.
	 * @return	Returns all text messages not yet synchronized.
	 */
	public String	getSms(String msg) {
		Log.i("Sms", "getSms");

		try {
			smsDate(msg);
			this.mJObj.put("action", "sms");
			getAllSms(this.mCInbox, false);
			getAllSms(this.mCSent, true);
			this.mJObj.put("list", this.mJArray);
		} catch (JSONException e) {
			Log.e("Sms : getSms : JSONException", e.toString());
		}
		return this.mJObj.toString();
	}

	/**
	 * @brief	Gets and sends a sms.
	 * @details	Gets the text message <b>msg</b> and sends it to the phone number <b>phoneNumber</b>.
	 * @param	phoneNumber	The phone number of the destination.
	 * @param	msg	The message to send.
	 */
	public void		sendSms(String phoneNumber, String msg) {
		SmsManager sms = SmsManager.getDefault();
		ArrayList<String>smsList = sms.divideMessage(msg);
		sms.sendMultipartTextMessage(phoneNumber, null, smsList, null, null);

		ContentValues sms_values = new ContentValues();
		long date = System.currentTimeMillis();
		sms_values.put("date", date);
        sms_values.put("address", phoneNumber);
        sms_values.put("body", msg);
        this.mCtx.getContentResolver().insert(Uri.parse("content://sms/inbox"), sms_values);
	}

	/**
	 * @brief	Sms forwarding.
	 * @details	Forwards a text message to the client when it has been received.
	 * @param	date	The date when the text message has been received.
	 * @param	content	The body of the text message.
	 * @param	sender	The phone number of the sender.
	 * @return	Returns a JSon string if it is successful created; otherwise <b>null</b> is returned.
	 */
	public String	forwardSms(String date, String content, String sender) {
		Log.i("Sms", "forwardSms");

	    JSONObject jObj = new JSONObject(), jObjSms = null;
		JSONArray jArray = new JSONArray();
		try {
			jObj.put("action", "sms");
			jObjSms = new JSONObject();
			jObjSms.put("sent_by_phone", false);
			jObjSms.put("date", date);
			jObjSms.put("sender", sender);
			jObjSms.put("content", content);
			Log.d("jObjSms", jObjSms.toString());
			jArray.put(jObjSms);
			jObj.put("list", jArray);
			return jObj.toString();
		} catch (JSONException e) {
			Log.e("getAllReceivedSms", e.toString());
		}
		return null;
	}
}
