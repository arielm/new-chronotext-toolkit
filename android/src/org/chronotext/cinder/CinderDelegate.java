/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

package org.chronotext.cinder;

import org.chronotext.gl.GLView;

import android.app.Activity;
import android.content.res.Configuration;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.View;
import android.view.ViewGroup.LayoutParams;

public class CinderDelegate extends Handler
{
  public static final int ACTION_CAPTURE_BACK_KEY = 1;
  public static final int ACTION_RELEASE_BACK_KEY = 2;

  protected Activity mActivity;
  protected Handler mHandler;

  protected CinderRenderer mRenderer;
  protected GLView mView;
  protected boolean mBackKeyCaptured;

  public CinderDelegate(Activity activity)
  {
    mActivity = activity;
    mHandler = this;

    mRenderer = new CinderRenderer(activity, this);

    mView = new GLView(activity);
    mView.setRenderer(mRenderer); // WILL START THE RENDERER'S THREAD

    mBackKeyCaptured = false;
  }

  public CinderDelegate(Activity activity, Handler handler)
  {
    this(activity);
    mHandler = handler;
  }

  public Activity getActivity()
  {
    return mActivity;
  }

  public Handler getHandler()
  {
    return mHandler;
  }

  public GLView getView()
  {
    return mView;
  }

  public void showView()
  {
    if (mView.getVisibility() == View.GONE)
    {
      mView.setVisibility(View.VISIBLE);
    }
  }

  public void hideView()
  {
    if (mView.getVisibility() == View.VISIBLE)
    {
      mView.setVisibility(View.GONE);
    }
  }

  protected void handleAction(int actionId)
  {
    switch (actionId)
    {
      case ACTION_CAPTURE_BACK_KEY:
        mBackKeyCaptured = true;
        break;

      case ACTION_RELEASE_BACK_KEY:
        mBackKeyCaptured = false;
        break;
    }
  }

  // ---------------------------------------- TO BE CALLED FROM THE HOST ACTIVITY ----------------------------------------

  public void onPause()
  {
    mView.onPause(); // PURPOSELY NOT CALLED ON THE RENDERER'S THREAD
  }

  public void onResume()
  {
    mView.onResume(); // PURPOSELY NOT CALLED ON THE RENDERER'S THREAD
  }

  public void onDestroy()
  {
      mView.queueEvent(new Runnable()
      {
        public void run()
        {
          mRenderer.onDestroy();
        }
      });
  }

  public void onConfigurationChanged (Configuration newConfig)
  {
    Log.i("CHR", "*** CinderDelegate.onConfigurationChanged ***");

    /*
     * DISPLAY CAN BE DEFORMED WHEN BACK FROM SLEEP-MODE WITH A DIFFERENT ORIENTATION
     * THE SOLUTION IN GLView.surfaceChanged() IS WORKING FOR SOME DEVICES (E.G. NEXUS 7 2013, VER 4.2.2)
     * BUT NOT ON OTHERS (E.G. XOOM 1, VER 3.1), HENCE THE FOLLOWING HACK
     *
     * ASSERTION: OUR GLView IS FULL-SCREEN
     * LUCKILY, OTHER VIEWS (E.G. NATIVE WIDGETS ON TOP OF IT) DO NOT REQUIRE ANY "SPECIAL TREATMENT"
     *
     * REFERENCE: http://stackoverflow.com/questions/7185644/android-opengl-crazy-aspect-ratio-after-sleep
     */

    // MOMENTARY DISABLING OF THE SOLUTION, FOR TESTING PURPOSES
    //
    // LayoutParams layoutParams = mView.getLayoutParams();

    // if ((layoutParams.width != mView.definedWidth) || (layoutParams.height != mView.definedHeight))
    // {
    //   layoutParams.width = mView.definedWidth;
    //   layoutParams.height = mView.definedHeight;
    // }
  }

  public boolean onBackPressed()
  {
    if (mBackKeyCaptured)
    {
      mView.queueEvent(new Runnable()
      {
        public void run()
        {
          mRenderer.event(CinderRenderer.EVENT_BACK_KEY); 
        }
      });

      return true;
    }
    else
    {
      return false;
    }
  }

  // ---------------------------------------- MESSAGING SYSTEM ----------------------------------------

  /*
   * THIS IS RECEIVED ON THE RENDERER'S THREAD
   */
  public void action(final int actionId)
  {
    mActivity.runOnUiThread(new Runnable()
    {
      public void run()
      {
        handleAction(actionId);
      }
    });
  }

  /*
   * THIS IS RECEIVED ON THE RENDERER'S THREAD
   */
  public void receiveMessageFromSketch(int what, String body)
  {
    if (mHandler != null)
    {
      mHandler.sendMessage(Message.obtain(mHandler, what, body));
    }
  }

  public void sendMessageToSketch(int what)
  {
    sendMessageToSketch(what, (String) null);
  }

  /*
   * THIS WILL BE SENT ON THE RENDERER'S THREAD
   */
  public void sendMessageToSketch(int what, String body)
  {
    mView.sendMessage(what, body);
  }
}
