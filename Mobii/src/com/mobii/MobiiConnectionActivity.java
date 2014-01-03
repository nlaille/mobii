/**
 * @file	MobiiConnectionActivity.java
 * @author	Sebastien GUILLERM
 * @brief	Main activity.
 */

package com.mobii;

import com.mobii.connection.ConnectionService;
import com.mobii.connection.Uid;
import com.mobii.connection.Wifi;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.ComponentName;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.ServiceConnection;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.os.IBinder;
import android.util.Log;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
 
public class MobiiConnectionActivity extends Activity {

	public static final String		MOBII = "MobiiFile";
	public static SharedPreferences	mSharedMobii;
	public boolean					mSettingBoot, mSettingWifi;

	public static Intent			mIntent, mIntentService;

	private static Context			mCtx;
	private EditText				mEtPwd;
	private TextView				mTvUid;
	private Button					mBtConnection;
	final private String			mDefaultPwd = "mobii2014";
	private String					mUid, mPwd;
	private boolean					mBound;
	private ConnectionService			mBoundService;

	private ServiceConnection mConnection = new ServiceConnection() {
		@Override
		public void onServiceConnected(ComponentName name, IBinder service) {
			mBoundService = ((ConnectionService.LocalBinder)service).getService();
		}

		@Override
		public void onServiceDisconnected(ComponentName name) {
			mBoundService = null;
		}
	};

	private void doBindService() {
		bindService(new Intent(MobiiConnectionActivity.this, ConnectionService.class), mConnection, Context.BIND_AUTO_CREATE);
		this.mBound = true;
		Log.i("MobiiService", "Bound");
		if (mBoundService != null) {
			Log.i("doBindService", "Success");
			setContentView(R.layout.activity_mobii_connecting);
		}
	}

	private void doUnbindService() {
		if (mBound) {
			unbindService(mConnection);
			this.mBound = false;
			Log.i("MobiiService", "Unbound");
		}
	}

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		initInterface();

		mCtx = getApplicationContext();
		mSharedMobii = getSharedPreferences(MOBII, MODE_PRIVATE);
    	mIntent = new Intent(MobiiConnectionActivity.getContextApplication(), MobiiConnectedActivity.class);
    	mIntentService = new Intent(MobiiConnectionActivity.getContextApplication(), ConnectionService.class);

    	this.mSettingBoot = mSharedMobii.getBoolean("option_boot", false);
    	this.mSettingWifi = mSharedMobii.getBoolean("option_wifi", false);
	}

	@Override
	public void	onStart() {
		super.onStart();
		this.mUid = mSharedMobii.getString("user_id", "");
		this.mTvUid.setText(new Uid().displayUid(this.mUid));

		if (this.mBound) {
			doUnbindService();
			initInterface();
			this.mTvUid.setText(new Uid().displayUid(this.mUid));
		}
	}

	public void	initInterface() {
		setContentView(R.layout.activity_mobii_connection);
		this.mEtPwd = (EditText) findViewById(R.id.et_pwd);
		this.mTvUid = (TextView) findViewById(R.id.tv_uid);
		this.mBtConnection = (Button) findViewById(R.id.bt_connection);
		this.mBtConnection.setOnClickListener(mConnectListener);
	}

	private OnClickListener mConnectListener = new OnClickListener() {
		@Override
		public void onClick(View v) {
			mPwd = mEtPwd.getText().toString().length() >= 8 ? mEtPwd.getText().toString() : mDefaultPwd;
			mSharedMobii.edit().putString("password", mPwd).commit();
			Log.i("PWD", mPwd);

			setContentView(R.layout.activity_mobii_connecting);
			if (!new Wifi(MobiiConnectionActivity.this, mSharedMobii).checkWifiOnly(false)) {
				startService(mIntentService);
				doBindService();
				if (mBound) {
					startActivity(mIntent);
				}
			}
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
		AlertDialog.Builder requestExit = new AlertDialog.Builder(MobiiConnectionActivity.this);
		requestExit
			.setTitle("Exit")
			.setIcon(R.drawable.ic_exit)
			.setMessage("Do you want to exit ?")
			.setCancelable(true)
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
		AlertDialog alertExit = requestExit.create();
		alertExit.show();
	}

	@Override
	public boolean	onCreateOptionsMenu(Menu menu) {
		MenuInflater inflater = getMenuInflater();
		inflater.inflate(R.menu.mobii_connection_menu, menu);
		return true;
	}

	@Override
	public boolean	onMenuItemSelected(int featureId, MenuItem item) {
		switch (item.getItemId()) {
		case R.id.item_settings:
			Intent intent_settings = new Intent(MobiiConnectionActivity.this, MobiiSettingsActivity.class);
			startActivity(intent_settings);
			return true;

		case R.id.item_about:
			Intent intent_about = new Intent(MobiiConnectionActivity.this, MobiiAboutActivity.class);
			startActivity(intent_about);
			return true;

		case R.id.item_exit:
			onBackPressed();
			return true;
		}
		return super.onMenuItemSelected(featureId, item);
	}
}
