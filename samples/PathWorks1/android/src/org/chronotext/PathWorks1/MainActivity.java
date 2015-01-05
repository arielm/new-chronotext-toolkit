
package org.chronotext.PathWorks1;

import org.chronotext.cinder.CinderDelegate;
import org.chronotext.utils.Utils;

import android.app.Activity;
import android.content.pm.ActivityInfo;
import android.os.Bundle;

public class MainActivity extends Activity
{
  static
  {
    Utils.DEBUG = true;
    Utils.TAG = "cinder";

    System.loadLibrary("PathWorks1");
  }

  CinderDelegate delegate;

  @Override
  public void onCreate(Bundle savedInstanceState)
  {
    super.onCreate(savedInstanceState);

    delegate = new CinderDelegate(this);
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
}
