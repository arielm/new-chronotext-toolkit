/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

package org.chronotext.cinder;

import java.util.Vector;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import org.chronotext.cinder.CinderBridge;
import org.chronotext.cinder.Touch;
import org.chronotext.utils.Utils;

import android.opengl.GLSurfaceView;
import android.view.View;

public class CinderRenderer implements GLSurfaceView.Renderer
{
  /*
   * PARALLEL TO chronotext/cinder/CinderSketch.h
   */
  public static final int EVENT_RESUMED = 1;
  public static final int EVENT_SHOWN = 2;
  public static final int EVENT_PAUSED = 3;
  public static final int EVENT_HIDDEN = 4;
  public static final int EVENT_FOREGROUND = 5;
  public static final int EVENT_BACKGROUND = 6;
  public static final int EVENT_MEMORY_WARNING = 7;
  public static final int EVENT_CONTEXT_LOST = 8;
  public static final int EVENT_CONTEXT_RENEWED = 9;
  public static final int EVENT_TRIGGER_BACK = 10;
  public static final int EVENT_TRIGGER_ESCAPE = 11;

  protected CinderBridge cinderBridge;

  protected boolean launched;
  protected boolean setup;
  protected boolean attached;
  protected boolean paused;
  protected boolean hidden;
  protected boolean started;
  protected boolean shutdown;

  protected boolean setupRequest;
  protected boolean contextRenewalRequest;

  protected boolean resizeRequest;
  protected int viewportWidth;
  protected int viewportHeight;

  protected boolean startRequest;
  protected int startReason;

  protected int drawCount;

  public CinderRenderer(CinderBridge bridge)
  {
    cinderBridge = bridge;
  }

  @Override
  public void onSurfaceCreated(GL10 gl, EGLConfig config)
  {
    Utils.LOGD("CinderRenderer.onSurfaceCreated");
  }

  @Override
  public void onSurfaceChanged(GL10 gl, int width, int height)
  {
    Utils.LOGD("CinderRenderer.onSurfaceChanged: " + width + "x" + height);

    /*
     * IT IS IMPERATIVE TO CALL glViewport() UPON EACH onSurfaceChanged()
     * EVEN IF IT SEEMS THAT THE LATTER IS CALLED TOO OFTEN BY THE SYSTEM
     *
     * OTHERWISE: THE GLView WILL BE DEFORMED IN SOME SITUATIONS:
     * E.G. RETURNING FROM SLEEP - DURING WHICH THE ORIENTATION HAS CHANGED
     */

    gl.glViewport(0, 0, width, height);

    resizeRequest = true;
    viewportWidth = width;
    viewportHeight = height;

    // ---

    if (!setup)
    {
      setupRequest = true;
    }
  }

  @Override
  public void onDrawFrame(GL10 gl)
  {
    if (attached && !paused && !hidden)
    {
      if (contextRenewalRequest)
      {
        contextRenewalRequest = false;
        contextRenewed();

        resizeRequest = true; // ALREADY TRUE IN PRINCIPLE
      }

      if (setupRequest)
      {
        setupRequest = false;
        performSetup(viewportWidth, viewportHeight);
      }

      if (resizeRequest)
      {
        resizeRequest = false;
        performResize(viewportWidth, viewportHeight);
      }

      if (startRequest)
      {
        startRequest = false;
        performStart(startReason);

        drawCount = 0;
      }

      if (started)
      {
        performDraw();
      }
    }
  }

  // ---------------------------------------- LIFE-CYCLE ----------------------------------------

  protected void performSetup(int width, int height)
  {
    if (!setup)
    {
      Utils.LOGD("CinderRenderer.performSetup: " + width + "x" + height);
      setup(width, height);

      setup = true;
    }
  }

  protected void performResize(int width, int height)
  {
      Utils.LOGD("CinderRenderer.performResize: " + width + "x" + height);
      resize(width, height);
  }

  protected void performDraw()
  {
    if (drawCount++ == 0)
    {
      Utils.LOGD("CinderRenderer.performDraw");
    }

    draw();
  }

  protected void performStart(int reason)
  {
    start(reason);
    started = true;
  }
  
  protected void performStop(int reason)
  {
    stop(reason);
    started = false;
  }

  protected void requestStart(int reason)
  {
    startRequest = true;
    startReason = reason;
  }

  // ---

  protected void start(int reason)
  {
    Utils.LOGD("CinderRenderer.start: " + (reason == CinderBridge.START_REASON_APP_RESUMED ? "RESUMED" : "SHOWN"));

    dispatchEvent(reason);
    cinderBridge.sketchDidStart(CinderBridge.THREAD_RENDERER, reason);
  }

  protected void stop(int reason)
  {
    Utils.LOGD("CinderRenderer.stop: " + (reason == CinderBridge.STOP_REASON_APP_PAUSED ? "PAUSED" : "HIDDEN"));

    dispatchEvent(reason);
    cinderBridge.sketchDidStop(CinderBridge.THREAD_RENDERER, reason);
  }

  protected void contextLost()
  {
    Utils.LOGD("CinderRenderer.contextLost");
    dispatchEvent(EVENT_CONTEXT_LOST);
  }

  protected void contextRenewed()
  {
    Utils.LOGD("CinderRenderer.contextRenewed");
    dispatchEvent(EVENT_CONTEXT_RENEWED);
  }

  protected void foreground()
  {
    Utils.LOGD("CinderRenderer.foreground");
    dispatchEvent(EVENT_FOREGROUND);
  }

  protected void background()
  {
    Utils.LOGD("CinderRenderer.background");
    dispatchEvent(EVENT_BACKGROUND);
  }

  // ---------------------------------------- POSTED TO THE RENDERER'S THREAD FROM GLView ----------------------------------------

  public void contextCreated()
  {
    if (setup)
    {
      contextRenewalRequest = true;  
    }
  }

  public void contextDestroyed()
  {
    if (setup)
    {
      contextLost();
    }
  }

  public void performLaunch()
  {
    if (!launched)
    {
      Utils.LOGD("CinderRenderer.performLaunch");

      launch();
      cinderBridge.sketchDidLaunch(CinderBridge.THREAD_RENDERER);

      launched = true;
    }
  }

  public void performShutdown()
  {
    if (setup && !shutdown)
    {
      Utils.LOGD("CinderRenderer.performShutdown");

      shutdown();
      cinderBridge.sketchDidShutdown(CinderBridge.THREAD_RENDERER); // REASON: DETACHED FROM WINDOW

      shutdown = true;
    }
  }

  public void attachedToWindow()
  {
    Utils.LOGD("CinderRenderer.attachedToWindow");

    attached = true;

    if (!hidden)
    {
      requestStart(CinderBridge.START_REASON_VIEW_SHOWN); // REASON: ATTACHED TO WINDOW
    }
  }

  public void detachedFromWindow()
  {
    Utils.LOGD("CinderRenderer.detachedFromWindow");

    attached = false;

    if (!paused && !hidden)
    {
      performStop(CinderBridge.STOP_REASON_VIEW_HIDDEN); // REASON: DETACHED FROM WINDOW
    }
  }

  public void resumed()
  {
    Utils.LOGD("CinderRenderer.resumed");

    if (hidden)
    {
      foreground(); // TODO: TEST
    }
    else
    {
      paused = false;
      requestStart(CinderBridge.START_REASON_APP_RESUMED); // REASON: APP RESUMED
    }
  }

  public void paused()
  {
    Utils.LOGD("CinderRenderer.paused");

    if (hidden)
    {
      background(); // TODO: TEST
    }
    else
    {
      paused = true;
      performStop(CinderBridge.STOP_REASON_APP_PAUSED); // REASON: APP RESUMED
    }
  }

  public void visibilityChanged(int visibility)
  {
    Utils.LOGD("CinderRenderer.visibilityChanged: " + visibility);

    switch (visibility)
    {
      case View.VISIBLE:
      {
        hidden = false;
        requestStart(CinderBridge.START_REASON_VIEW_SHOWN); // REASON: VIEW SHOWN
        break;
      }

      case View.GONE:
      case View.INVISIBLE: // WARNING: THIS ONE USED TO TRIGGER SOFTWARE-RENDERING ON OLDER SYSTEMS (E.G. XOOM 1, HONEYCOMB)
      {
        hidden = true;
        performStop(CinderBridge.STOP_REASON_VIEW_HIDDEN); // REASON: VIEW HIDDEN
        break;
      }
    }
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

  protected native void launch();
  protected native void setup(int width, int height);
  protected native void shutdown();

  protected native void resize(int width, int height);
  protected native void draw();

  protected native void addTouch(int index, float x, float y);
  protected native void updateTouch(int index, float x, float y);
  protected native void removeTouch(int index, float x, float y);

  public native void dispatchEvent(int eventId);
}
