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

import org.chronotext.cinder.CinderDelegate;
import org.chronotext.cinder.CinderRenderer;
import org.chronotext.cinder.Touch;
import org.chronotext.utils.Utils;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.View;

public class GLView extends GLSurfaceView
{
  protected CinderDelegate cinderDelegate;
  protected CinderRenderer cinderRenderer;

  protected boolean attached;
  protected boolean paused;
  protected boolean finishing;
  protected boolean destroyed;

  public GLView(Context context, CinderDelegate _cinderDelegate)
  {
    super(context);

    cinderDelegate = _cinderDelegate;

    // ---

    /*
     * TODO: THE FOLLOWING SHOULD BE USER-DEFINABLE
     */

    // setEGLContextClientVersion(2);
    // setEGLConfigChooser(8, 8, 8, 8, 0, 0);
    // getHolder().setFormat(PixelFormat.RGBA_8888);

    setEGLContextFactory(new CustomContextFactory(1)); // FIXME: EGL-CONTEXT-CLIENT-VERSION SHOULD NOT BE HARD-CODED

    /*
     * TEXTURES (AND OTHER GL-RELATED MEMORY) WILL NOT BE INALIDATED ANYMORE ON-PAUSE
     */
    setPreserveEGLContextOnPause(true);

    // ---

    cinderRenderer = new CinderRenderer();
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

    if (destroyed)
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
    }
  }

  @Override
  protected void onDetachedFromWindow()
  {
    Utils.LOGD("GLView.onDetachedFromWindow");

    if (attached)
    {
      attached = false; // THE ONLY WAY TO COMMUNICATE WITH THE RENDERER'S THREAD BEFORE IT EXITS
      super.onDetachedFromWindow(); // WILL EXIT THE RENDERER'S THREAD (EVENTS QUEUED RIGHT BEOFRE-OR-AFTER THIS WILL NEVER BE DELIVERED)
    }
  }

  /*
   * INVOKED ON THE MAIN-THREAD BY CinderDelegate
   */
  @Override
  public void onResume()
  {
    if (destroyed)
    {
      return;
    }
    
    if (attached && paused) // SIMPLE PROTECTION AGAINST SPURIOUS onResume() CALLS
    {
      paused = false;
      super.onResume();

      cinderDelegate.resuming();

      queueEvent(new Runnable()
      {
        public void run()
        {
          cinderRenderer.resumed();
        }
      });
    }
  }

  /*
   * INVOKED ON THE MAIN-THREAD BY CinderDelegate
   */
  @Override
  public void onPause()
  {
    if (destroyed)
    {
      return;
    }

    if (attached && !paused) // SIMPLE PROTECTION AGAINST SPURIOUS onPause() CALLS
    {
      paused = true;
      super.onPause();

      cinderDelegate.pausing();

      queueEvent(new Runnable()
      {
        public void run()
        {
          cinderRenderer.paused();
        }
      });
    }
  }

  /*
   * INVOKED ON THE MAIN-THREAD BY CinderDelegate
   */
  public void onDestroy()
  {
    if (destroyed)
    {
      return;
    }

    /*
     * INTENDED TO BE USED BY CustomContextFactory.destroyContext() ON THE RENDERER'S THREAD
     *
     * TRICKY, BUT THE BEHAVIOR OF GLSurfaceView.queueEvent() AT SHUT-DOWN IS SIMPLY NOT
     * CONSISTENT ACCROSS THE OS-VERSIONS WE SUPPORT. SEE ALSO: GLView.onDetachedFromWindow()
     */
    finishing = true;

    cinderDelegate.finishing();
  }

  /*
   * INVOKED ON THE MAIN-THREAD BY CinderDelegate
   */
  public boolean onBackPressed()
  {
    if (destroyed)
    {
      return false;
    }

    queueEvent(new Runnable()
    {
      public void run()
      {
        cinderRenderer.event(CinderRenderer.EVENT_BACK); 
      }
    });

    return true;
  }

  @Override
  public void onVisibilityChanged(View changedView, final int visibility)
  {
    if (attached && (changedView == this))
    {
      queueEvent(new Runnable()
      {
        public void run()
        {
          cinderRenderer.visibilityChanged(visibility);
        }
      });
    }
  }

  @Override
  public boolean onTouchEvent(MotionEvent event)
  {
    if (destroyed)
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

  /*
   * BASED ON GLSurfaceView.DefaultContextFactory
   *
   * NECESSARY BECAUSE:
   *
   * 1) setPreserveEGLContextOnPause(true) IS NOT TRUSTABLE BY DESIGN, SO WE NEED TO BE NOTIFIED WHEN CONTEXT-DESTRUCTION ACTUALLY OCCURS
   *
   * 2) EGLContextFactory.createContext() IS THE ONLY "HOOK" FOR COMMUNICATING ON THE RENDERER'S THREAD WHEN:
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
        destroyed = true;

        cinderRenderer.detachedFromWindow();
        cinderRenderer.performShutdown();
      }

      egl.eglDestroyContext(display, context);
    }
  }

  public void sendMessage(final int what, final String body)
  {
    if (!destroyed)
    {
      queueEvent(new Runnable()
      {
        public void run()
        {
          cinderRenderer.sendMessage(what, body);
        }
      });
    }
  }
}
