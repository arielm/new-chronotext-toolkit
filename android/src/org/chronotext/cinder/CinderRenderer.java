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

import android.content.Context;
import android.content.res.Configuration;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.Display;
import android.view.Surface;
import android.view.WindowManager;

public class CinderRenderer extends GLRenderer
{
  public static final int EVENT_RESUMED = 1;
  public static final int EVENT_ATTACHED = 2;
  public static final int EVENT_SHOWN = 3;
  public static final int EVENT_PAUSED = 4;
  public static final int EVENT_DETACHED = 5;
  public static final int EVENT_HIDDEN = 6;
  public static final int EVENT_CONTEXT_LOST = 7;
  public static final int EVENT_CONTEXT_RENEWED = 8;
  public static final int EVENT_BACKGROUND = 9;
  public static final int EVENT_FOREGROUND = 10;
  public static final int EVENT_BACK_KEY = 11;

  protected Context mContext;
  protected Object mListener;

  public CinderRenderer(Context context, Object listener)
  {
    mContext = context;
    mListener = listener;

    prelaunch();
  }

  protected WindowManager getWindowManager()
  {
    return (WindowManager) mContext.getSystemService(Context.WINDOW_SERVICE);
  }
    
  // ---------------------------------------- CALL-BACKS TAKING PLACE ON THE RENDERER'S THREAD ----------------------------------------

  public void setup(GL10 gl, int width, int height)
  {
    Log.i("CHR", "*** CinderRenderer.setup ***");

    Display display = getWindowManager().getDefaultDisplay();
    DisplayMetrics dm = new DisplayMetrics();
    display.getMetrics(dm);

    float widthInches = (float) dm.widthPixels / dm.xdpi;
    float heightInches = (float) dm.heightPixels / dm.ydpi;
    float diagonal = (float) Math.sqrt(widthInches * widthInches + heightInches * heightInches);

    /*
     * MORE RELIABLE THAN dm.densityDpi
     */
    float density = (float) Math.sqrt(dm.widthPixels * dm.widthPixels + dm.heightPixels * dm.heightPixels) / diagonal;

    /*
     * REFERENCES:
     * http://android-developers.blogspot.co.il/2010/09/one-screen-turn-deserves-another.html
     * http://developer.download.nvidia.com/tegra/docs/tegra_android_accelerometer_v5f.pdf
     */
    int displayRotation = display.getRotation();

    launch(mContext, mListener);
    setup(width, height, diagonal, density, displayRotation);
  }

  public void resize(GL10 gl, int width, int height)
  {
    Log.i("CHR", "*** CinderRenderer.resize: " + width + "x" + height + " ***");
    resize();
  }

  public void draw(GL10 gl)
  {
    draw();
  }

  public void start(int reason)
  {
    Log.i("CHR", "*** CinderRenderer.start: " + reason + " ***");
    event(reason);
  }

  public void stop(int reason)
  {
    Log.i("CHR", "*** CinderRenderer.stop: " + reason + " ***");
    event(reason);
  }

  public void contextLost()
  {
    Log.i("CHR", "*** CinderRenderer.contextLost ***");
    event(EVENT_CONTEXT_LOST);
  }

  public void contextRenewed()
  {
    Log.i("CHR", "*** CinderRenderer.contextRenewed ***");
    event(EVENT_CONTEXT_RENEWED);
  }

  public void foreground()
  {
    Log.i("CHR", "*** CinderRenderer.foreground ***");
    event(EVENT_FOREGROUND);
  }

  public void background()
  {
    Log.i("CHR", "*** CinderRenderer.background ***");
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
  public native void launch(Context context, Object listener);

  public native void setup(int width, int height, float diagonal, float density, int displayRotation);
  public native void shutdown();

  public native void resize();
  public native void draw();

  public native void event(int id);

  public native void addTouch(int index, float x, float y);
  public native void updateTouch(int index, float x, float y);
  public native void removeTouch(int index, float x, float y);

  public native void sendMessage(int what, String body);
}
