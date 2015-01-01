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
   * PARALLEL TO chronotext/cinder/CinderSketch/CinderDelegate.h
   */
  public static final int ACTION_CAPTURE_BACK_KEY = 1;
  public static final int ACTION_RELEASE_BACK_KEY = 2;

  protected Activity activity;

  protected CinderRenderer renderer;
  protected GLView view;
  
  protected boolean backKeyCaptured;

  public CinderDelegate(Activity _activity)
  {
    activity = _activity;

    performInit(); // WILL CREATE THE C++ CinderDelegate

    renderer = new CinderRenderer();
    view = new GLView(activity, renderer); // WILL START THE RENDERER'S THREAD
  }

  protected void performInit()
  {
    Display display = DisplayUtils.getDisplay(activity);
    Point displaySize = DisplayUtils.getRealSize(display);
    float displayDensity = DisplayUtils.getRealDensity(display);

    Utils.LOGD("CinderDelegate.init: " + displaySize.x + "x" + displaySize.y + " (" + displayDensity + " dpi)");
    init(activity, this, display, displaySize.x, displaySize.y, displayDensity);
  }

  public Activity getActivity()
  {
    return activity;
  }

  public GLView getView()
  {
    return view;
  }

  public void showView()
  {
    if (view.getVisibility() == View.GONE)
    {
      view.setVisibility(View.VISIBLE);
    }
  }

  public void hideView()
  {
    if (view.getVisibility() == View.VISIBLE)
    {
      view.setVisibility(View.GONE);
    }
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

  // ---------------------------------------- TO BE FORWARDED FROM THE HOST ACTIVITY (DO NOT OVERLOAD) ----------------------------------------

  public void onPause()
  {
    view.onPause(); // PURPOSELY NOT (IMMEDIATLY) QUEUED TO THE RENDERER'S THREAD
  }

  public void onResume()
  {
    view.onResume(); // PURPOSELY NOT (IMMEDIATLY) QUEUED TO THE RENDERER'S THREAD
  }

  public void onDestroy()
  {
    view.onDestroy(); // PURPOSELY NOT (IMMEDIATLY) QUEUED TO THE RENDERER'S THREAD
  }
  
  public boolean onBackPressed()
  {
    if (backKeyCaptured)
    {
      view.queueEvent(new Runnable()
      {
        public void run()
        {
          renderer.event(CinderRenderer.EVENT_BACK_KEY); 
        }
      });

      return true;
    }
    else
    {
      return false;
    }
  }

  // ---------------------------------------- SKETCH / DELEGATE COMMUNICATION ----------------------------------------

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
    view.sendMessage(what, (String) null);
  }

  /*
   * WILL BE QUEUED TO THE RENDERER'S THREAD
   */
  public void sendMessageToSketch(int what, String body)
  {
    view.sendMessage(what, body);
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

  protected native void init(Context context, Object listener, Display display, int displayWidth, int displayHeight, float displayDensity);

  // ---------------------------------------- ABSTRACT METHODS ----------------------------------------

  /*
   * TODO
   */
  // public abstract void paused();
  // public abstract void resumed();
  // public abstract void destroyed();
}
