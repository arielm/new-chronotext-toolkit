/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

package org.chronotext.MobileTest;

import org.chronotext.utils.Utils;

import android.app.Activity;
import android.os.Bundle;

public class MainActivity extends Activity
{
  static
  {
    Utils.DEBUG = true;
    Utils.TAG = "cinder";
  }

  Bridge bridge;

  @Override
  public void onCreate(Bundle savedInstanceState)
  {
    super.onCreate(savedInstanceState);
    bridge = new Bridge(this);
  }

  @Override
  protected void onPause()
  {
    super.onPause();
    bridge.onPause();
  }

  @Override
  protected void onResume()
  {
    super.onResume();
    bridge.onResume();
  }
  
  @Override
  public void onBackPressed()
  {
    if (!bridge.onBackPressed())
    {
      super.onBackPressed();
    }
  }

  /*
   * THE FOLLOWING IS NOT MANDATORY (CURRENTLY USED FOR DEBUGGING PURPOSES)
   */

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
