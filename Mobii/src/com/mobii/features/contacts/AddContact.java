/**
 * @file	AddContacs.java
 * @author	Sebastien GUILLERM
 * @brief	Add contact functions.
 */

package com.mobii.features.contacts;

import java.text.DateFormat;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.Locale;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import android.content.ContentProviderOperation;
import android.content.ContentResolver;
import android.content.Context;
import android.content.OperationApplicationException;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.os.RemoteException;
import android.provider.ContactsContract;
import android.provider.ContactsContract.CommonDataKinds;
import android.provider.ContactsContract.CommonDataKinds.Phone;
import android.provider.ContactsContract.CommonDataKinds.StructuredName;
import android.provider.ContactsContract.Data;
import android.provider.ContactsContract.RawContacts;
import android.util.Log;

import com.mobii.MobiiConnectionActivity;

public class AddContact {

	public static Context	mCtx = MobiiConnectionActivity.getContextApplication();

	private ContentResolver	mCr = mCtx.getContentResolver();
	private Cursor			mC;
	private JSONObject		mJObj;
	private JSONArray		mJArray;

	/**
	 * @brief	Adds a contact.
	 * @details	Parses a JSon string to get information of a contact in order to add it on the mobile.
	 * @param	contact A JSon string which contains contact information.
	 */
	public void		addContact(String contact) {
		try {
			this.mJArray = new JSONObject(contact).getJSONArray("data");
			this.mJObj = this.mJArray.getJSONObject(0);
			String name = this.mJObj.getString("firstname") + " " + this.mJObj.getString("lastname");
			String phoneList = this.mJObj.getString("phoneList");
			String emailList = this.mJObj.getString("mailList");
			String society = this.mJObj.getString("society");
			//String profilePic = this.mJObj.getString("profilePic");
			String birthday = this.mJObj.getString("birthday");
			String notes = this.mJObj.getString("notes");

			this.mC = this.mCr.query(Data.CONTENT_URI, null, null, null, null);
			if (this.mC.getCount() > 0)
				while (this.mC.moveToNext()) {
					/*if (!name.isEmpty() && !this.mC.getString(this.mC.getColumnIndex(Data.DISPLAY_NAME)).isEmpty() &&
							name.compareTo(this.mC.getString(this.mC.getColumnIndex(Data.DISPLAY_NAME))) == 0) {
						Log.i("Contact", "Already exists");
						return;
					}*/
				}

			ArrayList<ContentProviderOperation> ops = new ArrayList<ContentProviderOperation>();
            int rawContactInsertIndex = ops.size();

            ops.add(ContentProviderOperation.newInsert(RawContacts.CONTENT_URI)
                    .withValue(RawContacts.ACCOUNT_TYPE, null)
                    .withValue(RawContacts.ACCOUNT_NAME, null).build());

            // Adds name (firstname and lastname)
            if (name.isEmpty() == false)
            	ops.add(ContentProviderOperation.newInsert(Data.CONTENT_URI)
            			.withValueBackReference(Data.RAW_CONTACT_ID, rawContactInsertIndex)
            			.withValue(Data.MIMETYPE, StructuredName.CONTENT_ITEM_TYPE)
            			.withValue(StructuredName.DISPLAY_NAME, name)
            			.build());

            // Adds phone numbers
            if (phoneList.isEmpty() == false) {
            	JSONArray jArray = new JSONArray(phoneList);

            	for (int i = 0; i < jArray.length(); i++) {
            		JSONObject jObj = jArray.getJSONObject(i);
                	String phone = jObj.getString("value"),
                 		   label = jObj.getString("label");

                	ops.add(ContentProviderOperation.newInsert(Data.CONTENT_URI)
            				.withValueBackReference(Data.RAW_CONTACT_ID, rawContactInsertIndex)
            				.withValue(Data.MIMETYPE, Phone.CONTENT_ITEM_TYPE)
            				.withValue(Phone.NUMBER, phone)
            				.withValue(Phone.TYPE, Phone.TYPE_CUSTOM)
            				.withValue(Phone.LABEL, label)
            				.build());
            	}
            }

            // Adds email addresses
            if (emailList.isEmpty() == false) {
            	JSONArray jArray = new JSONArray(emailList);

            	for (int i = 0; i < jArray.length(); i++) {
            		JSONObject jObj = jArray.getJSONObject(i);
                	String email = jObj.getString("value"),
                 		   label = jObj.getString("label");

            		ops.add(ContentProviderOperation.newInsert(Data.CONTENT_URI)
            				.withValueBackReference(Data.RAW_CONTACT_ID, rawContactInsertIndex)
            				.withValue(Data.MIMETYPE, CommonDataKinds.Email.CONTENT_ITEM_TYPE)
            				.withValue(CommonDataKinds.Email.ADDRESS, email)
            				.withValue(CommonDataKinds.Email.TYPE, CommonDataKinds.Email.TYPE_CUSTOM)
            				.withValue(CommonDataKinds.Email.LABEL, label)
            				.build());
            	}
            }

            // Adds society
            if (society.isEmpty() == false)
            	ops.add(ContentProviderOperation.newInsert(Data.CONTENT_URI)
            			.withValueBackReference(Data.RAW_CONTACT_ID, rawContactInsertIndex)
            			.withValue(Data.MIMETYPE, CommonDataKinds.Organization.CONTENT_ITEM_TYPE)
            			.withValue(CommonDataKinds.Organization.COMPANY, society)
            			.withValue(CommonDataKinds.Organization.TYPE, CommonDataKinds.Organization.TYPE_WORK)
            			.build());

            // Adds profile picture
            /*if (profilePic.isEmpty() == false)
				ops.add(ContentProviderOperation.newInsert(Data.CONTENT_URI)
						.withValueBackReference(Data.RAW_CONTACT_ID, rawContactInsertIndex)
						.withValue(Data.MIMETYPE, CommonDataKinds.Photo.CONTENT_ITEM_TYPE)
						.withValue(CommonDataKinds.Photo.PHOTO, profilePic)
						.build());*/

            // Adds birthday
            if (birthday.isEmpty() == false) {
            	DateFormat df = new SimpleDateFormat("yyyy-MM-dd'T'HH:mm:ss", Locale.FRANCE);
            	DateFormat df2 = new SimpleDateFormat("yyyy-MM-dd", Locale.FRANCE);
            	Date date = new Date();

            	try {
					date = df.parse(this.mJObj.getString("birthday"));
					birthday = df2.format(date).toString();
				} catch (ParseException e) {
					Log.e("AddContact", "addContact", e);
				}

            	ops.add(ContentProviderOperation.newInsert(Data.CONTENT_URI)
            			.withValueBackReference(Data.RAW_CONTACT_ID, rawContactInsertIndex)
            			.withValue(Data.MIMETYPE, CommonDataKinds.Event.CONTENT_ITEM_TYPE)
            			.withValue(CommonDataKinds.Event.START_DATE, birthday)
            			.withValue(Data.MIMETYPE, CommonDataKinds.Event.CONTENT_ITEM_TYPE)
            			.withValue(CommonDataKinds.Event.TYPE, CommonDataKinds.Event.TYPE_BIRTHDAY)
            			.build());
            }

            // Adds note
            if (notes.isEmpty() == false)
            	ops.add(ContentProviderOperation.newInsert(Data.CONTENT_URI)
            			.withValueBackReference(Data.RAW_CONTACT_ID, rawContactInsertIndex)
            			.withValue(Data.MIMETYPE, CommonDataKinds.Note.CONTENT_ITEM_TYPE)
            			.withValue(CommonDataKinds.Note.NOTE, notes)
            			.build());

			try {
            	this.mCr.applyBatch(ContactsContract.AUTHORITY, ops);
            }
            catch (RemoteException e) { 
				Log.e("AddContact", "RemoteException", e);
            }
            catch (OperationApplicationException e) {
            	Log.e("AddContact", "OperationApplicationException", e);
            }
		} catch (JSONException e) {
			Log.e("AddContact", "JSONException", e);
		}
	}
}
