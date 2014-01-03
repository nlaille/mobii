/**
 * @file	UptContact.java
 * @author	Sebastien GUILLERM
 * @brief	Update contact functions.
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

import com.mobii.MobiiConnectionActivity;

import android.content.ContentProviderOperation;
import android.content.ContentProviderOperation.Builder;
import android.content.ContentResolver;
import android.content.Context;
import android.content.OperationApplicationException;
import android.graphics.Bitmap;
import android.os.RemoteException;
import android.provider.ContactsContract;
import android.provider.ContactsContract.CommonDataKinds;
import android.provider.ContactsContract.Data;
import android.provider.ContactsContract.CommonDataKinds.Phone;
import android.util.Log;

public class UptContact {

	public static Context	mCtx = MobiiConnectionActivity.getContextApplication();

	private ContentResolver	mCr = mCtx.getContentResolver();
	private JSONObject		mJObj;
	private JSONArray		mJArray;

	/**
	 * @brief	Updates a contact.
	 * @details	Parses a JSon string to get information of a contact in order to update it.
	 * @param	contact A JSon string which contains contact information.
	 */
	public void		uptContact(String contact) {
		try {
			this.mJArray = new JSONObject(contact).getJSONArray("data");
			this.mJObj = this.mJArray.getJSONObject(0);
			String id = String.valueOf(this.mJObj.getString("id"));
			String name = this.mJObj.getString("firstname") + " " + this.mJObj.getString("lastname");
			String firstname = this.mJObj.getString("firstname");
			String lastname = this.mJObj.getString("lastname");
			String phoneList = this.mJObj.getString("phoneList");
			String emailList = this.mJObj.getString("mailList");
			String society = this.mJObj.getString("society");
			//String profilePic = this.mJObj.getString("profilePic");
			String birthday = this.mJObj.getString("birthday");
			String notes = this.mJObj.getString("notes");

			ArrayList<ContentProviderOperation> ops = new ArrayList<ContentProviderOperation>();

			// Updates name (firstname and lastname)
			if (name.isEmpty() == false) {
            	Builder builder = ContentProviderOperation.newUpdate(Data.CONTENT_URI);
                builder.withSelection(Data.CONTACT_ID + " = ?" + " AND " + Data.MIMETYPE + " = ?",
                					  new String[] {id, CommonDataKinds.StructuredName.CONTENT_ITEM_TYPE});
                builder.withValue(CommonDataKinds.StructuredName.GIVEN_NAME, firstname);
                builder.withValue(CommonDataKinds.StructuredName.FAMILY_NAME, lastname);
                ops.add(builder.build());
			}

			// Updates phone numbers
			if (phoneList.isEmpty() == false) {
            	JSONArray jArray = new JSONArray(phoneList);

            	for (int i = 0; i < jArray.length(); i++) {
            		JSONObject jObj = jArray.getJSONObject(i);
                	String phone = jObj.getString("value"),
                 		   label = jObj.getString("label");

                	Builder builder = ContentProviderOperation.newUpdate(Data.CONTENT_URI);
                	builder.withSelection(Data.CONTACT_ID + " = ?" + " AND " + Data.MIMETYPE + " = ?" + " AND " + CommonDataKinds.Phone.TYPE + " = ?",
                						  new String[] {id, CommonDataKinds.Phone.CONTENT_ITEM_TYPE, label});
                	builder.withValue(CommonDataKinds.Phone.NUMBER, phone);
                	builder.withValue(Phone.TYPE, Phone.TYPE_CUSTOM);
    				builder.withValue(Phone.LABEL, label);
                	ops.add(builder.build());
            	}
            }

			// Updates email addresses
			if (emailList.isEmpty() == false) {
            	JSONArray jArray = new JSONArray(emailList);

            	for (int i = 0; i < jArray.length(); i++) {
            		JSONObject jObj = jArray.getJSONObject(i);
                	String email = jObj.getString("value"),
                 		   label = jObj.getString("label");

                	Builder builder = ContentProviderOperation.newUpdate(Data.CONTENT_URI);
                	builder.withSelection(Data.CONTACT_ID + " = ?" + " AND " + Data.MIMETYPE + " = ?" + " AND " + CommonDataKinds.Email.TYPE + " = ?",
                						  new String[] {id, CommonDataKinds.Email.CONTENT_ITEM_TYPE, label});
                	builder.withValue(CommonDataKinds.Email.ADDRESS, email);
                	builder.withValue(CommonDataKinds.Email.TYPE, CommonDataKinds.Email.TYPE_CUSTOM);
    				builder.withValue(CommonDataKinds.Email.LABEL, label);
                	ops.add(builder.build());
            	}
            }

			// Updates society
			if (society.isEmpty() == false) {
			    Builder builder = ContentProviderOperation.newUpdate(Data.CONTENT_URI);
            	builder.withSelection(Data.CONTACT_ID + " = ?" + " AND " + Data.MIMETYPE + " = ?"/* + " AND " + CommonDataKinds.Organization.TYPE + " = ?"*/,
            						  new String[] {id, CommonDataKinds.Organization.CONTENT_ITEM_TYPE/*,
            						  				String.valueOf(CommonDataKinds.Organization.TYPE_WORK)*/});
            	builder.withValue(CommonDataKinds.Organization.DATA, society);
            	ops.add(builder.build());
            }

			// Updates profile picture
			/*if (profilePic.isEmpty() == false) {
            	try {
            		Bitmap bitmap = MediaStore.Images.Media.getBitmap(getContentResolver(), Uri.parse(photo_uri));
            		ByteArrayOutputStream image = new ByteArrayOutputStream();
            		bitmap.compress(Bitmap.CompressFormat.JPEG , 100, image);

            		Builder builder = ContentProviderOperation.newUpdate(Data.CONTENT_URI);
            		builder.withSelection(Data.CONTACT_ID + " = ?" + " AND " + Data.MIMETYPE + " = ?",
            							  new String[] {id, CommonDataKinds.Photo.CONTENT_ITEM_TYPE});
            		builder.withValue(CommonDataKinds.Photo.PHOTO, image.toByteArray());
            		ops.add(builder.build());
            	}
            	catch (Exception e) {
					Log.e("UptContact", "profilePic", e);
            	}
            }*/

			// Updates birthday
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
            	Builder builder = ContentProviderOperation.newUpdate(Data.CONTENT_URI);
            	builder.withSelection(Data.CONTACT_ID + " = ?" + " AND " + Data.MIMETYPE + " = ?" + " AND " + CommonDataKinds.Event.TYPE + " = ?",
            						  new String[] {id, CommonDataKinds.Event.CONTENT_ITEM_TYPE,
            										String.valueOf(CommonDataKinds.Event.TYPE_BIRTHDAY)});
            	builder.withValue(CommonDataKinds.Event.START_DATE, birthday);
            	ops.add(builder.build());
			}

			// Updates note
			if (notes.isEmpty() == false) {
				Builder builder = ContentProviderOperation.newUpdate(Data.CONTENT_URI);
            	builder.withSelection(Data.CONTACT_ID + " = ?" + " AND " + Data.MIMETYPE + " = ?",
            						  new String[] {id, CommonDataKinds.Note.CONTENT_ITEM_TYPE});
            	builder.withValue(CommonDataKinds.Note.NOTE, notes);
            	ops.add(builder.build());
            }

			try {
            	this.mCr.applyBatch(ContactsContract.AUTHORITY, ops);
			} catch (RemoteException e) { 
				Log.e("UptContact", "RemoteException", e);
            } catch (OperationApplicationException e) {
            	Log.e("UptContact", "OperationApplicationException", e);
            }
		} catch (JSONException e) {
			Log.e("Contacts : uptContact : JSONException", e.toString());
		}
	}
}
