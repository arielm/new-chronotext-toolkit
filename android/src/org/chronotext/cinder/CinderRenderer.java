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

import org.chronotext.cinder.Touch;
import org.chronotext.utils.Utils;

import android.opengl.GLSurfaceView;
import android.view.View;

public class CinderRenderer implements GLSurfaceView.Renderer
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

  public static final int REASON_RESUMED = 1;
  public static final int REASON_SHOWN = 2;
  public static final int REASON_PAUSED = 3;
  public static final int REASON_HIDDEN = 4;

  protected boolean launched;
  protected boolean initialized;
  protected boolean attached;
  protected boolean paused;
  protected boolean hidden;
  protected boolean started;

  protected boolean setupRequest;
  protected boolean contextRenewalRequest;

  protected boolean resizeRequest;
  protected int viewportWidth;
  protected int viewportHeight;

  protected boolean startRequest;
  protected int startReason;

  protected int ticks;
  protected long t0;
  protected long now;
  protected long elapsed;

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

    if (!initialized)
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
        performStart(startReason);
      }

      if (started)
      {
        now = System.currentTimeMillis();

        if (startRequest)
        {
          ticks = 0;
          t0 = now;
        }

        ticks++;
        elapsed = now - t0;

        // ---

        performDraw();
      }
    }

    startRequest = false; // PURPOSELY AT THE END
  }

  // ---------------------------------------- LIFE-CYCLE ----------------------------------------

  protected void performSetup(int width, int height)
  {
    if (!initialized)
    {
      Utils.LOGD("CinderRenderer.setup: " + width + "x" + height);
      setup(width, height);

      initialized = true;
    }
  }

  protected void performResize(int width, int height)
  {
      Utils.LOGD("CinderRenderer.resize: " + width + "x" + height);
      resize(width, height);
  }

  protected void performDraw()
  {
    if (ticks == 1)
    {
      Utils.LOGD("CinderRenderer.draw");
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
    Utils.LOGD("AVERAGE FRAME-RATE: " + ticks / (elapsed / 1000f) + " FRAMES PER SECOND");

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
    Utils.LOGD("CinderRenderer.start: " + (reason == REASON_RESUMED ? "RESUMED" : "SHOWN"));
    event(reason);
  }

  protected void stop(int reason)
  {
    Utils.LOGD("CinderRenderer.stop: " + (reason == REASON_PAUSED ? "PAUSED" : "HIDDEN"));
    event(reason);
  }

  protected void contextLost()
  {
    Utils.LOGD("CinderRenderer.contextLost");
    event(EVENT_CONTEXT_LOST);
  }

  protected void contextRenewed()
  {
    Utils.LOGD("CinderRenderer.contextRenewed");
    event(EVENT_CONTEXT_RENEWED);
  }

  protected void foreground()
  {
    Utils.LOGD("CinderRenderer.foreground");
    event(EVENT_FOREGROUND);
  }

  protected void background()
  {
    Utils.LOGD("CinderRenderer.background");
    event(EVENT_BACKGROUND);
  }

  // ---------------------------------------- INVOKED ON THE RENDERER'S THREAD FROM GLView ----------------------------------------

  public void contextCreated()
  {
    if (initialized)
    {
      contextRenewalRequest = true;  
    }
  }

  public void contextDestroyed()
  {
    if (initialized)
    {
      contextLost();
    }
  }

  public void performLaunch()
  {
    if (!launched)
    {
      Utils.LOGD("CinderRenderer.launch");
      launch();

      launched = true;
    }
  }

  public void performShutdown()
  {
    if (initialized)
    {
      Utils.LOGD("CinderRenderer.shutdown");
      shutdown();
    }
  }

  public void attachedToWindow()
  {
    Utils.LOGD("CinderRenderer.attachedToWindow");

    attached = true;

    if (!hidden)
    {
      requestStart(REASON_SHOWN);
    }
  }

  public void detachedFromWindow()
  {
    Utils.LOGD("CinderRenderer.detachedFromWindow");

    attached = false;

    if (!paused && !hidden)
    {
      performStop(REASON_HIDDEN);
    }
  }

  public void resumed()
  {
    Utils.LOGD("CinderRenderer.resumed");

    if (hidden)
    {
      foreground();
    }
    else
    {
      paused = false;
      requestStart(REASON_RESUMED);
    }
  }

  public void paused()
  {
    Utils.LOGD("CinderRenderer.paused");

    if (hidden)
    {
      background();
    }
    else
    {
      paused = true;
      performStop(REASON_PAUSED);
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
        requestStart(REASON_SHOWN);
        break;
      }

      case View.GONE:
      case View.INVISIBLE: // WARNING: THIS ONE SEEMS TO TRIGGER SOFTWARE-RENDERING ON OLDER SYSTEMS (E.G. XOOM 1 V3.1)
      {
        hidden = true;
        performStop(REASON_HIDDEN);
        break;
      }
    }
  }

  // ---

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

  public native void launch();
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
