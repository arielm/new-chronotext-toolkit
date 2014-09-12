
package org.chronotext.GrapeFrukt;

import org.chronotext.cinder.CinderDelegate;

import android.app.Activity;
import android.content.pm.ActivityInfo;
import android.content.res.Configuration;
import android.os.Bundle;
import android.util.Log;

public class MainActivity extends Activity
{
  static
  {
    System.loadLibrary("GrapeFrukt");
  }

  CinderDelegate delegate;

  @Override
  public void onCreate(Bundle savedInstanceState)
  {
    super.onCreate(savedInstanceState);

    delegate = new CinderDelegate(this, null);
    setContentView(delegate.getView());
  }

  @Override
  protected void onPause()
  {
    super.onPause();
    delegate.onPause();
  }

  @Override
  protected void onResume()
  {
    super.onResume();
    delegate.onResume();
  }

  @Override
  protected void onStart()
  {
    super.onStart();
    Log.i("CHR", "*** MainActivity.onStart ***");
  }

  @Override
  protected void onRestart()
  {
    super.onRestart();
    Log.i("CHR", "*** MainActivity.onRestart ***");
  }
  
  @Override
  protected void onDestroy()
  {
    super.onDestroy();
    delegate.onDestroy();
  }

  @Override
  public void onConfigurationChanged(Configuration newConfig)
  {
    super.onConfigurationChanged(newConfig);
    delegate.onConfigurationChanged(newConfig);
  }
  
  @Override
  public void onBackPressed()
  {
    if (!delegate.onBackPressed())
    {
      super.onBackPressed();
    }
  }
}
