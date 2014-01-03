/**
 * @file	Call.java
 * @author	Sebastien GUILLERM
 * @brief	Manages calls
 */

package com.mobii.features.call;

import android.content.Context;
import android.content.Intent;
import android.net.Uri;

public class Call {

	private Context	mCtx;

	public Call(final Context ctx) {
		this.mCtx = ctx;
	}

	public Call(Call c) {
		this.mCtx = c.mCtx;
	}

	/**
	 * @brief	Calls someone.
	 * @details	Gets a request from the server to call someone and launches the call to the phone number requested.
	 * @param	phoneNumber	A String corresponding to the phone number to call.
	 */
	public void sendCall(String phoneNumber) {
		Intent callIntent = new Intent(Intent.ACTION_CALL);
		callIntent.setData(Uri.parse("tel:"+phoneNumber));
		(this.mCtx).startActivity(callIntent);
	}

	/**
	 * @brief	Calls forwarding.
	 * @details	Forwards a call to the client when someone is calling.
	 */
	public void	forwardCall() {
		
	}
}
