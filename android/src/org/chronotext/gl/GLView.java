/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

package org.chronotext.gl;

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
  protected GLRenderer renderer;
  protected boolean resumed;

  public int definedWidth; // FIXME
  public int definedHeight; // FIXME

  public GLView(Context context)
  {
    super(context);

    // setEGLContextClientVersion(2);
    // setEGLConfigChooser(8, 8, 8, 8, 0, 0);
    // getHolder().setFormat(PixelFormat.RGBA_8888);
  }

  @Override
  public void setRenderer(Renderer renderer)
  {
    super.setRenderer(renderer);
    this.renderer = (GLRenderer) renderer;
  }

  @Override
  public void surfaceCreated(SurfaceHolder holder)
  {
    Log.i("CHR", "*** GLView.surfaceCreated ***"); 
    super.surfaceCreated(holder);
  }

  @Override
  public void surfaceChanged(SurfaceHolder holder, int format, int width, int height)
  {
    Log.i("CHR", "*** GLView.surfaceChanged: " + width + "x" + height + " ***");

    /*
     * DISPLAY CAN BE DEFORMED WHEN BACK FROM SLEEP-MODE WITH A DIFFERENT ORIENTATION
     * THE FOLLOWING SEEMS TO SOLVE THE ISSUE FOR SOME DEVICES (E.G. NEXUS 7 2013, VER 4.2.2) BUT NOT FOR
     * OTHERS (E.G. XOOM 1, VER 3.1), IN WHICH CASE WE RELY ON THE SOLUTION IN CinderDelegate.onConfigurationChanged()
     *
     * REFERENCE: http://stackoverflow.com/questions/7185644/android-opengl-crazy-aspect-ratio-after-sleep
     */
    if (definedWidth * definedHeight == 0)
    {
      holder.setFixedSize(width, height);

      definedWidth = width;
      definedHeight = height;
    }
    
    super.surfaceChanged(holder, format, width, height);
  }

  @Override
  public void surfaceDestroyed(SurfaceHolder holder) // OCCURS WHEN PRESSING THE "HOME" BUTTON BUT *NOT* WHEN GOING TO SLEEP
  {
    Log.i("CHR", "*** GLView.surfaceDestroyed ***"); 
    super.surfaceDestroyed(holder);
  }

  @Override
  protected void onAttachedToWindow()
  {
    Log.i("CHR", "*** GLView.onAttachedToWindow ***");
    super.onAttachedToWindow(); // WILL RESTART THE RENDERER'S THREAD

    queueEvent(new Runnable()
    {
      public void run()
      {
        renderer.onAttachedToWindow();
      }
    });
  }

  @Override
  protected void onDetachedFromWindow()
  {
    Log.i("CHR", "*** GLView.onDetachedFromWindow ***");

    queueEvent(new Runnable()
    {
      public void run()
      {
        /*
         * CURRENTLY NEVER REACHED WHEN DETACHED AS A RESULT OF APPLICATION DESTRUCTION
         * NOT SURE IF IT WORTHS TRYING TO FIND A SOLUTION...
         */
        renderer.onDetachedFromWindow();
      }
    });

    /*
     * PURPOSELY CALLED AT THE END
     * TODO: TEST IF renderer.onDetachedFromWindow() IS REACHED WHEN DETACHED AS A RESULT OF VIEW REMOVAL
     */
    super.onDetachedFromWindow(); // WILL STOP THE RENDERER'S THREAD
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
          renderer.onResume();
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
          renderer.onPause();
        }
      });
    }
  }

  @Override
  public void onWindowFocusChanged(final boolean hasFocus)
  {
    Log.i("CHR", "*** GLView.onWindowFocusChanged: " + hasFocus + " ***");
    super.onWindowFocusChanged(hasFocus);

    queueEvent(new Runnable()
    {
      public void run()
      {
        renderer.onWindowFocusChanged(hasFocus);
      }
    });
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
          renderer.onVisibilityChanged(visibility);
        }
      });
    }
  }

  @Override
  protected void onSizeChanged (int w, int h, int oldw, int oldh)
  {
    Log.i("CHR", "*** GLView.onSizeChanged: " + w + "x" + h + " | " + oldw + "x" + oldh + " ***");
    super.onSizeChanged(w, h, oldw, oldh);
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
            renderer.addTouches(touches);
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
            renderer.addTouches(touches);
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
            renderer.removeTouches(touches);
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
            renderer.removeTouches(touches);
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
            renderer.updateTouches(touches);
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
        renderer.sendMessage(what, body);
      }
    });
  }
}
