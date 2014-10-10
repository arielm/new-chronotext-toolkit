/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

package org.chronotext.cinder;

import java.util.Vector;

import javax.microedition.khronos.opengles.GL10;

import org.chronotext.gl.GLRenderer;
import org.chronotext.gl.Touch;
import org.chronotext.utils.DisplayUtils;
import org.chronotext.utils.Utils;

import android.content.Context;
import android.graphics.Point;
import android.view.Display;

public class CinderRenderer extends GLRenderer
{
  public static final int EVENT_RESUMED = 1;
  public static final int EVENT_SHOWN = 2;
  public static final int EVENT_PAUSED = 3;
  public static final int EVENT_HIDDEN = 4;
  public static final int EVENT_CONTEXT_LOST = 5;
  public static final int EVENT_CONTEXT_RENEWED = 6;
  public static final int EVENT_BACKGROUND = 7;
  public static final int EVENT_FOREGROUND = 8;
  public static final int EVENT_BACK_KEY = 9;

  protected Context mContext;
  protected Object mListener;

  protected int ticks;

  public CinderRenderer(Context context, Object listener)
  {
    mContext = context;
    mListener = listener;

    prelaunch();
  }

  // ---------------------------------------- CALLBACKS TAKING PLACE ON THE RENDERER'S THREAD ----------------------------------------

  public void launch()
  {
    Display display = DisplayUtils.getDisplay(mContext);
    Point displaySize = DisplayUtils.getRealSize(display);
    float displayDensity = DisplayUtils.getRealDensity(display);

    Utils.LOGD("CinderRenderer.launch: " + displaySize.x + "x" + displaySize.y + " (" + displayDensity + "dpi)");
    launch(mContext, mListener, display, displaySize.x, displaySize.y, displayDensity);
  }

  public void setup(GL10 gl, int width, int height)
  {
    Utils.LOGD("CinderRenderer.setup: " + width + "x" + height);
    setup(width, height);
  }

  public void resize(GL10 gl, int width, int height)
  {
    Utils.LOGD("CinderRenderer.resize: " + width + "x" + height);
    resize(width, height);
  }

  public void draw(GL10 gl)
  {
    if (ticks++ == 0)
    {
      Utils.LOGD("CinderRenderer.draw");  
    }
    
    draw();
  }

  public void start(int reason)
  {
    Utils.LOGD("CinderRenderer.start: " + (reason == REASON_RESUMED ? "RESUMED" : "SHOWN"));
    event(reason);

    ticks = 0;
  }

  public void stop(int reason)
  {
    Utils.LOGD("CinderRenderer.stop: " + (reason == REASON_PAUSED ? "PAUSED" : "HIDDEN"));
    event(reason);
  }

  public void contextLost()
  {
    Utils.LOGD("CinderRenderer.contextLost");
    event(EVENT_CONTEXT_LOST);
  }

  public void contextRenewed()
  {
    Utils.LOGD("CinderRenderer.contextRenewed");
    event(EVENT_CONTEXT_RENEWED);
  }

  public void foreground()
  {
    Utils.LOGD("CinderRenderer.foreground");
    event(EVENT_FOREGROUND);
  }

  public void background()
  {
    Utils.LOGD("CinderRenderer.background");
    event(EVENT_BACKGROUND);
  }

  public void addTouches(Vector<Touch> touches)
  {
    for (Touch touch : touches)
    {
      addTouch(touch.index, touch.x, touch.y);
    }
  }

  public void updateTouches(Vector<Touch> touches)
  {
    for (Touch touch : touches)
    {
      updateTouch(touch.index, touch.x, touch.y);
    }
  }

  public void removeTouches(Vector<Touch> touches)
  {
    for (Touch touch : touches)
    {
      removeTouch(touch.index, touch.x, touch.y);
    }
  }

  // ---------------------------------------- JNI ----------------------------------------

  public native void prelaunch();
  public native void launch(Context context, Object listener, Display display, int displayWidth, int displayHeight, float displayDensity);

  public native void setup(int width, int height);
  public native void shutdown();

  public native void resize(int width, int height);
  public native void event(int eventId);
  
  public native void draw();

  public native void addTouch(int index, float x, float y);
  public native void updateTouch(int index, float x, float y);
  public native void removeTouch(int index, float x, float y);

  public native void sendMessage(int what, String body);
}
