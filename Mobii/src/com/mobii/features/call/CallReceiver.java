package com.mobii.features.call;

import com.mobii.MobiiConnectionActivity;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.telephony.PhoneStateListener;
import android.telephony.TelephonyManager;
import android.util.Log;
import android.widget.Toast;

public class CallReceiver extends BroadcastReceiver {
	@Override
	public void onReceive(Context ctx, Intent i) {
		try {
			TelephonyManager tmgr = (TelephonyManager) MobiiConnectionActivity.getContextApplication().getSystemService(Context.TELEPHONY_SERVICE);
             MyPhoneStateListener PhoneListener = new MyPhoneStateListener();
             tmgr.listen(PhoneListener, PhoneStateListener.LISTEN_CALL_STATE);
		} catch (Exception e) {
			Log.e("Phone Receive Error", " " + e);
		}
	}

	private class MyPhoneStateListener extends PhoneStateListener {
		public void onCallStateChanged(int state, String incomingNumber) {
			Log.d("MyPhoneListener",state+"   incoming no:"+incomingNumber);
			if (state == 1) {
				String msg = "New Phone Call Event. Incomming Number : "+incomingNumber;
				int duration = Toast.LENGTH_LONG;
				Toast toast = Toast.makeText(MobiiConnectionActivity.getContextApplication(), msg, duration);
				toast.show();
			}
		}
	}
}
