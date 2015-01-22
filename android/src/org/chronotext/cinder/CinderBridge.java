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

public class CinderBridge extends Handler
{
  /*
   * PARALLEL TO chronotext/cinder/CinderSketch.h
   */

  public static final int START_REASON_VIEW_SHOWN = 1;
  public static final int START_REASON_APP_RESUMED = 2;

  public static final int STOP_REASON_VIEW_HIDDEN = 1;
  public static final int STOP_REASON_APP_PAUSED = 2;

  public static final int ACTION_CAPTURE_BACK = 1;
  public static final int ACTION_RELEASE_BACK = 2;

  // ---

  public static final int THREAD_MAIN = 1;
  public static final int THREAD_RENDERER = 2;

  // ---

  protected Activity activity;

  protected GLView view;
  protected GLView.Properties viewProperties;
  
  protected boolean initialized;
  protected boolean backCaptured;

  public CinderBridge(Activity activity)
  {
    this.activity = activity;

    performInit(); // WILL CREATE THE C++ CinderBridge
  }

  public Activity getActivity()
  {
    return activity;
  }

  public GLView getView()
  {
    if (view == null)
    {
      if (viewProperties == null)
      {
        viewProperties = new GLView.Properties();
      }

      view = new GLView(activity, this, viewProperties); // WILL START THE RENDERER'S THREAD
    }

    return view;
  }

  public void setViewProperties(GLView.Properties properties)
  {
    viewProperties = properties;
  }

  // ---------------------------------------- LIFE-CYCLE ----------------------------------------

  protected void performInit()
  {
    if (!initialized)
    {
      Display display = DisplayUtils.getDisplay(activity);
      Point displaySize = DisplayUtils.getRealSize(display);
      float displayDensity = DisplayUtils.getRealDensity(display);

      Utils.LOGD("CinderBridge.performInit: " + displaySize.x + "x" + displaySize.y + " (" + displayDensity + " dpi)");
      init(this, activity, display, displaySize.x, displaySize.y, displayDensity);

      initialized = true;
    }
  }

  // ---------------------------------------- INVOKED ON THE MAIN-THREAD FROM GLView ----------------------------------------

  /*
   * IN PROGRESS...
   *
   * ALTERNATIVE: USING A SINGLE handleEvent(int eventId) METHOD (CONS: LONG AND CUMBERSOME ENUMS...)
   *
   * TODO:
   * - COMPLETE "BRIDGE CALLBACKS" SYSTEM
   * - INPUT (E.G. TOUCH, ACCELEROMETER), "POSTED FUNCTIONS", ETC. SHOULD BE DISPATCHED IN THE SAME ORDER ON ALL THE SUPPORTED PLATFORMS
   */

//public void appWillPause(int threadId) {}
//public void appDidResume(int threadId) {}
//public void appWillTerminate(int threadId) {}

  public void sketchDidInit(int threadId) {}
  public void sketchDidSetup(int threadId) {}

  public void sketchWillLaunch(int threadId) {}
  public void sketchDidLaunch(int threadId) {}

  public void sketchWillStart(int threadId, int reason) {}
  public void sketchDidStart(int threadId, int reason) {}

  public void sketchWillStop(int threadId, int reason) {}
  public void sketchDidStop(int threadId, int reason) {}

  public void sketchWillShutdown(int threadId) {}
  public void sketchDidShutdown(int threadId) {}

  // ---------------------------------------- TO BE FORWARDED FROM THE HOST ACTIVITY (DO NOT OVERRIDE) ----------------------------------------

  public void onPause()
  {
    if (view != null)
    {
      if (view.pause())
      {
        Utils.LOGD("CinderBridge.onPause");
        sketchWillStop(CinderBridge.THREAD_MAIN, CinderBridge.STOP_REASON_APP_PAUSED);
      }
    }
  }

  public void onResume()
  {
    if (view != null)
    {
      if (view.resume())
      {
        Utils.LOGD("CinderBridge.onResume");
        sketchWillStart(CinderBridge.THREAD_MAIN, CinderBridge.START_REASON_APP_RESUMED);
      }
    }
  }

  public void onDestroy()
  {
    if (view != null)
    {
      if (view.shutdown())
      {
        Utils.LOGD("CinderBridge.onDestroy");
        sketchWillShutdown(CinderBridge.THREAD_MAIN); // REASON: APP DESTROYED
      }
    }
  }
  
  public boolean onBackPressed()
  {
    if (view != null)
    {
      if (backCaptured)
      {
        Utils.LOGD("CinderBridge.onBackPressed");
        return view.backPressed();
      }
    }

    return false;
  }

  // ---------------------------------------- SKETCH <-> BRIDGE COMMUNICATION ----------------------------------------

  /*
   * WILL BE QUEUED TO THE MAIN-THREAD (VIA JAVA-HANDLER)
   *
   * TODO: DO NOT NECESSARILY QUEUE EVERY MESSSAGE TO THE MAIN-THREAD
   * PRE-REQUISITE: RECEIVING ADDITIONAL INFO FROM SKETCH REGARDING THE "CURRENT" AND "DESTINATION" THREADS, ETC.
   */
  public void messageFromSketch(int what, String body)
  {
    sendMessage(Message.obtain(this, what, body));
  }

  /*
   * WILL BE QUEUED TO THE RENDERER'S THREAD (VIA CPP-HANDLER)
   *
   * TODO: HANDLE MESSAGES SENT "BEFORE LAUNCH"
   */
  public void sendMessageToSketch(int what)
  {
    sendMessageToSketch(what, (String) null);
  }

  /*
   * TODO: FINALIZE THREAD-SAFETY POLICY
   */
  public boolean handleAction(int actionId)
  {
    switch (actionId)
    {
      case ACTION_CAPTURE_BACK:
        backCaptured = true;
        return true;

      case ACTION_RELEASE_BACK:
        backCaptured = false;
        return true;
    }

    return false;
  }

  // ---------------------------------------- QUERIES (TO BE CALLED FROM ANY THREAD ATTACHED TO JAVA) ----------------------------------------

  public synchronized String getMemoryInfo()
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

  protected native void init(Object bridge, Context context, Display display, int displayWidth, int displayHeight, float displayDensity);

  /*
   * WILL BE QUEUED TO THE RENDERER'S THREAD (VIA CPP-HANDLER)
   */
  public native void sendMessageToSketch(int what, String body);
}
