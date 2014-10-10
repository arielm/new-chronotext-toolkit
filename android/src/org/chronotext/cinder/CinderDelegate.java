/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

package org.chronotext.cinder;

import org.chronotext.gl.GLView;
import org.chronotext.utils.Utils;

import android.app.Activity;
import android.content.Context;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.View;

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
    prelaunch(activity, this); // WILL CREATE THE C++ CinderDelegate

    mView = new GLView(activity);
    mView.setRenderer(mRenderer); // WILL START THE RENDERER'S THREAD
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

  // ---------------------------------------- TO BE FORWARDED FROM THE HOST ACTIVITY ----------------------------------------

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
    mView.onDestroy(); // PURPOSELY NOT CALLED ON THE RENDERER'S THREAD
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

  // ---------------------------------------- JNI ----------------------------------------

  public native void prelaunch(Context context, Object listener);
}
