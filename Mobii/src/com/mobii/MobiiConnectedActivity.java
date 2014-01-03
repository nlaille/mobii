package com.mobii;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import com.mobii.connection.Uid;

public class MobiiConnectedActivity extends Activity {

	public static final String		MOBII = "MobiiFile";
	public static SharedPreferences	mSharedMobii;
	public boolean					mSettingBoot, mSettingWifi;

	private static Context			mCtx;
	private TextView				mTvUid;
	private Button					mBtDisconnection;
	private String					mUid;

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_mobii_connected);

        this.mTvUid = (TextView) findViewById(R.id.tv_uid);
        this.mBtDisconnection = (Button) findViewById(R.id.bt_connection);
        this.mBtDisconnection.setOnClickListener(mConnectListener);

        mCtx = getApplicationContext();
		mSharedMobii = getSharedPreferences(MOBII, MODE_PRIVATE);

        this.mSettingBoot = mSharedMobii.getBoolean("option_boot", false);
        this.mSettingWifi = mSharedMobii.getBoolean("option_wifi", false);
	}

	@Override
	public void	onStart() {
		super.onStart();
		while((this.mUid = mSharedMobii.getString("user_id", "")).isEmpty() == true);
		this.mTvUid.setText(new Uid().displayUid(this.mUid));
	}

	@Override
	public void	onDestroy() {
		super.onDestroy();
		stopService(MobiiConnectionActivity.mIntentService);
	}

	private OnClickListener mConnectListener = new OnClickListener() {
		@Override
		public void onClick(View v) {
			onBackPressed();
		}
	};

	public static Context	getContextApplication() {
		return mCtx;
	}

	public static SharedPreferences	getSharedPreferences() {
		return mSharedMobii;
	}

	@Override
	public void onBackPressed() {
		AlertDialog.Builder requestDisconnection = new AlertDialog.Builder(MobiiConnectedActivity.this);
		requestDisconnection
			.setTitle("Disconnection")
			//.setIcon(R.drawable.ic_disconnection)
			.setCancelable(true);
				requestDisconnection
				.setMessage("Do you want to disconnect ?")
				.setPositiveButton("No", new DialogInterface.OnClickListener() {
					public void onClick(DialogInterface dialog, int id) {
						dialog.cancel();
					}
				})
				.setNegativeButton("Yes", new DialogInterface.OnClickListener() {
					public void onClick(DialogInterface dialog, int id) {
						finish();
					}
				});
		AlertDialog alertDisconnection = requestDisconnection.create();
		alertDisconnection.show();
	}

	@Override
	public boolean	onCreateOptionsMenu(Menu menu) {
		MenuInflater inflater = getMenuInflater();
		return true;
	}

	@Override
	public boolean	onMenuItemSelected(int featureId, MenuItem item) {
		return super.onMenuItemSelected(featureId, item);
	}
}
