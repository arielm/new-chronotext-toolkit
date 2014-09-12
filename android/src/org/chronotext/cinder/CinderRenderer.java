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

/*
 * WARNING: BE SURE TO DEFINE android:screenOrientation IN THE MANIFEST
 * OR TO CALL setRequestedOrientation() INSIDE Activity.onCreate()
 * BECAUSE THE CURRENT SYSTEM IS NOT HANDLING AUTO-ROTATION
 */

public class CinderRenderer extends GLRenderer
{
  public static final int EVENT_ATTACHED = 1;
  public static final int EVENT_DETACHED = 2;
  public static final int EVENT_PAUSED = 3;
  public static final int EVENT_RESUMED = 4;
  public static final int EVENT_SHOWN = 5;
  public static final int EVENT_HIDDEN = 6;
  public static final int EVENT_BACKGROUND = 7;
  public static final int EVENT_FOREGROUND = 8;
  public static final int EVENT_BACK_KEY = 9;

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

  public void launch()
  {
    Log.i("CHR", "*** CinderRenderer.launch ***");
    launch(mContext, mListener);
  }

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
      
    setup(width, height, diagonal, density, displayRotation);
    initialized = true;
  }

  public void draw(GL10 gl)
  {
    draw();
  }

  public void attached()
  {
    Log.i("CHR", "*** CinderRenderer.attached ***");

    event(EVENT_ATTACHED);
    attached = true;
    hidden = false; // TODO: ???
  }

  public void detached()
  {
    event(EVENT_DETACHED);
    attached = false;
  }

  public void paused(boolean contextLost)
  {
    Log.i("CHR", "*** CinderRenderer.paused: " + contextLost + " ***");

    if (contextLost)
    {
      contextLost();  
    }

    event(EVENT_PAUSED);
    resumed = false;
  }

  public void resumed(boolean contextRenewed)
  {
    Log.i("CHR", "*** CinderRenderer.resumed: " + contextRenewed + " ***");

    if (contextRenewed)
    {
      contextRenewed();
    }
      
    event(EVENT_RESUMED);
    resumed = true;
    hidden = false; // TODO: ???
  }

  public void background()
  {
    event(EVENT_BACKGROUND);
  }

  public void foreground()
  {
    event(EVENT_FOREGROUND);
  }

  public void shown()
  {
    event(EVENT_SHOWN);
    hidden = false; // TODO: ???
  }

  public void hidden()
  {
    event(EVENT_HIDDEN);
    hidden = true; // TODO: ???
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
  public native void draw();

  public native void contextRenewed();
  public native void contextLost();
  public native void event(int id);

  public native void addTouch(int index, float x, float y);
  public native void updateTouch(int index, float x, float y);
  public native void removeTouch(int index, float x, float y);

  public native void sendMessage(int what, String body);
}
