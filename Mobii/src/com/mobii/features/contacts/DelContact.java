/**
 * @file	DelContact.java
 * @author	Sebastien GUILLERM
 * @brief	Delete contact functions.
 */

package com.mobii.features.contacts;

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
import android.provider.ContactsContract;
import android.provider.ContactsContract.Data;
import android.provider.ContactsContract.RawContacts;
import android.util.Log;

public class DelContact {

	public static Context	mCtx = MobiiConnectionActivity.getContextApplication();

	private ContentResolver	mCr = mCtx.getContentResolver();
	private JSONObject		mJObj;
	private JSONArray		mJArray;

	/**
	 * @brief	Deletes a contact.
	 * @details	Parses a JSon string to get id of a contact in order to delete it.
	 * @param	contact A JSon string which contains contact information.
	 */
	public void		delContact(String contact) {
		try {
			this.mJArray = new JSONObject(contact).getJSONArray("data");
			this.mJObj = this.mJArray.getJSONObject(0);
			String id = String.valueOf(this.mJObj.getString("id"));

			ArrayList<ContentProviderOperation> ops = new ArrayList<ContentProviderOperation>();

			ops.add(ContentProviderOperation.newDelete(RawContacts.CONTENT_URI)
					.withSelection(RawContacts._ID + " = ? ", new String[] {id})
					.build());

			ops.add(ContentProviderOperation.newDelete(Data.CONTENT_URI)
					.withSelection(RawContacts.CONTACT_ID + " = ? ", new String[] {id})
					.build());

			try {
				this.mCr.applyBatch(ContactsContract.AUTHORITY, ops);
			} catch (RemoteException e) {
				Log.e("Contacts : delContact : RemoteException", e.toString());
			} catch (OperationApplicationException e) {
				Log.e("Contacts : delContact : OperationApplicationException", e.toString());
			}
		} catch (JSONException e) {
			Log.e("Contacts : delContact : JSONException", e.toString());
		}
	}
}
