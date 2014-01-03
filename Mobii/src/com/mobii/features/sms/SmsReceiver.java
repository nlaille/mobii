package com.mobii.features.sms;

import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Locale;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import com.mobii.MobiiConnectionActivity;
import com.mobii.connection.TcpClient;
import com.mobii.protocol.IProtocolCom;
import com.mobii.protocol.TcpPacket;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.telephony.SmsMessage;
import android.util.Log;
import android.widget.Toast;

public class SmsReceiver extends BroadcastReceiver {

	public SharedPreferences	mSharedMobii = MobiiConnectionActivity.getSharedPreferences();

	private String				mSms;

	@Override
    public void onReceive(Context context, Intent intent) {
		Date d = new Date();
		DateFormat df = new SimpleDateFormat("yyyy-MM-dd'T'hh:mm:ssz", new Locale("fr"));
        Bundle bundle = intent.getExtras();
        SmsMessage[] sms = null;
        String date = "", sender = "", content = "";

        if (bundle != null) {
        	Object[] pdus = (Object[]) bundle.get("pdus");
        	sms = new SmsMessage[pdus.length];
        	for (int i = 0; i < sms.length; i++) {
        		sms[i] = SmsMessage.createFromPdu((byte[])pdus[i]);

        		d.setTime(Long.valueOf(sms[i].getTimestampMillis()));
        		date = df.format(d);
                sender = sms[i].getDisplayOriginatingAddress().toString();
                content = sms[i].getMessageBody().toString();
        	}

        	Log.i("Sms date", ""+date);
        	Log.i("Sms content", content);
        	Log.i("Sms sender", sender);

    	    JSONObject jObj = new JSONObject(), jObjSms = null;
    		JSONArray jArray = new JSONArray();
    		try {
    			jObj.put("action", "sms");
    			jObjSms = new JSONObject();
    			jObjSms.put("sent_by_phone", false);
    			jObjSms.put("date", date);
    			jObjSms.put("sender", sender);
    			jObjSms.put("content", content);
    			jArray.put(jObjSms);
    			jObj.put("list", jArray);
    		} catch (JSONException e) {
    			Log.e("SmsReceiver : onReceive", e.toString());
    		}
    		this.mSms = jObj.toString();
    		Log.d("Sms Forward", this.mSms);

    		final class SmsForwardThread implements Runnable {
    			@Override
    			public void	run() {
    				TcpClient tcpClient = new TcpClient();
    				TcpPacket tcpPacket = new TcpPacket();
    				String src = MobiiConnectionActivity.getSharedPreferences().getString("user_id", "");
    				String dest = MobiiConnectionActivity.getSharedPreferences().getString("user_dest", "");
    				if (!dest.isEmpty()) {
    					byte[] packet = tcpPacket.getPacket(src.getBytes(),
    														dest.getBytes(),
    														IProtocolCom.d_com, IProtocolCom.o_jsonMsg, mSms);
    					tcpClient.sendPacket(packet);
    				}
    			}
    		}

    		// Forward the text message only if the app is connected to the server
    		//if (mSharedMobii.getBoolean("isConnected", false) == true) {
    			new Thread(new SmsForwardThread()).start();
    			this.mSms = null;

    			Toast toast = Toast.makeText(context, "Mobii : A message text has been received\nFrom : "+sender, Toast.LENGTH_LONG);
        		toast.show();
    		//}
        }
	}
}
