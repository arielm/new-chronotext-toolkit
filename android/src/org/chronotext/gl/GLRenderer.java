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
  protected boolean showRequest;

  public void onSurfaceCreated(GL10 gl, EGLConfig config)
  {
    Log.i("CHR", "*** GLRenderer.onSurfaceCreated ***");
  }

  public void onSurfaceChanged(GL10 gl, int w, int h)
  {
    Log.i("CHR", "*** GLRenderer.onSurfaceChanged: " + w + "x" + h + " ***");

    /*
     * WARNING:
     * THIS CALLBACK IS CALLED FAR TOO MUCH BY THE SYSTEM, AND WITH INCONSISTENT VALUES
     * THE LOGIC USED HERE IS INTENDED TO AVOID POTENTIALLY HARMFUL CONSEQUENCES
     */

    if (!initialized)
    {
      initialized = true;

      launch();
      setup(gl, w, h);

      ticks = 0;
      attached = true;
      attached();
    }
  }

  public void onDrawFrame(GL10 gl)
  {
    if (resumeRequest)
    {
        resumeRequest = false;

        ticks = 0;
        resumed = true;
        resumed(true);
        hidden = false; // TODO: TEST
    }

    if (showRequest)
    {
      showRequest = false;

      ticks = 0;
      shown(); // TODO: TEST
      hidden = false; // TODO: TEST
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

  // ---------------------------------------- QUEUED EVENTS, INITIALLY RECEIVED ON THE UI-THREAD ----------------------------------------

  public void onAttachedToWindow()
  {
    Log.i("CHR", "*** GLRenderer.onAttachedToWindow ***");

    if (initialized && !resumed)
    {
      ticks = 0;
      attached = true;
      attached(); // TODO: TEST
      hidden = false; // TODO: TEST
    }
  }

  public void onDetachedFromWindow()
  {
    Log.i("CHR", "*** GLRenderer.onDetachedFromWindow ***");
    
    if (resumed && !hidden)
    {
      attached = false;
      detached(); // TODO: TEST
      hidden = false; // TODO: TEST
    }
  }

  public void onVisibilityChanged(int visibility)
  {
    Log.i("CHR", "*** GLRenderer.onVisibilityChanged: " + visibility + " ***");

    switch (visibility)
    {
      case View.VISIBLE :
      {
        /*
         * AT THIS STAGE (IN CASE THE APP WAS PREVIOUSLY IN THE BACKGROUND), THE SURFACE IS "NOT READY" YET
         * THEREFORE shown() IS CALLED onDrawFrame()
         */
        showRequest = true; // TODO: TEST
        break;
      }

      case View.GONE :
      {
        hidden(); // TODO: TEST
        hidden = true; // TODO: TEST
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
        /*
         * AT THIS STAGE, THE SURFACE MAY NOT HAVE BEEN CREATED YET
         * THEREFORE resumed() IS CALLED onDrawFrame()
         */
         resumeRequest = true;
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
        Log.i("CHR", "AVERAGE FRAME-RATE: " + ticks / (elapsed / 1000f) + " FRAMES PER SECOND");

        /*
         * AT THIS STAGE, THE SURFACE HAS ALREADY BEEN DESTROYED,
         * I.E. UNLOADING TEXTURES WILL BE A NO-OP...
         */
        resumed = false;
        paused(true);
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

  public abstract void launch();

  public abstract void setup(GL10 gl, int width, int height);
  public abstract void shutdown();
  public abstract void draw(GL10 gl);

  public abstract void attached();
  public abstract void detached();

  public abstract void paused(boolean contextLost);
  public abstract void resumed(boolean contextRenewed);

  public abstract void background();
  public abstract void foreground();

  public abstract void shown();
  public abstract void hidden();

  public abstract void addTouches(Vector<Touch> touches);
  public abstract void updateTouches(Vector<Touch> touches);
  public abstract void removeTouches(Vector<Touch> touches);

  public abstract void sendMessage(int what, String body);
}
