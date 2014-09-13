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
  protected int ticks;
  protected long t0;
  protected long now;
  protected long elapsed;

  protected boolean initialized;
  protected boolean resumed;
  protected boolean attached;
  protected boolean hidden;

  protected boolean resumeRequest;
  protected boolean attachRequest;
  protected boolean showRequest;

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
    if (resumeRequest)
    {
      resumeRequest = false;

      start();
      resumed(true); // XXX
    }
    else if (attachRequest)
    {
      attachRequest = false;

      start();
      attached();
    }
    else if (showRequest)
    {
      showRequest = false;

      start();
      shown();
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
    resumed = true;
    resumeRequest = true;
  }

  protected void pause()
  {
    resumed = false;

    stop();
    paused(true); // XXX
  }

  protected void attach() // TODO: TEST
  {
    attached = true;
    attachRequest = true;
  }

  protected void detach() // TODO: TEST
  {
    attached = false;

    stop();
    detached();
  }

  protected void show() // TODO: TEST
  {
    hidden = false;
    showRequest = true;
  }

  protected void hide() // TODO: TEST
  {
    hidden = true;

    stop();
    hidden();
  }

  protected void start()
  {
    ticks = 0;
  }

  protected void stop()
  {
    Log.i("CHR", "AVERAGE FRAME-RATE: " + ticks / (elapsed / 1000f) + " FRAMES PER SECOND");
  }

  // ---------------------------------------- QUEUED EVENTS, INITIALLY RECEIVED ON THE UI-THREAD ----------------------------------------

  public void onAttachedToWindow()
  {
    Log.i("CHR", "*** GLRenderer.onAttachedToWindow ***");

    if (initialized && !resumed)
    {
      attach();
    }
  }

  public void onDetachedFromWindow()
  {
    Log.i("CHR", "*** GLRenderer.onDetachedFromWindow ***");
    
    if (resumed && !hidden)
    {
      detach();
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
      {
        hide();
        break;
      }

      case View.INVISIBLE:
      {
        /*
         * WARNING: View.INVISIBLE SEEMS TO TRIGGER SOFTWARE-RENDERING ON OLDER SYSTEMS (E.G. XOOM 1 V3.1)
         */
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
        foreground(); // TODO: TEST
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
      if (hidden)
      {
        background(); // TODO: TEST
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

  public abstract void resumed(boolean contextRenewed);
  public abstract void paused(boolean contextLost);

  public abstract void attached();
  public abstract void detached();

  public abstract void shown();
  public abstract void hidden();

  public abstract void background();
  public abstract void foreground();

  public abstract void addTouches(Vector<Touch> touches);
  public abstract void updateTouches(Vector<Touch> touches);
  public abstract void removeTouches(Vector<Touch> touches);

  public abstract void sendMessage(int what, String body);
}
