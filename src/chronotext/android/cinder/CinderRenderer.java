
package chronotext.android.cinder;

import javax.microedition.khronos.opengles.GL10;

import android.content.Context;
import android.content.res.AssetManager;
import android.content.res.Configuration;
import android.view.Display;
import android.view.Surface;
import android.view.WindowManager;
import chronotext.android.gl.GLRenderer;

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

  public static final int ACCELEROMETER_ROTATION_DEFAULT = 0;
  public static final int ACCELEROMETER_ROTATION_PORTRAIT = 1;
  public static final int ACCELEROMETER_ROTATION_LANDSCAPE = 2;

  protected Context mContext;
  protected Object mListener;

  public CinderRenderer(Context context, Object listener)
  {
    mContext = context;
    mListener = listener;
    
    prelaunch();
  }

  /*
   * BASED ON CODE FROM Cocos2dxAccelerometer.java
   * Copyright (c) 2010-2011 cocos2d-x.org 
   * http://www.cocos2d-x.org
   */
  protected int getAccelerometerRotation()
  {
    int orientation = mContext.getResources().getConfiguration().orientation;

    Display display = ((WindowManager) mContext.getSystemService(Context.WINDOW_SERVICE)).getDefaultDisplay();
    int naturalOrientation = display.getOrientation();

    if ((orientation == Configuration.ORIENTATION_LANDSCAPE) && (naturalOrientation != Surface.ROTATION_0))
    {
      return ACCELEROMETER_ROTATION_LANDSCAPE;
    }
    else if ((orientation == Configuration.ORIENTATION_PORTRAIT) && (naturalOrientation != Surface.ROTATION_0))
    {
      return ACCELEROMETER_ROTATION_PORTRAIT;
    }

    return ACCELEROMETER_ROTATION_DEFAULT;
  }

  // ---------------------------------------- CALL-BACKS TAKING PLACE ON THE RENDERER'S THREAD ----------------------------------------
  
  public void launch()
  {
    launch(mContext.getAssets(), mListener);
  }

  public void setup(GL10 gl, int width, int height)
  {
    setup(width, height, getAccelerometerRotation());
    initialized = true;
  }

  public void draw(GL10 gl)
  {
    draw();
  }

  public void attached()
  {
    event(EVENT_ATTACHED);
    attached = true;
    hidden = false;
  }

  public void detached()
  {
    event(EVENT_DETACHED);
    attached = false;
  }

  public void paused()
  {
    event(EVENT_PAUSED);
    resumed = false;
  }

  public void resumed()
  {
    event(EVENT_RESUMED);
    resumed = true;
    hidden = false;
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
    hidden = false;
  }

  public void hidden()
  {
    event(EVENT_HIDDEN);
    hidden = true;
  }

  // ---------------------------------------- JNI ----------------------------------------

  public native void prelaunch();
  
  public native void launch(AssetManager assets, Object listener);

  public native void setup(int width, int height, int accelerometerRotation);
  
  public native void shutdown();

  public native void draw();

  public native void event(int id);

  public native void addTouch(float x, float y);

  public native void updateTouch(float x, float y);

  public native void removeTouch(float x, float y);

  public native void sendMessage(int what, String body);
}
