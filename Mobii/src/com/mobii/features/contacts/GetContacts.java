/**
 * @file	GetContacts.java
 * @author	Sebastien GUILLERM
 * @brief	Get contacts functions.
 */

package com.mobii.features.contacts;

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Locale;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import android.content.ContentResolver;
import android.content.Context;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.provider.ContactsContract;
import android.provider.ContactsContract.CommonDataKinds.Photo;
import android.util.Log;

import com.mobii.MobiiConnectionActivity;

public class GetContacts {

	public Context			mCtx = MobiiConnectionActivity.getContextApplication();

	private ContentResolver	mCr = mCtx.getContentResolver();
	private Cursor			mC;
	private JSONObject		mJObj, mJObjData;
	private JSONArray		mJArray;
	private String			mId, mName;

	/**
	 * @brief	Gets the list of the contacts.
	 * @return	Returns a JSon string which contains the list of the contacts.
	 */
	public String 	getContacts() {
		Log.i("Contacts", "getContacts");

		this.mJObj = new JSONObject();
		this.mJObjData = null;
		this.mJArray = new JSONArray();
		this.mC = mCr.query(ContactsContract.Contacts.CONTENT_URI, null, null, null, null);
		try {
			this.mJObj.put("action", "contacts");
			if (this.mC.getCount() > 0) {
				while (this.mC.moveToNext()) {
					this.mJObjData = new JSONObject();
					getId();
					getName();
					getPhone();
					getEmail();
					getSociety();
					//getProfilePic();
					getBirthday();
					getNotes();

					//for (int i = 0; i < this.jArray_.length(); i++)
						//if (this.jObjData_.equals(this.jArray_.get(i)) == false)
							//this.jArray_.put(this.jObjData_);
					if ((this.mId.isEmpty() && this.mName.isEmpty()) == false)
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

	/**
	 * @brief	Gets the id of a contact.
	 */
	public void		getId() {
		this.mId = "";

		try {
			this.mId = this.mC.getString(this.mC.getColumnIndex(ContactsContract.Contacts._ID));
			this.mJObjData.put("id", Integer.valueOf(this.mId));
		} catch (JSONException e) {
			Log.e("GetContacts", "getId", e);
		}
	}

	/**
	 * @brief	Gets the firstname and the lastname of a contact.
	 */
	private void	getName() {
		String firstname = "", lastname = "";

		try {
			if ((this.mName = this.mC.getString(this.mC.getColumnIndex(ContactsContract.RawContacts.DISPLAY_NAME_PRIMARY))) != null) {
				String[] name = this.mName.split(" ");
				firstname = name[0];
				if (name.length > 1 && name[1].isEmpty() == false)
					lastname = name[1];
			}
			this.mJObjData.put("firstname", firstname);
			this.mJObjData.put("lastname", lastname);
		} catch (JSONException e) {
			Log.e("GetContacts", "getName", e);
		}
	}

	/**
	 * @brief	Gets the phone numbers of a contact.
	 */
	private void	getPhone() {
		JSONArray jArray;
		JSONObject jObj;
		Cursor cPhone = this.mCr.query(ContactsContract.CommonDataKinds.Phone.CONTENT_URI, null,
									   ContactsContract.CommonDataKinds.Phone.CONTACT_ID + "= ?",
									   new String[]{this.mId}, null);

		try {
			jArray = new JSONArray();
			while (cPhone.moveToNext()) {
				jObj = new JSONObject();
				String phone = "", label = "", customLabel = "";
				int type = 0;

				try {
					phone = cPhone.getString(cPhone.getColumnIndex(ContactsContract.CommonDataKinds.Phone.NUMBER));
					type = cPhone.getInt(cPhone.getColumnIndex(ContactsContract.CommonDataKinds.Phone.TYPE));
					customLabel = cPhone.getString(cPhone.getColumnIndex(ContactsContract.CommonDataKinds.Phone.LABEL));
					label = (String) ContactsContract.CommonDataKinds.Phone.getTypeLabel(this.mCtx.getResources(), type, customLabel);
					if (phone.isEmpty() == false) {
						jObj.put("label", label);
						jObj.put("value", phone);
					}
				} catch (Exception e) {
					Log.e("Contacts : getPhone", e.toString());
				}
				jArray.put(jObj);
			}
			this.mJObjData.put("phoneList", jArray);
		} catch (JSONException e) {
			Log.e("GetContacts", "getPhone", e);
		}
		cPhone.close();
	}

	/**
	 * @brief	Gets the email addresses of a contact.
	 */
	private void	getEmail() {
		JSONArray jArray;
		JSONObject jObj;
		Cursor cEmail = this.mCr.query(ContactsContract.CommonDataKinds.Email.CONTENT_URI, null,
										ContactsContract.CommonDataKinds.Email.CONTACT_ID + " = ?",
										new String[] {this.mId}, null);

		try {
			jArray = new JSONArray();
			while (cEmail.moveToNext()) {
				jObj = new JSONObject();
				String email = "", label = "", customLabel = "";
				int type = 0;

				try {
					email = cEmail.getString(cEmail.getColumnIndex(ContactsContract.CommonDataKinds.Email.DATA));
					type = cEmail.getInt(cEmail.getColumnIndex(ContactsContract.CommonDataKinds.Email.TYPE));
					customLabel = cEmail.getString(cEmail.getColumnIndex(ContactsContract.CommonDataKinds.Email.LABEL));
					label = (String) ContactsContract.CommonDataKinds.Email.getTypeLabel(this.mCtx.getResources(), type, customLabel);
					if (email.isEmpty() == false) {
						jObj.put("label", label);
						jObj.put("value", email);
					}
				} catch (Exception e) {
					Log.e("Contacts : getEmail", e.toString());
				}
				jArray.put(jObj);
			}
			this.mJObjData.put("mailList", jArray);
		} catch (JSONException e) {
			Log.e("GetContacts", "getEmail", e);
		}
		cEmail.close();
	}

	/**
	 * @brief	Gets the name of the company where a contact works.
	 */
	private void	getSociety() {
		Cursor cSociety = this.mCr.query(ContactsContract.Data.CONTENT_URI, null,
										 ContactsContract.Data.CONTACT_ID + " = ?" + " AND " + ContactsContract.Data.MIMETYPE + " = ?",
										 new String[] {this.mId, ContactsContract.CommonDataKinds.Organization.CONTENT_ITEM_TYPE}, null);
		String society = "";

		try {
			if (cSociety.moveToFirst())
				society = cSociety.getString(cSociety.getColumnIndex(ContactsContract.CommonDataKinds.Organization.DATA));
			this.mJObjData.put("society", society);
		} catch (JSONException e) {
			Log.e("GetContacts", "getSociety", e);
		}
		cSociety.close();
	}

	/**
	 * @brief	Gets the profile picture of a contact.
	 */
	private void	getProfilePic() {
		Uri uriPhoto = ContactsContract.Data.CONTENT_URI;
		String[] projectionPhoto = new String[] { ContactsContract.CommonDataKinds.Photo.PHOTO };
		String selectionPhoto = ContactsContract.Contacts._ID + " = ?" + " AND " +
									 ContactsContract.CommonDataKinds.Photo.IN_VISIBLE_GROUP;
		String[] selectionArgsPhoto = new String[] {this.mId};
		Cursor cPhoto = this.mCr.query(uriPhoto, projectionPhoto, selectionPhoto, selectionArgsPhoto, null);
		Bitmap profilePic = null;

		try {
			if (cPhoto.moveToFirst()) {
				byte[] photo = cPhoto.getBlob(cPhoto.getColumnIndex(Photo.PHOTO));
				profilePic = BitmapFactory.decodeByteArray(photo, 0, photo.length);
			}
			this.mJObjData.put("profilePic", profilePic);
		} catch (JSONException e) {
			Log.e("GetContacts", "getProfilePic", e);
		}
		cPhoto.close();
	}

	/**
	 * @brief	Gets the birthday of a contact.
	 */
	private void	getBirthday() {
		Cursor cBirthday = this.mCr.query(ContactsContract.Data.CONTENT_URI, null,
										  ContactsContract.Data.CONTACT_ID + " = ?" + " AND " + ContactsContract.Data.MIMETYPE + " = ?",
										  new String[] {this.mId, ContactsContract.CommonDataKinds.Event.CONTENT_ITEM_TYPE}, null);
		SimpleDateFormat df = new SimpleDateFormat("yyyy-MM-dd", Locale.FRANCE);
		Date d = new Date();
		String birthday = "";

		try {
			if (cBirthday.moveToFirst())
				try {
					d = df.parse(cBirthday.getString(cBirthday.getColumnIndex(ContactsContract.CommonDataKinds.Event.START_DATE)));
					birthday = df.format(d).toString();
				} catch (ParseException e) {
					Log.e("GetContacts", "getBirthday", e);
				}
			this.mJObjData.put("birthday", birthday);
		} catch (JSONException e) {
			Log.e("GetContacts", "getBirthday", e);
		}
		cBirthday.close();
	}

	/**
	 * @brief	Gets the notes of a contact.
	 */
	private void	getNotes() {
		Cursor cNotes = this.mCr.query(ContactsContract.Data.CONTENT_URI, null,
									   ContactsContract.Data.CONTACT_ID + " = ?" + " AND " + ContactsContract.Data.MIMETYPE + " = ?",
									   new String[] {this.mId, ContactsContract.CommonDataKinds.Note.CONTENT_ITEM_TYPE}, null);
		String notes = "";

		try {
			if (cNotes.moveToFirst())
				notes = cNotes.getString(cNotes.getColumnIndex(ContactsContract.CommonDataKinds.Note.NOTE));
			this.mJObjData.put("notes", notes);
		} catch (JSONException e) {
			Log.e("GetContacts", "getNotes", e);
		}
		cNotes.close();
	}
}
