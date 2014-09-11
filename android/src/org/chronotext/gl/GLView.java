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
  public int definedWidth;
  public int definedHeight;

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
  public void surfaceChanged(SurfaceHolder holder, int format, int width, int height)
  {
    Log.i("CHR", "*** GLView.onSurfaceChanged: " + width + "x" + height + " ***");

    /*
     * DISPLAY CAN BE DEFORMED WHEN BACK FROM SLEEP-MODE WITH A DIFFERENT ORIENTATION
     * THE FOLLOWING SEEMS TO SOLVE THE ISSUE FOR SOME DEVICES (E.G. NEXUS 7 2012) BUT NOT FOR
     * OTHERS (E.G. XOOM 1), IN WHICH CASE WE RELY ON THE SOLUTION IN GLView.onConfigurationChanged()
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
  public void surfaceDestroyed(SurfaceHolder holder) // OCCURS WHEN PRESSING THE HOME BUTTON BUT *NOT* WHEN DEVICE GOES TO SLEEP
  {
    Log.i("CHR", "*** GLView.surfaceDestroyed ***"); 
    super.surfaceDestroyed(holder);
  }

  @Override
  protected void onAttachedToWindow()
  {
    Log.i("CHR", "*** GLView.onAttachedToWindow ***");
    super.onAttachedToWindow();

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
    super.onDetachedFromWindow();
    Log.i("CHR", "*** GLView.onDetachedFromWindow ***");

    queueEvent(new Runnable()
    {
      public void run()
      {
        renderer.onDetachedFromWindow();
      }
    });
  }

  @Override
  public void onResume()
  {
    super.onResume();
    Log.i("CHR", "*** GLView.onResume ***");

    queueEvent(new Runnable()
    {
      public void run()
      {
        renderer.onResume();
      }
    });
  }

  @Override
  public void onPause()
  {
    super.onPause();
    Log.i("CHR", "*** GLView.onPause ***");

    queueEvent(new Runnable()
    {
      public void run()
      {
        renderer.onPause();
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

  public void onDestroy()
  {
    queueEvent(new Runnable()
    {
      public void run()
      {
        renderer.onDestroy();
      }
    });
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
