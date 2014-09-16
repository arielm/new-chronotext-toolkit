/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

package org.chronotext.gl;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;

import java.util.Vector;

import org.chronotext.gl.Touch;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.Log;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.View;

public class GLView extends GLSurfaceView
{
  protected GLRenderer mRenderer;

  protected boolean resumed;
  protected boolean attached;

  public GLView(Context context)
  {
    super(context);

    // setEGLContextClientVersion(2);
    // setEGLConfigChooser(8, 8, 8, 8, 0, 0);
    // getHolder().setFormat(PixelFormat.RGBA_8888);

    setEGLContextFactory(new CustomContextFactory(1)); // NECESSARY BECAUSE setPreserveEGLContextOnPause(true) CAN'T BE FULLY TRUSTED
    setPreserveEGLContextOnPause(true);
  }

  @Override
  public void setRenderer(Renderer renderer)
  {
    if (mRenderer == null)
    {
      super.setRenderer(renderer); // WILL START THE RENDERER'S THREAD
      mRenderer = (GLRenderer) renderer;

      queueEvent(new Runnable()
      {
        public void run()
        {
          mRenderer.performLaunch();
        }
      });
    }
  }

  /*
   * OCCURS AT APPLICATION-STARTUP OR WHEN RETURNING FROM BACKGROUND
   * BUT *NOT* WHEN RETURNING FROM SLEEP
   */
  @Override
  public void surfaceCreated(SurfaceHolder holder)
  {
    Log.i("CHR", "*** GLView.surfaceCreated ***"); 
    super.surfaceCreated(holder);
  }

  /*
   * OCCURS WHEN ENTERING BACKGROUND OR WHEN VIEW IS HIDDEN
   * BUT *NOT* WHEN ENTERING SLEEP
   */
  @Override
  public void surfaceDestroyed(SurfaceHolder holder)
  {
    Log.i("CHR", "*** GLView.surfaceDestroyed ***"); 
    super.surfaceDestroyed(holder);
  }

  @Override
  public void surfaceChanged(SurfaceHolder holder, int format, int width, int height)
  {
    Log.i("CHR", "*** GLView.surfaceChanged: " + width + "x" + height + " ***");
    super.surfaceChanged(holder, format, width, height);
  }

  @Override
  protected void onAttachedToWindow()
  {
    Log.i("CHR", "*** GLView.onAttachedToWindow ***");
    super.onAttachedToWindow(); // WILL START THE RENDERER'S THREAD (IF NECESSARY)

    queueEvent(new Runnable()
    {
      public void run()
      {
        mRenderer.onAttachedToWindow();
      }
    });
  }

  @Override
  protected void onDetachedFromWindow()
  {
    Log.i("CHR", "*** GLView.onDetachedFromWindow ***");
    super.onDetachedFromWindow(); // WILL EXIT THE RENDERER'S THREAD

    /*
     * PROBLEM: THE FOLLOWING CAN'T BE DELIVERED
     * SEE THE SOURCE-CODE GLSurfaceView.onDetachedFromWindow() TO FIND-OUT WHY
     */
    queueEvent(new Runnable()
    {
      public void run()
      {
        mRenderer.onDetachedFromWindow();
      }
    });
  }

  /*
   * RECEIVED ON MAIN-THREAD
   */
  @Override
  public void onResume()
  {
    Log.i("CHR", "*** GLView.onResume ***");

    if (!resumed)
    {
      resumed = true;
      super.onResume();

      queueEvent(new Runnable()
      {
        public void run()
        {
          mRenderer.onResume();
        }
      });
    }
  }

  /*
   * RECEIVED ON MAIN-THREAD
   */
  @Override
  public void onPause()
  {
    Log.i("CHR", "*** GLView.onPause ***");

    if (resumed)
    {
      resumed = false;
      super.onPause();

      queueEvent(new Runnable()
      {
        public void run()
        {
          mRenderer.onPause();
        }
      });
    }
  }

  /*
   * WARNING: THIS CALLBACK IS CALLED SO INCONSISTENTLY BY THE SYSTEM THAT WE CAN'T RELY ON IT
   */
  @Override
  public void onWindowFocusChanged(final boolean hasFocus)
  {
    Log.i("CHR", "*** GLView.onWindowFocusChanged: " + hasFocus + " ***");
    super.onWindowFocusChanged(hasFocus);
  }

  @Override
  public void onVisibilityChanged(View changedView, final int visibility)
  {
    if (changedView == this)
    {
      queueEvent(new Runnable()
      {
        public void run()
        {
          mRenderer.onVisibilityChanged(visibility);
        }
      });
    }
  }

  @Override
  public boolean onTouchEvent(MotionEvent event)
  {
    switch (event.getAction() & MotionEvent.ACTION_MASK)
    {
      case MotionEvent.ACTION_DOWN :
      {
        int index = 0;
        final Vector<Touch> touches = new Vector<Touch>();
        touches.add(new Touch(event.getPointerId(index), event.getX(index), event.getY(index)));

        queueEvent(new Runnable()
        {
          public void run()
          {
            mRenderer.addTouches(touches);
          }
        });
        break;
      }

      case MotionEvent.ACTION_POINTER_DOWN :
      {
        int index = event.getActionIndex();
        final Vector<Touch> touches = new Vector<Touch>();
        touches.add(new Touch(event.getPointerId(index), event.getX(index), event.getY(index)));

        queueEvent(new Runnable()
        {
          public void run()
          {
            mRenderer.addTouches(touches);
          }
        });
        break;
      }

      case MotionEvent.ACTION_UP :
      {
        int index = 0;
        final Vector<Touch> touches = new Vector<Touch>();
        touches.add(new Touch(event.getPointerId(index), event.getX(index), event.getY(index)));

        queueEvent(new Runnable()
        {
          public void run()
          {
            mRenderer.removeTouches(touches);
          }
        });
        break;
      }

      case MotionEvent.ACTION_POINTER_UP :
      {
        int index = event.getActionIndex();
        final Vector<Touch> touches = new Vector<Touch>();
        touches.add(new Touch(event.getPointerId(index), event.getX(index), event.getY(index)));

        queueEvent(new Runnable()
        {
          public void run()
          {
            mRenderer.removeTouches(touches);
          }
        });
        break;
      }


      case MotionEvent.ACTION_MOVE :
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
            mRenderer.updateTouches(touches);
          }
        });
        break;
      }
    }

    return true;
  }

  public void sendMessage(final int what, final String body)
  {
    queueEvent(new Runnable()
    {
      public void run()
      {
        mRenderer.sendMessage(what, body);
      }
    });
  }

  /*
   * BASED ON GLSurfaceView.DefaultContextFactory
   * SEEMS LIKE THE ONLY WAY TO BE NOTIFIED WHEN THE GL-CONTEXT IS ACTUALLY CREATED OR DESTROYED
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
      Log.i("CHR", "*** CustomContextFactory.createContext ***");

      mRenderer.contextCreated();

      int[] attrib_list = { 0x3098, mEGLContextClientVersion, EGL10.EGL_NONE };
      return egl.eglCreateContext(display, config, EGL10.EGL_NO_CONTEXT, mEGLContextClientVersion != 0 ? attrib_list : null);
    }

    public void destroyContext(EGL10 egl, EGLDisplay display, EGLContext context)
    {
      Log.i("CHR", "*** CustomContextFactory.destroyContext ***");

      mRenderer.contextDestroyed();
      egl.eglDestroyContext(display, context);
    }
  }
}
