/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

package org.chronotext.PathWorks2;

import org.chronotext.cinder.CinderBridge;
import org.chronotext.utils.Utils;

import android.app.Activity;
import android.os.Bundle;

public class MainActivity extends Activity
{
  static
  {
    Utils.DEBUG = true;
    Utils.TAG = "cinder";

    System.loadLibrary("PathWorks2");
  }

  CinderBridge bridge;

  @Override
  public void onCreate(Bundle savedInstanceState)
  {
    super.onCreate(savedInstanceState);

    bridge = new CinderBridge(this);
    setContentView(bridge.getView());
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
}
