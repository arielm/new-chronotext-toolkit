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

  protected Display getDisplay()
  {
    return ((WindowManager) mContext.getSystemService(Context.WINDOW_SERVICE)).getDefaultDisplay();
  }
    
  // ---------------------------------------- CALL-BACKS TAKING PLACE ON THE RENDERER'S THREAD ----------------------------------------

  public void launch()
  {
    /*
     * TODO: CREATE DisplayInfo ON THE C++ SIDE (CF WindowInfo/DisplayInfo REDESIGN...)
     */
    launch(mContext, mListener, getDisplay());
  }

  public void setup(GL10 gl, int width, int height)
  {
    Utils.LOGD("CinderRenderer.setup");

    /*
     * TODO:
     * THE WindowInfo CONCEPT SHOULD BE REPLACED BY DisplayInfo
     * SINCE VIEWPORT-SIZE CAN BE DIFFERENT FROM DISPLAY-SIZE
     */

    Display display = getDisplay();
    DisplayMetrics dm = new DisplayMetrics();
    display.getMetrics(dm);

    float widthInches = (float) dm.widthPixels / dm.xdpi;
    float heightInches = (float) dm.heightPixels / dm.ydpi;
    float diagonal = (float) Math.sqrt(widthInches * widthInches + heightInches * heightInches);

    /*
     * MORE RELIABLE THAN dm.densityDpi
     */
    float density = (float) Math.sqrt(dm.widthPixels * dm.widthPixels + dm.heightPixels * dm.heightPixels) / diagonal;

    setup(width, height, diagonal, density); // TODO: WE SHOULD ONLY PASS width AND height, I.E. THE VIEWPORT-SIZE (CF WindowInfo/DisplayInfo REDESIGN...)
  }

  public void resize(GL10 gl, int width, int height)
  {
    Utils.LOGD("CinderRenderer.resize: " + width + "x" + height);
    resize();
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
  public native void launch(Context context, Object listener, Display display);

  public native void setup(int width, int height, float diagonal, float density);
  public native void shutdown();

  public native void resize();
  public native void draw();

  public native void event(int id);

  public native void addTouch(int index, float x, float y);
  public native void updateTouch(int index, float x, float y);
  public native void removeTouch(int index, float x, float y);

  public native void sendMessage(int what, String body);
}
