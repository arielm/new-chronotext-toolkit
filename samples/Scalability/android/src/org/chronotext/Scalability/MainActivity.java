
package org.chronotext.Scalability;

import org.chronotext.cinder.CinderDelegate;
import org.chronotext.Utils;

import android.app.Activity;
import android.content.pm.ActivityInfo;
import android.content.res.Configuration;
import android.os.Bundle;

public class MainActivity extends Activity
{
  static
  {
    System.loadLibrary("Scalability");
  }

  static
  {
    Utils.DEBUG = true;
    Utils.TAG = "cinder";
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
  protected void onDestroy()
  {
    super.onDestroy();
    delegate.onDestroy();
  }
  
  @Override
  public void onBackPressed()
  {
    if (!delegate.onBackPressed())
    {
      super.onBackPressed();
    }
  }
  
  @Override
  protected void onStart()
  {
    Utils.LOGD("Activity.onStart");
    super.onStart();
  }

  @Override
  protected void onStop()
  {
    Utils.LOGD("Activity.onStop");
    super.onStop();
  }

  @Override
  protected void onRestart()
  {
    Utils.LOGD("Activity.onRestart");
    super.onRestart();
  }
}
