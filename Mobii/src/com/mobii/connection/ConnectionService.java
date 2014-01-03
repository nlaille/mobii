package com.mobii.connection;

import com.mobii.MobiiConnectionActivity;

import android.app.Service;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.AsyncTask;
import android.os.Binder;
import android.os.IBinder;
import android.util.Log;

public class ConnectionService extends Service {

	public static SharedPreferences	mSharedMobii = MobiiConnectionActivity.getSharedPreferences();
	public boolean					mRun;

	private Thread					mThread;
	private Runnable				mSocketThread;
	private final IBinder			myBinder = new LocalBinder();
	private TcpClient				mTcpClient = new TcpClient();

	public class LocalBinder extends Binder {
		public ConnectionService getService() {
			return ConnectionService.this;
		}
	}

	@Override
	public IBinder onBind(Intent intent) {
		return myBinder;
	}

	@Override
	public void onCreate() {
		super.onCreate();
		this.mRun = false;
		mSharedMobii.edit().putBoolean("isConnected", false).commit();
		mSharedMobii.edit().putBoolean("isLogged", false).commit();
	}

	@Override
	public int onStartCommand(Intent intent, int flags, int startId) {
		super.onStartCommand(intent, flags, startId);
		this.mSocketThread = new ConnectionThread();
		this.mThread = new Thread(this.mSocketThread);
		this.mThread.start();
		Log.i("Thread", "Start");
		return START_STICKY;
	}

	@Override
	public void onDestroy() {
		super.onDestroy();
		new StopClientTask().execute();
	}

	private class StopClientTask extends AsyncTask<Void, Void, Void> {
		@Override
		protected Void doInBackground(Void... arg0) {
			mTcpClient.stop();
			return null;
		}
	}

	private class ConnectionThread implements Runnable {
		@Override
		public void run() {
			mTcpClient.run();
			mTcpClient.log();
			if (mTcpClient.mLog) {
				Log.i("Client", "Connected and Logged in to the server");
				mTcpClient.listen();
			}
		}
	}
}
