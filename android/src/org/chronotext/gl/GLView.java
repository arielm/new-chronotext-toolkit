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
import android.content.res.Configuration;
import android.opengl.GLSurfaceView;
import android.util.Log;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.View;
import android.view.ViewGroup.LayoutParams;

public class GLView extends GLSurfaceView
{
  protected GLRenderer renderer;

  protected boolean resumed;
  protected boolean attached;

  protected int originalWidth;
  protected int originalHeight;

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
   * OCCURS WHEN ENTERING BACKGROUND
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
  protected void onSizeChanged(int w, int h, int oldw, int oldh)
  {
    Log.i("CHR", "*** GLView.onSizeChanged: " + w + "x" + h + " | " + oldw + "x" + oldh + " ***");
    super.onSizeChanged(w, h, oldw, oldh);

    if (originalWidth * originalHeight == 0)
    {
      /*
       * ASSERTION: THIS IS THE "ORIGINAL SIZE" OF OUR GLView
       */
      originalWidth = w;
      originalHeight = h;

      getHolder().setFixedSize(originalWidth, originalHeight); // NECESSARY FOR THE SYSTEM-BUG TREATED IN onConfigurationChanged()
    }
  }

  /*
   * DISPLAY CAN BE DEFORMED WHEN RETURNING FROM SLEEP AT A DIFFERENT ORIENTATION
   * THIS IS A SYSTEM-BUG REPRODUCIBLE ON DEVICES AS-OLD-AS XOOM 1 (3.1) AND AS-RECENT-AS NEXUS 7 2013 (4.2.2)
   *
   * THE WORKAROUND IS ASSUMING THAT OUR GLView IS FULL-SCREEN
   * LUCKILY, OTHER VIEWS (E.G. ANDROID WIDGETS PLACED ON TOP OF IT) DO NOT REQUIRE ANY "SPECIAL TREATMENT"
   *
   * REFERENCE: http://stackoverflow.com/questions/7185644/android-opengl-crazy-aspect-ratio-after-sleep
   */
  public void onConfigurationChanged(Configuration newConfig)
  {
    Log.i("CHR", "*** GLView.onConfigurationChanged ***");

    LayoutParams layoutParams = getLayoutParams();
    layoutParams.width = originalWidth;
    layoutParams.height = originalHeight;
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
          renderer.onVisibilityChanged(visibility);
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
