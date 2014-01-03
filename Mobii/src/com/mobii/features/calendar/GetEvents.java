package com.mobii.features.calendar;

import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Locale;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import com.mobii.MobiiConnectionActivity;

import android.content.ContentResolver;
import android.content.Context;
import android.database.Cursor;
import android.provider.CalendarContract;
import android.util.Log;

public class GetEvents {

	public Context			mCtx = MobiiConnectionActivity.getContextApplication();

	private ContentResolver	mCr = mCtx.getContentResolver();
	private Cursor			mC;
	private JSONObject		mJObj, mJObjData;
	private JSONArray		mJArray;
	private String			mId;

	public String	getEvents() {
		Log.i("Events", "getEvents");

		this.mJObj = new JSONObject();
		this.mJObjData = null;
		this.mJArray = new JSONArray();
		this.mC = this.mCr.query(CalendarContract.Events.CONTENT_URI, null, null, null, null);
		try {
			this.mJObj.put("action", "calendar");
			if (this.mC.getCount() > 0) {
				while (this.mC.moveToNext()) {
					this.mJObjData = new JSONObject();
					getId();
					getTitle();
					getLocation();
					getStartdate();
					getEnddate();
					//getNotes();
					this.mJArray.put(this.mJObjData);
				}
			}
			this.mJObj.put("data", this.mJArray);
		} catch (JSONException e) {
			Log.e("GetContacts", "getContacts", e);
		}
		this.mC.close();
		Log.i("contacts", this.mJObj.toString());
		return this.mJObj.toString();
	}

	public void		getId() {
		this.mId = "";

		try {
			this.mId = this.mC.getString(this.mC.getColumnIndex(CalendarContract.Events._ID));
			this.mJObjData.put("id", Integer.valueOf(this.mId));
		} catch (JSONException e) {
			Log.e("GetContacts", "getId", e);
		}
	}

	public void		getTitle() {
		String title = "";
		try {
			try {
				title = this.mC.getString(this.mC.getColumnIndex(CalendarContract.Events.TITLE));
			} catch (Exception e) {
				Log.e("GetEvents", "getTitle", e);
			}
			this.mJObjData.put("title", title);
		} catch (JSONException e) {
			Log.e("GetEvents", "getTitle", e);
		}
	}

	public void		getLocation() {
		String location = "";

		try {
			try {
				location = this.mC.getString(this.mC.getColumnIndex(CalendarContract.Events.EVENT_LOCATION));
			} catch (Exception e) {
				Log.e("GetEvents", "getLocation", e);
			}
			this.mJObjData.put("location", location);
		} catch (JSONException e) {
			Log.e("GetContacts", "getId", e);
		}
	}

	public void 	getStartdate() {
		DateFormat df = new SimpleDateFormat("yyyy-MM-dd'T'HH:mm:ss", Locale.FRANCE);
		Date d = new Date();
		String startdate = "", startVal = "";

		try {
			try {
				startVal = this.mC.getString(this.mC.getColumnIndex(CalendarContract.Events.DTEND));
				d.setTime(startVal != null ? Long.valueOf(startVal) : 0);
				startdate = df.format(d);
			} catch (Exception e) {
				Log.e("GetEvents", "getStartdate", e);
			}
			this.mJObjData.put("startdate", startdate);
		} catch (JSONException e) {
			Log.e("GetEvents", "getStartdate", e);
		}
	}

	public void		getEnddate() {
		DateFormat df = new SimpleDateFormat("yyyy-MM-dd'T'HH:mm:ss", Locale.FRANCE);
		Date d = new Date();
		String enddate = "", endVal = "";

		try {
			try {
				endVal = this.mC.getString(this.mC.getColumnIndex(CalendarContract.Events.DTEND));
				d.setTime(Long.valueOf(endVal != null ? Long.valueOf(endVal) : 0));
				enddate = df.format(d);
			} catch (Exception e) {
				Log.e("GetEvents", "getEnddate", e);
			}
			this.mJObjData.put("enddate", enddate);
		} catch (JSONException e) {
			Log.e("GetEvents", "getEnddate", e);
		}
	}

	public void		getNotes() {
		Cursor cNotes = this.mCr.query(CalendarContract.Events.CONTENT_URI, null,
				   CalendarContract.Events._ID + " = ?",
				   new String[] {this.mId}, null);
		String notes = "";

		try {
			if (cNotes.moveToFirst())
				notes = cNotes.getString(cNotes.getColumnIndex(CalendarContract.Events.DESCRIPTION));
			this.mJObjData.put("notes", notes);
		} catch (JSONException e) {
			Log.e("GetEvents", "getNotes", e);
		}
		cNotes.close();
	}
}
