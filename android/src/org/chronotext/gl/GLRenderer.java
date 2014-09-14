/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

package org.chronotext.gl;

import java.util.Vector;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import org.chronotext.gl.Touch;

import android.opengl.GLSurfaceView;
import android.util.Log;
import android.view.View;

public abstract class GLRenderer implements GLSurfaceView.Renderer
{
  public static final int REASON_RESUMED = 1;
  public static final int REASON_ATTACHED = 2;
  public static final int REASON_SHOWN = 3;
  public static final int REASON_PAUSED = 4;
  public static final int REASON_DETACHED = 5;
  public static final int REASON_HIDDEN = 6;

  protected int ticks;
  protected long t0;
  protected long now;
  protected long elapsed;

  protected boolean initialized;
  protected boolean paused;
  protected boolean attached;
  protected boolean hidden;

  protected boolean startRequest;
  protected int startReason;

  protected boolean contextRenewalRequest;

  public void onSurfaceCreated(GL10 gl, EGLConfig config)
  {
    Log.i("CHR", "*** GLRenderer.onSurfaceCreated ***");
  }

  /*
   * WARNING: THIS CALLBACK IS CALLED FAR TOO MUCH BY THE SYSTEM
   */
  public void onSurfaceChanged(GL10 gl, int w, int h)
  {
    Log.i("CHR", "*** GLRenderer.onSurfaceChanged: " + w + "x" + h + " ***");

    if (!initialized)
    {
      initialized = true;

      setup(gl, w, h);
      attach();
    }
  }

  public void onDrawFrame(GL10 gl)
  {
    if (startRequest)
    {
      startRequest = false;
      performStart(startReason);
    }
    
    // ---

    now = System.currentTimeMillis();

    if (ticks == 0)
    {
      Log.i("CHR", "*** GLRenderer.onDrawFrame ***");
      t0 = now;
    }

    ticks++;
    elapsed = now - t0;

    // ---

    draw(gl);
  }

  // ---------------------------------------- LIFE-CYCLE ----------------------------------------

  protected void resume()
  {
    paused = false;
    requestStart(REASON_RESUMED);
  }

  protected void pause()
  {
    paused = true;
    performStop(REASON_PAUSED);
  }

  protected void attach() // TODO: TEST
  {
    attached = true;
    requestStart(REASON_ATTACHED);
  }

  protected void detach() // TODO: TEST
  {
    attached = false;
    performStop(REASON_DETACHED);
  }

  protected void show()
  {
    hidden = false;
    requestStart(REASON_SHOWN);
  }

  protected void hide()
  {
    hidden = true;
    performStop(REASON_HIDDEN);
  }

  protected void requestStart(int reason)
  {
    startRequest = true;
    startReason = reason;
  }

  protected void performStart(int reason)
  {
    ticks = 0;

    if (contextRenewalRequest)
    {
      contextRenewalRequest = false;
      contextRenewed();
    }

    start(reason);
  }

  protected void performStop(int reason)
  {
    Log.i("CHR", "AVERAGE FRAME-RATE: " + ticks / (elapsed / 1000f) + " FRAMES PER SECOND");
    stop(reason);    
  }

  protected void handleContextLoss()
  {
    contextRenewalRequest = true;
    contextLost();
  }

  // ---------------------------------------- QUEUED EVENTS, INITIALLY RECEIVED ON THE UI-THREAD ----------------------------------------

  public void onAttachedToWindow()
  {
    Log.i("CHR", "*** GLRenderer.onAttachedToWindow ***");

    if (initialized && !paused)
    {
      attach();
    }
  }

  /*
   * PROBLEM 1: THIS CAN'T BE REACHED, AS DESCRIBED IN GLView.onDetachedFromWindow()
   */
  public void onDetachedFromWindow()
  {
    Log.i("CHR", "*** GLRenderer.onDetachedFromWindow ***");
    
    if (!paused && !hidden)
    {
      detach(); // PROBLEM 2: WHENEVER REACHED, WE ALSO NEED TO HANDLE GL-CONTEXT-LOSS
    }
  }

  public void onVisibilityChanged(int visibility)
  {
    Log.i("CHR", "*** GLRenderer.onVisibilityChanged: " + visibility + " ***");

    switch (visibility)
    {
      case View.VISIBLE:
      {
        show();
        break;
      }

      case View.GONE:
      case View.INVISIBLE: // WARNING: THIS ONE SEEMS TO TRIGGER SOFTWARE-RENDERING ON OLDER SYSTEMS (E.G. XOOM 1 V3.1)
      {
        hide();
        break;
      }
    }
  }

  public void onResume()
  {
    Log.i("CHR", "*** GLRenderer.onResume ***");

    if (attached)
    {
      if (hidden)
      {
        foreground();
      }
      else
      {
         resume();
      }
    }
  }

  public void onPause()
  {
    Log.i("CHR", "*** GLRenderer.onPause ***");

    if (attached)
    {
      handleContextLoss(); // ASSERTION: GLSurfaceView HAS DESTROYED ITS GL-CONTEXT

      if (hidden)
      {
        background();
      }
      else
      {
        pause();
      }
    }
  }

  /*
   * THIS IS RELATED TO APPLICATION-DESTRUCTION (I.E. NOT SURFACE-DESTRUCTION)
   */
  public void onDestroy()
  {
    Log.i("CHR", "*** GLRenderer.onDestroy ***");
    shutdown();
  }

  // ---------------------------------------- ABSTRACT METHODS ----------------------------------------

  public abstract void setup(GL10 gl, int width, int height);
  public abstract void shutdown();
  public abstract void draw(GL10 gl);

  public abstract void start(int reason);
  public abstract void stop(int reason);

  public abstract void foreground();
  public abstract void background();

  public abstract void contextLost();
  public abstract void contextRenewed();

  public abstract void addTouches(Vector<Touch> touches);
  public abstract void updateTouches(Vector<Touch> touches);
  public abstract void removeTouches(Vector<Touch> touches);

  public abstract void sendMessage(int what, String body);
}
