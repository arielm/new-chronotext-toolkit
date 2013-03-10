
package org.chronotext.complexsketch;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.pm.ActivityInfo;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;

/*
 *
 */

@SuppressLint("HandlerLeak")
public class MainActivity extends Activity
{
  Bridge mBridge;
  Handler mHandler;

  @Override
  public void onCreate(Bundle savedInstanceState)
  {
    super.onCreate(savedInstanceState);

    // ---

    /*
     * DEFINING THIS IN THE MANIFEST IS NOT ALWAYS WORKING... 
     */
    setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);

    mHandler = new Handler()
    {
      @Override
      public void handleMessage(Message msg)
      {
        MainActivity.this.handleMessage(msg);
      }
    };

    // ----

    mBridge = new Bridge(this, mHandler); // WILL LOAD THE ComplexSketch LIBRARY
    setContentView(mBridge.getView());
  }

  @Override
  protected void onPause()
  {
    super.onPause();
    mBridge.onPause();
  }

  @Override
  protected void onResume()
  {
    super.onResume();
    mBridge.onResume();
  }

  @Override
  protected void onDestroy()
  {
    super.onDestroy();
    mBridge.onDestroy();
  }

  // ---------------------------------------- HANDLER ----------------------------------------

  public void handleMessage(Message msg)
  {}
}
