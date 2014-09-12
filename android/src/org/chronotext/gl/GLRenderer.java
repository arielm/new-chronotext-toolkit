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
  protected boolean attached;
  protected boolean resumed;
  protected boolean hidden;
  
  protected boolean showRequest;

  public void onSurfaceCreated(GL10 gl, EGLConfig config)
  {
    Log.i("CHR", "*** GLRenderer.onSurfaceCreated ***");

    /*
     * WE DON'T CALL setup() FROM HERE BECAUSE WE WANT TO KNOW THE SURFACE-SIZE FIRST
     */

    if (!initialized)
    {
      Log.i("CHR", "*** CinderRenderer.launch ***");
      launch();
    }
  }

  public void onSurfaceChanged(GL10 gl, int w, int h)
  {
    Log.i("CHR", "*** GLRenderer.onSurfaceChanged: " + w + "x" + h + "***");

    /*
     * WARNING:
     * THIS CALLBACK IS CALLED FAR TOO MUCH BY THE SYSTEM, AND WITH INCONSISTENT VALUES
     * THE LOGIC USED HERE SEEMS TO FIX ALL THE RELATED ISSUES
     */

    if (initialized)
    {
      if (!resumed)
      {
        Log.i("CHR", "*** CinderRenderer.resumed ***");
        resumed();
      }
    }
    else
    {
      Log.i("CHR", "*** CinderRenderer.setup: " + w + "x" + h + " ***");
      setup(gl, w, h);
    }

    if (!attached)
    {
      Log.i("CHR", "*** CinderRenderer.attached ***");
      attached();
    }
  }

  public void onDrawFrame(GL10 gl)
  {
    now = System.currentTimeMillis();

    if (ticks == 0)
    {
      t0 = now;
    }

    ticks++;
    elapsed = now - t0;

    // ---
    
    if (showRequest)
    {
      Log.i("CHR", "*** CinderRenderer.shown ***");
      shown();
      showRequest = false;
    }
    
    // ---

    draw(gl);
  }

  // ---------------------------------------- QUEUED EVENTS, INITIALLY RECEIVED ON THE UI-THREAD ----------------------------------------

  public void onAttachedToWindow()
  {
    Log.i("CHR", "*** CinderRenderer.onAttachedToWindow ***");

    ticks = 0;

    if (initialized && !resumed)
    {
      Log.i("CHR", "*** CinderRenderer.attached ***");
      attached();
    }
  }

  public void onDetachedFromWindow()
  {
    Log.i("CHR", "*** CinderRenderer.onAttachedToWindow ***");
    
    if (resumed && !hidden)
    {
      Log.i("CHR", "*** CinderRenderer.detached ***");
      detached();
    }
  }

  public void onVisibilityChanged(int visibility)
  {
    Log.i("CHR", "*** GLRenderer.onVisibilityChanged: " + visibility + " ***");

    switch (visibility)
    {
      case View.VISIBLE :
      {
        ticks = 0;
        
        /*
         * AT THIS STAGE (IN CASE THE APP WAS PREVIOUSLY IN THE BACKGROUND), THE SURFACE IS "NOT READY" YET
         * SO, WE DON'T CALL shown() HERE BUT IN onDraw()
         */
        showRequest = true;
        break;
      }

      case View.GONE :
      {
        Log.i("CHR", "*** CinderRenderer.hidden ***");
        hidden();
        break;
      }
    }
  }

  public void onResume()
  {
    Log.i("CHR", "*** GLRenderer.onResume ***");

    if (attached)
    {
      ticks = 0;

      if (hidden)
      {
        Log.i("CHR", "*** CinderRenderer.foreground ***");
        foreground();
      }
      else
      {
        /*
         * AT THIS STAGE, THE SURFACE HAS NOT BEEN RE-CREATED YET
         * SO, WE DON'T CALL resumed() HERE BUT IN onSurfaceChanged()
         */
      }
    }
  }

  public void onPause()
  {
    Log.i("CHR", "*** GLRenderer.onPause ***");

    if (attached)
    {
      if (!hidden)
      {
        Log.i("CHR", "AVERAGE FRAME-RATE: " + ticks / (elapsed / 1000f) + " FRAMES PER SECOND");

        /*
         * AT THIS STAGE, THE SURFACE HAS BEEN ALREADY DESTROYED,
         * I.E. UNLOADING TEXTURES WILL BE A NO-OP...
         */
        Log.i("CHR", "*** CinderRenderer.paused ***");
        paused();
      }
      else
      {
        Log.i("CHR", "*** CinderRenderer.background ***");
        background();
      }
    }
  }

  public void onWindowFocusChanged(boolean hasFocus)
  {
    Log.i("CHR", "*** CinderRenderer.onWindowFocusChanged: " + hasFocus + " ***");
    // TODO: SET hasFocus FLAG AND USE IT FOR "PROPERLY RESUMING" WHEN BACK-FROM-SLEEP
  }

  /*
   * THIS IS RELATED TO APPLICATION-DESTRUCTION (I.E. NOT SURFACE-DESTRUCTION)
   */
  public void onDestroy()
  {
    shutdown();
  }

  // ---------------------------------------- ABSTRACT METHODS ----------------------------------------

  public abstract void launch();

  public abstract void setup(GL10 gl, int width, int height);
  public abstract void shutdown();
  public abstract void draw(GL10 gl);

  public abstract void attached();
  public abstract void detached();

  public abstract void paused();
  public abstract void resumed();

  public abstract void background();
  public abstract void foreground();

  public abstract void shown();
  public abstract void hidden();

  public abstract void addTouches(Vector<Touch> touches);
  public abstract void updateTouches(Vector<Touch> touches);
  public abstract void removeTouches(Vector<Touch> touches);

  public abstract void sendMessage(int what, String body);
}
