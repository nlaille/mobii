package com.mobii;

import android.content.SharedPreferences;
import android.os.Bundle;
import android.preference.PreferenceFragment;
import android.widget.CheckBox;

public class MobiiFragmentSettings extends PreferenceFragment implements ISettings {

	public SharedPreferences	mSharedMobii = MobiiConnectionActivity.getSharedPreferences();
	public boolean				boot_, wifi_;
	private CheckBox			bootCB_, wifiCB_;

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		//addPreferencesFromResource(R);

        this.wifi_ = false;
        this.boot_ = false;

        //this.wifiCB_ = (CheckBox) findViewById(R.id.wifi_connection);
        //this.bootCB_ = (CheckBox) findViewById(R.id.launch_startup);

        this.wifiCB_.setChecked(load().getBoolean(keyWifi_, false));
        this.bootCB_.setChecked(load().getBoolean(keyBoot_, false));
	}

    @Override
    public void		onResume() {
        super.onResume();
        this.wifiCB_.setChecked(load().getBoolean(keyWifi_, false));
        this.bootCB_.setChecked(load().getBoolean(keyBoot_, false));
    }

	@Override
    public void		onPause() {
        super.onPause();
        this.wifi_ = save(keyWifi_, this.wifiCB_.isChecked());
        this.boot_ = save(keyBoot_, this.bootCB_.isChecked());
    }

    @Override
    public void		onStop() {
        super.onStop();
        this.wifi_ = save(keyWifi_, this.wifiCB_.isChecked());
        this.boot_ = save(keyBoot_, this.bootCB_.isChecked());
    }

    /**
     * @brief	Saves the settings preferences in the shared preferences.
     * @param	key			A String corresponding to the keyword in the shared preferences.
     * @param	isChecked	A boolean, true if the corresponding checkbox is checked, otherwise false.
     * @return	A \e boolean equal to \e isChecked.
     */
    private boolean	save(String key, final boolean isChecked) {
        this.mSharedMobii.edit().putBoolean(key, isChecked).commit();
        return isChecked;
    }

    /**
     * @brief	Loads the settings preferences from the shared preferences.
     * @return	A \e SharedPreferences corresponding to the settings preferences.
     */
    private SharedPreferences	load() { 
    	return this.mSharedMobii;
	}
}
