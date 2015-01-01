/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

package org.chronotext.cinder;

import java.util.Vector;

import javax.microedition.khronos.opengles.GL10;

import org.chronotext.gl.GLRenderer;
import org.chronotext.gl.Touch;
import org.chronotext.utils.Utils;

public class CinderRenderer extends GLRenderer
{
  /*
   * PARALLEL TO chronotext/cinder/CinderSketch/CinderDelegate.h
   */
  public static final int EVENT_RESUMED = 1;
  public static final int EVENT_SHOWN = 2;
  public static final int EVENT_PAUSED = 3;
  public static final int EVENT_HIDDEN = 4;
  public static final int EVENT_CONTEXT_LOST = 5;
  public static final int EVENT_CONTEXT_RENEWED = 6;
  public static final int EVENT_BACKGROUND = 7;
  public static final int EVENT_FOREGROUND = 8;
  public static final int EVENT_BACK_KEY = 9;

  // ---------------------------------------- CALLBACKS TAKING PLACE ON THE RENDERER'S THREAD ----------------------------------------

  public void launch()
  {}

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
    draw();
  }

  public void start(int reason)
  {
    Utils.LOGD("CinderRenderer.start: " + (reason == REASON_RESUMED ? "RESUMED" : "SHOWN"));
    event(reason);
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
