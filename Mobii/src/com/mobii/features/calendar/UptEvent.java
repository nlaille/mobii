package com.mobii.features.calendar;

import java.util.ArrayList;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import android.content.ContentProviderOperation;
import android.content.ContentResolver;
import android.content.Context;
import android.content.OperationApplicationException;
import android.content.ContentProviderOperation.Builder;
import android.os.RemoteException;
import android.provider.CalendarContract;
import android.util.Log;

import com.mobii.MobiiConnectionActivity;

public class UptEvent {

	public static Context	mCtx = MobiiConnectionActivity.getContextApplication();

	private ContentResolver	mCr = mCtx.getContentResolver();
	private JSONObject		mJObj;
	private JSONArray		mJArray;

	public void	uptEvent(String event) {
		try {
			this.mJArray = new JSONObject(event).getJSONArray("data");
			this.mJObj = this.mJArray.getJSONObject(0);
			String id = String.valueOf(this.mJObj.getString("id"));
			String title = this.mJObj.getString("title");
			String location = this.mJObj.getString("where");
			String startdate = this.mJObj.getString("startdate");
			String enddate = this.mJObj.getString("enddate");
			String notes = this.mJObj.getString("notes");

			ArrayList<ContentProviderOperation> ops = new ArrayList<ContentProviderOperation>();

			// Updates title
			if (title.isEmpty() == false) {
				Builder builder = ContentProviderOperation.newUpdate(CalendarContract.Events.CONTENT_URI);
				builder.withSelection(CalendarContract.Events._ID + " = ?",
									  new String[] {id});
				builder.withValue(CalendarContract.Events.TITLE, title);
				ops.add(builder.build());
			}

			// Updates location
			if (location.isEmpty() == false) {
				Builder builder = ContentProviderOperation.newUpdate(CalendarContract.Events.CONTENT_URI);
				builder.withSelection(CalendarContract.Events._ID + " = ?",
									  new String[] {id});
				builder.withValue(CalendarContract.Events.EVENT_LOCATION, location);
				ops.add(builder.build());
			}

			// Updates startdate
			if (startdate.isEmpty() == false) {
				Builder builder = ContentProviderOperation.newUpdate(CalendarContract.Events.CONTENT_URI);
				builder.withSelection(CalendarContract.Events._ID + " = ?",
									  new String[] {id});
				builder.withValue(CalendarContract.Events.DTSTART, startdate);
				ops.add(builder.build());
			}

			// Updates enddate
			if (enddate.isEmpty() == false) {
				Builder builder = ContentProviderOperation.newUpdate(CalendarContract.Events.CONTENT_URI);
				builder.withSelection(CalendarContract.Events._ID + " = ?",
									  new String[] {id});
				builder.withValue(CalendarContract.Events.DTEND, enddate);
				ops.add(builder.build());
			}

			// Updates notes
			if (notes.isEmpty() == false) {
				Builder builder = ContentProviderOperation.newUpdate(CalendarContract.Events.CONTENT_URI);
				builder.withSelection(CalendarContract.Events._ID + " = ?",
									  new String[] {id});
				builder.withValue(CalendarContract.Events.DESCRIPTION, notes);
				ops.add(builder.build());
			}

			try {
            	this.mCr.applyBatch(CalendarContract.AUTHORITY, ops);
            } catch (RemoteException e) {
				Log.e("UptEvent", "RemoteException", e);
            } catch (OperationApplicationException e) {
            	Log.e("UptEvent", "OperationApplicationException", e);
            }
		} catch (JSONException e) {
			Log.e("UptEvent", "JSONException", e);
		}
	}
}
