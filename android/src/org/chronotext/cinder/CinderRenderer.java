/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2015, ARIEL MALKA ALL RIGHTS RESERVED.
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

  protected boolean setup;
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

  // ---------------------------------------- INTERNAL LIFE-CYCLE ----------------------------------------

  protected void performSetup(int width, int height)
  {
    if (!setup)
    {
      Utils.LOGD("CinderRenderer.performSetup: " + width + "x" + height);

      cinderBridge.dispatchEvent(CinderBridge.SKETCH_WILL_SETUP);
      setup(width, height);
      cinderBridge.dispatchEvent(CinderBridge.SKETCH_DID_SETUP);

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
    switch (reason)
    {
      case CinderBridge.VIEW_WILL_APPEAR:
      {
        Utils.LOGD("CinderRenderer.performStart: SHOWN");
        dispatchEvent(CinderRenderer.EVENT_SHOWN);
        break;
      }

      case CinderBridge.APP_DID_RESUME:
      {
        Utils.LOGD("CinderRenderer.performStart: RESUMED");
        dispatchEvent(CinderRenderer.EVENT_RESUMED);
        break;
      }
    }

    started = true;
  }
  
  protected void performStop(int reason)
  {
    switch (reason)
    {
      case CinderBridge.VIEW_WILL_DISAPPEAR:
      {
        Utils.LOGD("CinderRenderer.performStop: HIDDEN");
        dispatchEvent(CinderRenderer.EVENT_HIDDEN);
        break;
      }

      case CinderBridge.APP_WILL_PAUSE:
      {
        Utils.LOGD("CinderRenderer.performStop: PAUSED");
        dispatchEvent(CinderRenderer.EVENT_PAUSED);
        break;
      }
    }

    started = false;
  }

  protected void requestStart(int reason)
  {
    startRequest = true;
    startReason = reason;
  }

  // ---

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

  // ---------------------------------------- POSTED TO (OR INVOKED ON) THE RENDERER'S THREAD FROM GLView ----------------------------------------

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

  /*
   * TODO: CONSIDER MERGING WITH CinderRenderer.detachedFromWindow()
   * SEE ALSO: GLView.CustomContextFactory.destroyContext()
   */
  public void performShutdown()
  {
    if (setup)
    {
      Utils.LOGD("CinderRenderer.performShutdown");

      cinderBridge.dispatchEvent(CinderBridge.SKETCH_WILL_SHUTDOWN);
      shutdown();
      cinderBridge.dispatchEvent(CinderBridge.SKETCH_DID_SHUTDOWN);

      setup = false;
    }
  }

  public void attachedToWindow()
  {
    if (!attached)
    {
      Utils.LOGD("CinderRenderer.attachedToWindow");

      attached = true;

      if (!hidden)
      {
        requestStart(CinderBridge.VIEW_WILL_APPEAR); // REASON: ATTACHED TO WINDOW
      }
    }
  }

  /*
   * TODO: CONSIDER MERGING WITH CinderRenderer.performShutdown()
   * SEE ALSO: GLView.CustomContextFactory.destroyContext()
   */
  public void detachedFromWindow()
  {
    if (attached)
    {
      Utils.LOGD("CinderRenderer.detachedFromWindow");

      attached = false;

      if (!paused && !hidden)
      {
        performStop(CinderBridge.VIEW_WILL_DISAPPEAR); // REASON: DETACHED FROM WINDOW
      }
    } 
  }

  public void resumed()
  {
    if (attached)
    {
      Utils.LOGD("CinderRenderer.resumed");

      if (hidden)
      {
        foreground();
      }
      else
      {
        paused = false;
        requestStart(CinderBridge.APP_DID_RESUME); // REASON: APP RESUMED
      }
    }
  }

  public void paused()
  {
    if (attached)
    {
      Utils.LOGD("CinderRenderer.paused");

      if (hidden)
      {
        background();
      }
      else
      {
        paused = true;
        performStop(CinderBridge.APP_WILL_PAUSE); // REASON: APP PAUSED
      }      
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

        if (attached)
        {
          requestStart(CinderBridge.VIEW_WILL_APPEAR); // REASON: VIEW SHOWN  
        }
        
        break;
      }

      case View.GONE:
      case View.INVISIBLE: // WARNING: THIS ONE USED TO TRIGGER SOFTWARE-RENDERING ON OLDER SYSTEMS (E.G. XOOM 1, HONEYCOMB)
      {
        hidden = true;

        if (attached)
        {
          performStop(CinderBridge.VIEW_WILL_DISAPPEAR); // REASON: VIEW HIDDEN  
        }
        
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

  protected native void setup(int width, int height);
  protected native void shutdown();

  protected native void resize(int width, int height);
  protected native void draw();

  protected native void addTouch(int index, float x, float y);
  protected native void updateTouch(int index, float x, float y);
  protected native void removeTouch(int index, float x, float y);

  public native void dispatchEvent(int eventId);
}
