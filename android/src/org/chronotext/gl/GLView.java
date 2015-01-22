/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

package org.chronotext.gl;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;

import java.util.Vector;

import org.chronotext.cinder.CinderBridge;
import org.chronotext.cinder.CinderRenderer;
import org.chronotext.cinder.Touch;
import org.chronotext.utils.Utils;

import android.content.Context;
import android.graphics.PixelFormat;
import android.opengl.GLSurfaceView;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.View;

public class GLView extends GLSurfaceView
{
  public static class Properties
  {
    int eglContextClientVersion = 1;
    int[] eglConfigChooser;
    int pixelFormat = PixelFormat.UNKNOWN;
    boolean preserveEGLContextOnPause = true;

    public Properties setEGLContextClientVersion(int version)
    {
      eglContextClientVersion = version;
      return this;
    }

    public Properties setEGLConfigChooser(int redSize, int greenSize, int blueSize, int alphaSize, int depthSize, int stencilSize)
    {
      eglConfigChooser = new int[] {redSize, greenSize, blueSize, alphaSize, depthSize, stencilSize};
      return this;
    }

    public Properties setPixelFormat(int format)
    {
      pixelFormat = format;
      return this;
    }

    public Properties setPreserveEGLContextOnPause(boolean preserveOnPause)
    {
      preserveEGLContextOnPause = preserveOnPause;
      return this;
    }
  }

  // ---
  
  protected CinderBridge cinderBridge;
  protected CinderRenderer cinderRenderer;

  protected boolean attached;
  protected boolean paused;
  protected boolean hidden;
  protected boolean finishing;
  protected boolean shutdown;

  public GLView(Context context, CinderBridge bridge, Properties properties)
  {
    super(context);

    cinderBridge = bridge;

    // ---

    setEGLContextClientVersion(properties.eglContextClientVersion);
    setEGLContextFactory(new CustomContextFactory(properties.eglContextClientVersion));

    if (properties.eglConfigChooser != null)
    {
      setEGLConfigChooser(
        properties.eglConfigChooser[0],
        properties.eglConfigChooser[1],
        properties.eglConfigChooser[2],
        properties.eglConfigChooser[3],
        properties.eglConfigChooser[4],
        properties.eglConfigChooser[5]);
    }

    if (properties.pixelFormat != PixelFormat.UNKNOWN)
    {
      getHolder().setFormat(properties.pixelFormat);
    }

    setPreserveEGLContextOnPause(properties.preserveEGLContextOnPause);

    // ---

    cinderBridge.sketchWillLaunch(CinderBridge.THREAD_MAIN);

    cinderRenderer = new CinderRenderer(cinderBridge);
    setRenderer(cinderRenderer); // WILL START THE RENDERER'S THREAD

    queueEvent(new Runnable()
    {
      public void run()
      {
        cinderRenderer.performLaunch();
      }
    });
  }

  @Override
  public void surfaceCreated(SurfaceHolder holder)
  {
    Utils.LOGD("GLView.surfaceCreated"); 
    super.surfaceCreated(holder);
  }

  @Override
  public void surfaceDestroyed(SurfaceHolder holder)
  {
    Utils.LOGD("GLView.surfaceDestroyed"); 
    super.surfaceDestroyed(holder);
  }

  @Override
  public void surfaceChanged(SurfaceHolder holder, int format, int width, int height)
  {
    Utils.LOGD("GLView.surfaceChanged: " + width + "x" + height);
    super.surfaceChanged(holder, format, width, height);
  }

  @Override
  public void onWindowFocusChanged(final boolean hasFocus)
  {
    Utils.LOGD("GLView.onWindowFocusChanged: " + hasFocus);
    super.onWindowFocusChanged(hasFocus);
  }

  @Override
  protected void onAttachedToWindow()
  {
    Utils.LOGD("GLView.onAttachedToWindow");

    if (shutdown)
    {
      Utils.LOGE("GLView IS INVALID");
      return;
    }
    
    if (!attached)
    {
      attached = true;
      super.onAttachedToWindow();

      queueEvent(new Runnable()
      {
        public void run()
        {
          cinderRenderer.attachedToWindow();
        }
      });

      if (!hidden)
      {
        cinderBridge.sketchWillStart(CinderBridge.THREAD_MAIN, CinderBridge.START_REASON_VIEW_SHOWN); // TODO: TEST
      }
    }
  }

  @Override
  protected void onDetachedFromWindow()
  {
    Utils.LOGD("GLView.onDetachedFromWindow");

    if (shutdown || finishing)
    {
      return;
    }

    if (attached)
    {
      /*
       * A TRICKY-BUT-CERTAIN WAY TO COMMUNICATE WITH THE RENDERER'S THREAD BEFORE IT EXITS
       * SEE ALSO: GLView.onDestroy() AND GLView.CustomContextFactory.destroyContext()
       */
      attached = false;

      /*
       * WILL CAUSE THE RENDERER'S THREAD TO EXIT
       * (EVENTS QUEUED RIGHT BEFORE-OR-AFTER THIS WILL NEVER BE DELIVERED)
       */
      super.onDetachedFromWindow();

      if (!hidden)
      {
        cinderBridge.sketchWillStop(CinderBridge.THREAD_MAIN, CinderBridge.STOP_REASON_VIEW_HIDDEN); // TODO: TEST
      }
    }
  }

  @Override
  public void onVisibilityChanged(View changedView, final int visibility)
  {
    Utils.LOGD("GLView.visibilityChanged: " + visibility);

    if (shutdown || finishing)
    {
      return;
    }

    if (changedView == this)
    {
      switch (visibility)
      {
        case View.VISIBLE:
        {
          hidden = false;
          break;
        }

        case View.GONE:
        case View.INVISIBLE: // WARNING: THIS ONE USED TO TRIGGER SOFTWARE-RENDERING ON OLDER SYSTEMS (E.G. XOOM 1, HONEYCOMB)
        {
          hidden = true;
          break;
        }
      }

      queueEvent(new Runnable()
      {
        public void run()
        {
          cinderRenderer.visibilityChanged(visibility); // TODO: SHOULD A MORE "SPECIFIC" METHOD BE USED?
        }
      });

      if (attached)
      {
        if (hidden)
        {
          cinderBridge.sketchWillStop(CinderBridge.THREAD_MAIN, CinderBridge.STOP_REASON_VIEW_HIDDEN); // TODO: TEST
        }
        else
        {
          cinderBridge.sketchWillStart(CinderBridge.THREAD_MAIN, CinderBridge.START_REASON_VIEW_SHOWN); // TODO: TEST
        }
      }
    }
  }

  @Override
  public boolean onTouchEvent(MotionEvent event)
  {
    if (shutdown || finishing)
    {
      return false;
    }

    switch (event.getAction() & MotionEvent.ACTION_MASK)
    {
      case MotionEvent.ACTION_DOWN:
      {
        int index = 0;
        final Vector<Touch> touches = new Vector<Touch>();
        touches.add(new Touch(event.getPointerId(index), event.getX(index), event.getY(index)));

        queueEvent(new Runnable()
        {
          public void run()
          {
            cinderRenderer.addTouches(touches);
          }
        });

        break;
      }

      case MotionEvent.ACTION_POINTER_DOWN:
      {
        int index = event.getActionIndex();
        final Vector<Touch> touches = new Vector<Touch>();
        touches.add(new Touch(event.getPointerId(index), event.getX(index), event.getY(index)));

        queueEvent(new Runnable()
        {
          public void run()
          {
            cinderRenderer.addTouches(touches);
          }
        });

        break;
      }

      case MotionEvent.ACTION_UP:
      {
        int index = 0;
        final Vector<Touch> touches = new Vector<Touch>();
        touches.add(new Touch(event.getPointerId(index), event.getX(index), event.getY(index)));

        queueEvent(new Runnable()
        {
          public void run()
          {
            cinderRenderer.removeTouches(touches);
          }
        });

        break;
      }

      case MotionEvent.ACTION_POINTER_UP:
      {
        int index = event.getActionIndex();
        final Vector<Touch> touches = new Vector<Touch>();
        touches.add(new Touch(event.getPointerId(index), event.getX(index), event.getY(index)));

        queueEvent(new Runnable()
        {
          public void run()
          {
            cinderRenderer.removeTouches(touches);
          }
        });

        break;
      }

      case MotionEvent.ACTION_MOVE:
      {
        final Vector<Touch> touches = new Vector<Touch>();
          
        for (int i = 0; i < event.getPointerCount(); i++)
        {
          touches.add(new Touch(event.getPointerId(i), event.getX(i), event.getY(i)));
        }

        queueEvent(new Runnable()
        {
          public void run()
          {
            cinderRenderer.updateTouches(touches);
          }
        });

        break;
      }
    }

    return true;
  }

  // ---------------------------------------- INVOKED ON THE MAIN-THREAD BY CinderBridge ----------------------------------------

  public boolean resume()
  {
    if (!shutdown && !finishing)
    {
      if (attached && paused) // SIMPLE PROTECTION AGAINST SPURIOUS onResume() CALLS
      {
        paused = false;
        onResume();

        queueEvent(new Runnable()
        {
          public void run()
          {
            cinderRenderer.resumed();
          }
        });

        return true;
      }
    }

    return false;
  }

  public boolean pause()
  {
    if (!shutdown && !finishing)
    {
      if (attached && !paused) // SIMPLE PROTECTION AGAINST SPURIOUS onPause() CALLS
      {
        paused = true;
        onPause();

        queueEvent(new Runnable()
        {
          public void run()
          {
            cinderRenderer.paused();
          }
        });
      }

      return true;
    }

    return false;
  }

  public boolean shutdown()
  {
    if (!shutdown && !finishing)
    {
      /*
       * A TRICKY-BUT-CERTAIN WAY TO COMMUNICATE WITH THE RENDERER'S THREAD BEFORE IT EXITS
       * SEE ALSO: GLView.onDetachedFromWindow() AND GLView.CustomContextFactory.destroyContext()
       */
      finishing = true;

      return true;
    }

    return false;
  }

  public boolean backPressed()
  {
    if (!shutdown && !finishing)
    {
      if (attached && !paused)
      {
        queueEvent(new Runnable()
        {
          public void run()
          {
            cinderRenderer.dispatchEvent(CinderRenderer.EVENT_TRIGGER_BACK); 
          }
        });

        return true;
      }
    }

    return false;
  }

  // ---------------------------------------- CustomContextFactory ----------------------------------------

  /*
   * BASED ON GLSurfaceView.DefaultContextFactory
   *
   * NECESSARY BECAUSE:
   *
   * 1) setPreserveEGLContextOnPause(true) IS NOT TRUSTABLE BY DESIGN, SO WE NEED TO BE NOTIFIED WHEN CONTEXT-DESTRUCTION ACTUALLY OCCURS
   *
   * 2) EGLContextFactory.destroyContext() IS THE ONLY "HOOK" FOR COMMUNICATING ON THE RENDERER'S THREAD WHEN:
   *    - THE VIEW HAS BEEN DETACHED (ASSERTION: THE RENDERER'S THREAD WILL EXIT UPON VIEW-DETACHMENT, WHICH IN TURN WILL TRIGGER CONTEXT-DESTRUCTION)
   *    - THE ACTIVITY IS BEING DESTROYED (ASSERTION: ACTIVITY DESTRUCTION WILL TRIGGER VIEW-DETACHMENT)
   */

  protected class CustomContextFactory implements EGLContextFactory
  {
    protected int mEGLContextClientVersion;

    public CustomContextFactory(int eglContextClientVersion)
    {
      mEGLContextClientVersion = eglContextClientVersion;
    }

    public EGLContext createContext(EGL10 egl, EGLDisplay display, EGLConfig config)
    {
      Utils.LOGD("CustomContextFactory.createContext");
      cinderRenderer.contextCreated();

      int[] attrib_list = { 0x3098, mEGLContextClientVersion, EGL10.EGL_NONE };
      return egl.eglCreateContext(display, config, EGL10.EGL_NO_CONTEXT, mEGLContextClientVersion != 0 ? attrib_list : null);
    }

    public void destroyContext(EGL10 egl, EGLDisplay display, EGLContext context)
    {
      Utils.LOGD("CustomContextFactory.destroyContext");
      cinderRenderer.contextDestroyed();

      if (!attached || finishing)
      {
        shutdown = true;

        /*
         * TODO: THESE 2 COULD BE MERGE A MORE "SPECIFIC" METHOD
         */
        cinderRenderer.detachedFromWindow();
        cinderRenderer.performShutdown();

        /*
         * TODO: TRY TO NOTIFY CinderBridge ON THE MAIN-THREAD
         *
         * - SO THAT IT CAN AVOID USING GLView FURTHER
         * - SO THAT IT CAN CALL CinderBridge.sketchDidShutdown()
         *
         * UPDATE: THIS SHOULD BE FEASIBLE BY SIMPLY "WAITING" FOR super.onDetachedFromWindow()
         */
      }

      egl.eglDestroyContext(display, context);
    }
  }
}
