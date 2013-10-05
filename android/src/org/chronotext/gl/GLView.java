/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

package org.chronotext.gl;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.view.MotionEvent;
import android.view.View;

public class GLView extends GLSurfaceView
{
  GLRenderer renderer;

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
  protected void onAttachedToWindow()
  {
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
    queueEvent(new Runnable()
    {
      public void run()
      {
        renderer.onDetachedFromWindow();
      }
    });

    super.onDetachedFromWindow();
  }

  @Override
  public void onResume()
  {
    super.onResume();

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
        final float x = event.getX(0);
        final float y = event.getY(0);

        queueEvent(new Runnable()
        {
          public void run()
          {
            renderer.addTouch(0, x, y);
          }
        });
        break;
      }

      case MotionEvent.ACTION_POINTER_DOWN :
      {
        final int index = event.getActionIndex();
        final float x = event.getX(index);
        final float y = event.getY(index);

        queueEvent(new Runnable()
        {
          public void run()
          {
            renderer.addTouch(index, x, y);
          }
        });
        break;
      }

      case MotionEvent.ACTION_UP :
      {
        final float x = event.getX(0);
        final float y = event.getY(0);

        queueEvent(new Runnable()
        {
          public void run()
          {
            renderer.removeTouch(0, x, y);
          }
        });
        break;
      }

      case MotionEvent.ACTION_POINTER_UP :
      {
        final int index = event.getActionIndex();
        final float x = event.getX(index);
        final float y = event.getY(index);

        queueEvent(new Runnable()
        {
          public void run()
          {
            renderer.removeTouch(index, x, y);
          }
        });
        break;
      }


      case MotionEvent.ACTION_MOVE :
      {
        for (int i = 0; i < event.getPointerCount(); i++)
        {
          final int index = i;
          final float x = event.getX(index);
          final float y = event.getY(index);

          queueEvent(new Runnable()
          {
            public void run()
            {
              renderer.updateTouch(index, x, y);
            }
          });
        }
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
