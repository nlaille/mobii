package com.mobii.features.calendar;

import java.util.ArrayList;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import com.mobii.MobiiConnectionActivity;

import android.content.ContentProviderOperation;
import android.content.ContentResolver;
import android.content.Context;
import android.content.OperationApplicationException;
import android.os.RemoteException;
import android.provider.CalendarContract;
import android.util.Log;

public class DelEvent {

	public static Context	mCtx = MobiiConnectionActivity.getContextApplication();

	private ContentResolver	mCr = mCtx.getContentResolver();
	private JSONObject		mJObj;
	private JSONArray		mJArray;

	public void	delEvent(String event) {
		try {
			this.mJArray = new JSONObject(event).getJSONArray("data");
			this.mJObj = this.mJArray.getJSONObject(0);
			String id = String.valueOf(this.mJObj.getString("id"));

			ArrayList<ContentProviderOperation> ops = new ArrayList<ContentProviderOperation>();

			// Deletes event
			ops.add(ContentProviderOperation.newDelete(CalendarContract.Events.CONTENT_URI)
					.withSelection(CalendarContract.Events._ID + " = ? ", new String[] {id})
					.build());

			try {
				this.mCr.applyBatch(CalendarContract.AUTHORITY, ops);
			} catch (RemoteException e) {
				Log.e("DelEvent", "RemoteException", e);
			} catch (OperationApplicationException e) {
				Log.e("DelEvent", "OperationApplicationException", e);
			}
		} catch (JSONException e) {
			Log.e("DelEvent", "JSONException", e);
		}
	}
}
