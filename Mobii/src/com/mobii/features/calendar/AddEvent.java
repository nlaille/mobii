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
import android.database.Cursor;
import android.net.Uri;
import android.os.RemoteException;
import android.provider.CalendarContract;
import android.util.Log;

public class AddEvent {

	public static Context	mCtx = MobiiConnectionActivity.getContextApplication();

	private ContentResolver	mCr = mCtx.getContentResolver();
	private Cursor			mC;
	private JSONObject		mJObj;
	private JSONArray		mJArray;

	public void	addEvent(String event) {
		try {
			this.mJArray = new JSONObject(event).getJSONArray("data");
			this.mJObj = this.mJArray.getJSONObject(0);
			String title = this.mJObj.getString("title");
			String location = this.mJObj.getString("where");
			String startdate = this.mJObj.getString("startdate");
			String enddate = this.mJObj.getString("enddate");
			String notes = this.mJObj.getString("notes");

			this.mC = this.mCr.query(Uri.parse("content://calendar/calendars"), null, null, null, null);
			if (this.mC.getCount() > 0)
				while (this.mC.moveToNext())
					if (!title.isEmpty() && !this.mC.getString(this.mC.getColumnIndex(CalendarContract.Events.TITLE)).isEmpty() &&
							title.compareTo(this.mC.getString(this.mC.getColumnIndex(CalendarContract.Events.TITLE))) == 0 &&
							!startdate.isEmpty() && !this.mC.getString(this.mC.getColumnIndex(CalendarContract.Events.DTSTART)).isEmpty() &&
							startdate.compareTo(this.mC.getString(this.mC.getColumnIndex(CalendarContract.Events.DTSTART))) == 0 &&
							!enddate.isEmpty() && !this.mC.getString(this.mC.getColumnIndex(CalendarContract.Events.DTEND)).isEmpty() &&
							enddate.compareTo(this.mC.getString(this.mC.getColumnIndex(CalendarContract.Events.DTEND))) == 0 &&
							!location.isEmpty() && !this.mC.getString(this.mC.getColumnIndex(CalendarContract.Events.EVENT_LOCATION)).isEmpty() &&
							location.compareTo(this.mC.getString(this.mC.getColumnIndex(CalendarContract.Events.EVENT_LOCATION))) == 0){
						Log.i("Event", "Already exists");
						return;
					}

			ArrayList<ContentProviderOperation> ops = new ArrayList<ContentProviderOperation>();
			int eventInsertIndex = ops.size();

			ops.add(ContentProviderOperation.newInsert(CalendarContract.Events.CONTENT_URI)
					.withValue(CalendarContract.Events.ACCOUNT_TYPE, null)
					.withValue(CalendarContract.Events.ACCOUNT_NAME, null).build());

			// Adds title
			if (title.isEmpty() == false)
				ops.add(ContentProviderOperation.newInsert(CalendarContract.Events.CONTENT_URI)
						.withValueBackReference(CalendarContract.Events._ID, eventInsertIndex)
						.withValue(CalendarContract.Events.TITLE, title)
						.build());

			// Adds location
			if (location.isEmpty() == false)
				ops.add(ContentProviderOperation.newInsert(CalendarContract.Events.CONTENT_URI)
						.withValueBackReference(CalendarContract.Events._ID, eventInsertIndex)
						.withValue(CalendarContract.Events.EVENT_LOCATION, location)
						.build());

			// Adds startdate
			if (startdate.isEmpty() == false) {
				ops.add(ContentProviderOperation.newInsert(CalendarContract.Events.CONTENT_URI)
						.withValueBackReference(CalendarContract.Events._ID, eventInsertIndex)
						.withValue(CalendarContract.Events.DTSTART, startdate)
						.build());
			}

			// Adds enddate
			if (enddate.isEmpty() == false) {
				ops.add(ContentProviderOperation.newInsert(CalendarContract.Events.CONTENT_URI)
						.withValueBackReference(CalendarContract.Events._ID, eventInsertIndex)
						.withValue(CalendarContract.Events.DTEND, enddate)
						.build());
			}

			// Adds note
			if (notes.isEmpty() == false)
				ops.add(ContentProviderOperation.newInsert(CalendarContract.Events.CONTENT_URI)
						.withValueBackReference(CalendarContract.Events._ID, eventInsertIndex)
						.withValue(CalendarContract.Events.DESCRIPTION, notes)
						.build());

			try {
            	this.mCr.applyBatch(CalendarContract.AUTHORITY, ops);
            }
            catch (RemoteException e) {
				Log.e("AddEvent", "RemoteException", e);
            }
            catch (OperationApplicationException e) {
            	Log.e("AddEvent", "OperationApplicationException", e);
            }
		} catch (JSONException e) {
			Log.e("AddEvent", "JSONException", e);
		}
	}
}
