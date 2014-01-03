package com.mobii;

import android.app.Activity;
import android.os.Bundle;
import android.text.Html;
import android.text.method.LinkMovementMethod;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.widget.TextView;

public class MobiiAboutActivity extends Activity {
	@Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_mobii_about);

        // Website hyperlink
        final TextView mobiiSite = (TextView) findViewById(R.id.mobii_site);
    	mobiiSite.setClickable(true);
    	mobiiSite.setMovementMethod(LinkMovementMethod.getInstance());
        String mobiiLink = "<a href='http://www.getmobii.com/'> Mobii Website </a>";
        mobiiSite.setText(Html.fromHtml(mobiiLink));
	}

	@Override
	public void onBackPressed() {
		finish();
	}

	@Override
	public boolean	onCreateOptionsMenu(Menu menu) {
		MenuInflater inflater = getMenuInflater();
		inflater.inflate(R.menu.mobii_connection_menu, menu);
		return true;
	}

	@Override
	public boolean	onMenuItemSelected(int featureId, MenuItem item) {
		return super.onMenuItemSelected(featureId, item);
	}
}
