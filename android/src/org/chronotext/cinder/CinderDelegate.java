/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

package org.chronotext.cinder;

import org.chronotext.gl.GLView;
import org.chronotext.utils.DisplayUtils;
import org.chronotext.utils.Utils;

import org.json.JSONException;
import org.json.JSONObject;

import android.app.Activity;
import android.app.ActivityManager;
import android.app.ActivityManager.MemoryInfo;
import android.content.Context;
import android.graphics.Point;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.Display;
import android.view.View;

public class CinderDelegate extends Handler
{
  /*
   * PARALLEL TO chronotext/cinder/CinderDelegate.h
   */
  public static final int ACTION_CAPTURE_BACK_KEY = 1;
  public static final int ACTION_RELEASE_BACK_KEY = 2;

  protected Activity activity;
  protected CinderRenderer cinderRenderer;
  protected GLView glView;
  
  protected boolean backKeyCaptured;

  public CinderDelegate(Activity _activity)
  {
    activity = _activity;

    performInit(); // WILL CREATE THE C++ CinderDelegate

    cinderRenderer = new CinderRenderer();
    glView = new GLView(activity, this, cinderRenderer); // WILL START THE RENDERER'S THREAD
  }

  public Activity getActivity()
  {
    return activity;
  }

  public GLView getView()
  {
    return glView;
  }

  public void showView()
  {
    if (glView.getVisibility() == View.GONE)
    {
      glView.setVisibility(View.VISIBLE);
    }
  }

  public void hideView()
  {
    if (glView.getVisibility() == View.VISIBLE)
    {
      glView.setVisibility(View.GONE);
    }
  }

  // ---------------------------------------- LIFE-CYCLE ----------------------------------------

  protected void performInit()
  {
    Display display = DisplayUtils.getDisplay(activity);
    Point displaySize = DisplayUtils.getRealSize(display);
    float displayDensity = DisplayUtils.getRealDensity(display);

    Utils.LOGD("CinderDelegate.init: " + displaySize.x + "x" + displaySize.y + " (" + displayDensity + " dpi)");
    init(this, activity, display, displaySize.x, displaySize.y, displayDensity);
  }

  protected void handleAction(int actionId)
  {
    switch (actionId)
    {
      case ACTION_CAPTURE_BACK_KEY:
        backKeyCaptured = true;
        break;

      case ACTION_RELEASE_BACK_KEY:
        backKeyCaptured = false;
        break;
    }
  }

  // ---------------------------------------- TO BE FORWARDED FROM THE HOST ACTIVITY (DO NOT OVERRIDE) ----------------------------------------

  /*
   * WILL BE QUEUED TO THE RENDERER'S THREAD
   */
  public void onPause()
  {
    Utils.LOGD("CinderDelegate.onPause");
    glView.onPause();
  }

  /*
   * WILL BE QUEUED TO THE RENDERER'S THREAD
   */
  public void onResume()
  {
    Utils.LOGD("CinderDelegate.onResume");
    glView.onResume();
  }

  /*
   * WILL BE QUEUED TO THE RENDERER'S THREAD
   */
  public void onDestroy()
  {
    Utils.LOGD("CinderDelegate.onDestroy");
    glView.onDestroy();
  }
  
  public boolean onBackPressed()
  {
    Utils.LOGD("CinderDelegate.onBackPressed");

    if (backKeyCaptured)
    {
      glView.queueEvent(new Runnable()
      {
        public void run()
        {
          cinderRenderer.event(CinderRenderer.EVENT_BACK_KEY); 
        }
      });

      return true;
    }

    return false;
  }

  // ---------------------------------------- CALLBACKS INVOKED ON THE MAIN-THREAD ----------------------------------------

  public void pausing()
  {}

  public void resuming()
  {}

  public void finishing()
  {}

  // ---------------------------------------- SKETCH <-> DELEGATE COMMUNICATION ----------------------------------------

  /*
   * INVOKED ON THE RENDERER'S THREAD
   */
  public void action(final int actionId)
  {
    activity.runOnUiThread(new Runnable()
    {
      public void run()
      {
        handleAction(actionId);
      }
    });
  }

  /*
   * INVOKED ON THE RENDERER'S THREAD
   */
  public void receiveMessageFromSketch(int what, String body)
  {
      sendMessage(Message.obtain(this, what, body));
  }

  /*
   * WILL BE QUEUED TO THE RENDERER'S THREAD
   */
  public void sendMessageToSketch(int what)
  {
    glView.sendMessage(what, (String) null);
  }

  /*
   * WILL BE QUEUED TO THE RENDERER'S THREAD
   */
  public void sendMessageToSketch(int what, String body)
  {
    glView.sendMessage(what, body);
  }

  // ---------------------------------------- QUERIES (TO BE CALLED FROM ANY THREAD ATTACHED TO JAVA) ----------------------------------------

  public String getMemoryInfo()
  {
    MemoryInfo memoryInfo = new MemoryInfo();
    ((ActivityManager) activity.getSystemService(Context.ACTIVITY_SERVICE)).getMemoryInfo(memoryInfo);

    JSONObject json = new JSONObject();

    try 
    {
      json.put("availMem", memoryInfo.availMem);
      json.put("threshold", memoryInfo.threshold);
      json.put("lowMemory", memoryInfo.lowMemory);
    } 
    catch (JSONException e) 
    {}

    return json.toString();
  }

  // ---------------------------------------- JNI ----------------------------------------

  protected native void init(Object listener, Context context, Display display, int displayWidth, int displayHeight, float displayDensity);
}
