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
import org.chronotext.Utils;

import android.content.Context;
import android.content.res.Configuration;
import android.util.DisplayMetrics;
import android.view.Display;
import android.view.Surface;
import android.view.WindowManager;

import android.graphics.Point;
import android.os.Build;

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

  public Display getDisplay()
  {
    return ((WindowManager) mContext.getSystemService(Context.WINDOW_SERVICE)).getDefaultDisplay();
  }

  public Point getRealSize()
  {
    Display display = getDisplay();
    DisplayMetrics metrics = new DisplayMetrics();
    display.getMetrics(metrics);

    /*
     * PROBLEM: THE FOLLOWING IS AFFECTED BY "SYSTEM DECORATIONS" (E.G. NAVIGATION BAR)
     */
    int widthPixels = metrics.widthPixels;
    int heightPixels = metrics.heightPixels;

    /*
     * SOLUTIONS...
     *
     * REFERENCE: http://stackoverflow.com/a/15699681/50335
     */

    if (Build.VERSION.SDK_INT >= 14 && Build.VERSION.SDK_INT < 17)
    {
      try
      {
        widthPixels = (Integer) Display.class.getMethod("getRawWidth").invoke(display);
        heightPixels = (Integer) Display.class.getMethod("getRawHeight").invoke(display);
      }
      catch (Exception ignored)
      {}
    }

    if (Build.VERSION.SDK_INT >= 17)
    {
      try
      {
        Point realSize = new Point();
        Display.class.getMethod("getRealSize", Point.class).invoke(display, realSize);

        widthPixels = realSize.x;
        heightPixels = realSize.y;
      }
      catch (Exception ignored)
      {}
    }

    return new Point(widthPixels, heightPixels);
  }

  public Point getRealNaturalSize()
  {
    Point realSize = getRealSize();
    int rotation = getDisplay().getRotation();

    if ((rotation == Surface.ROTATION_0) || (rotation == Surface.ROTATION_180))
    {
      return realSize;
    }
    else
    {
      return new Point(realSize.y, realSize.x);
    }
  }

  public Point getRealLandscapeSize()
  {
    Point realSize = getRealSize();

    if (realSize.x > realSize.y)
    {
      return realSize;
    }
    else
    {
      return new Point(realSize.y, realSize.x);
    }
  }

  public float getRealDensity()
  {
    Display display = getDisplay();
    DisplayMetrics metrics = new DisplayMetrics();
    display.getMetrics(metrics);

    Point realNaturalSize = getRealNaturalSize();
    float widthInches = realNaturalSize.x / metrics.xdpi;
    float heightInches = realNaturalSize.y / metrics.ydpi;

    double diagonal = Math.sqrt(widthInches * widthInches + heightInches * heightInches);
    double density = Math.sqrt(realNaturalSize.x * realNaturalSize.x + realNaturalSize.y * realNaturalSize.y) / diagonal;

    return (float) density;
  }

  // ---------------------------------------- CALLBACKS TAKING PLACE ON THE RENDERER'S THREAD ----------------------------------------

  public void launch()
  {
    Display display = getDisplay();
    DisplayMetrics dm = new DisplayMetrics();
    display.getMetrics(dm);

    // float widthInches = (float) dm.widthPixels / dm.xdpi;
    // float heightInches = (float) dm.heightPixels / dm.ydpi;
    // float diagonal = (float) Math.sqrt(widthInches * widthInches + heightInches * heightInches);

    /*
     * MORE RELIABLE THAN dm.densityDpi (THE LATTER IS APPROXIMATING TO "BUCKETS" OF DENSITY: 120, 160, 240, 320...)
     */
    // float density = (float) Math.sqrt(dm.widthPixels * dm.widthPixels + dm.heightPixels * dm.heightPixels) / diagonal;


    Utils.LOGD("SDK VERSION: " + Build.VERSION.SDK_INT);
    Utils.LOGD("ROTATION: " + display.getRotation());

    Point getRealLandscapeSize = getRealLandscapeSize();
    Utils.LOGD("REAL LANDSCAPE SIZE (PIXELS): " + getRealLandscapeSize.x + " x " + getRealLandscapeSize.y);

    float density = getRealDensity();
    Utils.LOGD("REAL DENSITY: " + density);

    float widthInches = getRealLandscapeSize.x / density;
    float heightInches = getRealLandscapeSize.y / density;
    Utils.LOGD("REAL LANDSCAPE SIZE (INCHES): " + widthInches + " x " + heightInches);

    float diagonal = (float) Math.sqrt(widthInches * widthInches + heightInches * heightInches);
    Utils.LOGD("DIAGONAL: " + diagonal);

    Utils.LOGD("dm.sizePixels: " + dm.widthPixels + " x " + dm.heightPixels);
    Utils.LOGD("dm.densityDpi: " + dm.densityDpi);
    Utils.LOGD("PHYSICAL DPI: " + dm.xdpi + " x " + dm.ydpi);


    // Utils.LOGD("LCD DENSITY: " + DisplayMetrics.DENSITY_DEFAULT_SCALE);
    // Utils.LOGD("REAL METRICS:" + display.getRealMetrics());
    
    // Utils.LOGD("DIAGONAL: " + diagonal);
    // Utils.LOGD("DENSITY: " + density);

    launch(mContext, mListener, display, diagonal, density); // TODO: PASS "REAL" LANDSCAPE-SIZE AND DENSITY
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
