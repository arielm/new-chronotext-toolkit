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
    // Display display = getDisplay();
    // DisplayMetrics dm = new DisplayMetrics();
    // display.getMetrics(dm);

    // Utils.LOGD("SDK VERSION: " + Build.VERSION.SDK_INT);
    // Utils.LOGD("ROTATION: " + display.getRotation());

    // Point realDisplaySize = getRealSize();
    // Utils.LOGD("REAL SIZE (PIXELS): " + realDisplaySize.x + " x " + realDisplaySize.y);

    // float density = getRealDensity();
    // Utils.LOGD("REAL DENSITY: " + density);

    // float widthInches = realDisplaySize.x / density;
    // float heightInches = realDisplaySize.y / density;
    // Utils.LOGD("REAL SIZE (INCHES): " + widthInches + " x " + heightInches);

    // float diagonal = (float) Math.sqrt(widthInches * widthInches + heightInches * heightInches);
    // Utils.LOGD("DIAGONAL: " + diagonal);

    // Utils.LOGD("dm.sizePixels: " + dm.widthPixels + " x " + dm.heightPixels);
    // Utils.LOGD("dm.densityDpi: " + dm.densityDpi);
    // Utils.LOGD("PHYSICAL DPI: " + dm.xdpi + " x " + dm.ydpi);

    Display display = DisplayUtils.getDisplay(mContext);
    float density = DisplayUtils.getRealDensity(display);

    launch(mContext, mListener, display, 0, density); // TODO: PASS "REAL" LANDSCAPE-SIZE AND DENSITY
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
  public native void launch(Context context, Object listener, Display display, float diagonal, float density);

  public native void setup(int width, int height);
  public native void shutdown();

  public native void resize(int width, int height);
  public native void event(int id);

  public native void draw();

  public native void addTouch(int index, float x, float y);
  public native void updateTouch(int index, float x, float y);
  public native void removeTouch(int index, float x, float y);

  public native void sendMessage(int what, String body);
}
