
package chronotext.android.gl;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.opengl.GLSurfaceView;
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
    /*
     * WE DON'T CALL setup() FROM HERE BECAUSE WE WANT TO KNOW THE SURFACE-SIZE FIRST
     */

    if (initialized)
    {
      resumed();
    }
    else
    {
      launch();
    }
  }

  public void onSurfaceChanged(GL10 gl, int w, int h)
  {
    if (!initialized)
    {
      setup(gl, w, h);
    }

    if (!attached)
    {
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
      shown();
      showRequest = false;
    }
    
    // ---

    draw(gl);
  }

  // ---------------------------------------- QUEUED EVENTS, INITIALLY RECEIVED ON THE UI-THREAD ----------------------------------------

  public void onAttachedToWindow()
  {
    ticks = 0;

    if (initialized && !resumed)
    {
      attached();
    }
  }

  public void onDetachedFromWindow()
  {
    if (resumed && !hidden)
    {
      detached();
    }
  }

  public void onVisibilityChanged(int visibility)
  {
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
        hidden();
        break;
      }
    }
  }

  public void onResume()
  {
    if (attached)
    {
      ticks = 0;

      if (hidden)
      {
        foreground();
      }
      else
      {
        /*
         * AT THIS STAGE, THE SURFACE HAS NOT BEEN RE-CREATED YET
         * SO, WE DON'T CALL resumed() HERE BUT IN onSurfaceCreated()
         */
      }
    }
  }

  public void onPause()
  {
    if (attached)
    {
      if (!hidden)
      {
        System.out.printf("AVERAGE FRAME-RATE: %f FRAMES PER SECOND\n", ticks / (elapsed / 1000f));

        /*
         * AT THIS STAGE, THE SURFACE HAS BEEN ALREADY DESTROYED,
         * I.E. UNLOADING TEXTURES WILL BE A NO-OP...
         */
        paused();
      }
      else
      {
        background();
      }
    }
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

  public abstract void addTouch(float x, float y);

  public abstract void updateTouch(float x, float y);

  public abstract void removeTouch(float x, float y);

  public abstract void sendMessage(int what, String body);
}
