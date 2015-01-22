
package org.chronotext.MobileTest;

import org.chronotext.cinder.CinderBridge;
import org.chronotext.gl.GLView;
import org.chronotext.utils.Utils;
import org.json.JSONException;
import org.json.JSONObject;

import android.app.Activity;
import android.graphics.Color;
import android.os.Handler;
import android.os.Message;
import android.widget.Button;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.RelativeLayout;

public class Bridge extends CinderBridge
{
  static
  {
    System.loadLibrary("MobileTest");
  }

  static final int GLVIEW_ATTACHED_AND_VISIBLE_AT_START = 1;
  static final int GLVIEW_ATTACHED_AND_HIDDEN_AT_START = 2;
  static final int GLVIEW_NOT_ATTACHED_AT_START = 3; // XXX: WAS NOT RECOMMENDED

  boolean CAN_BE_HIDDEN = true;
  boolean CAN_BE_DETACHED = true;

//int testMode = GLVIEW_ATTACHED_AND_VISIBLE_AT_START;
  int testMode = GLVIEW_ATTACHED_AND_HIDDEN_AT_START;
//int testMode = GLVIEW_NOT_ATTACHED_AT_START;

  RelativeLayout rootView;
  RelativeLayout overlayView;

  Button button1;
  Button button2;

  boolean hasRootView;
  boolean hidden;
  boolean detached;

  public Bridge(Activity activity)
  {
    super(activity);

    // ---

    switch (testMode)
    {
      case GLVIEW_ATTACHED_AND_VISIBLE_AT_START:
        hidden = false;
        detached = false;
        break;

      case GLVIEW_ATTACHED_AND_HIDDEN_AT_START:
        hidden = true;
        detached = false;
        break;

      default:
      case GLVIEW_NOT_ATTACHED_AT_START:
        hidden = false;
        detached = true;
        break;
    }

    hasRootView = CAN_BE_HIDDEN || CAN_BE_DETACHED || hidden || detached;

    // ---

    setViewProperties(new GLView.Properties()
      .setEGLContextClientVersion(1)
      .setPreserveEGLContextOnPause(true));

    if (hasRootView)
    {
      rootView = new RelativeLayout(activity);
      rootView.setBackgroundColor(Color.YELLOW);
      activity.setContentView(rootView);
    }

    if (!detached)
    {
      if (hidden)
      {
        getView().setVisibility(View.GONE);
      }

      if (hasRootView)
      {
        rootView.addView(getView());
      }
      else
      {
        activity.setContentView(getView());
      }
    }

    // ---    

    overlayView = new RelativeLayout(activity);
    activity.addContentView(overlayView, new ViewGroup.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT));

    button1 = addButton(100, 100, 1);
    button2 = addButton(100, 200, 2);

    refreshButtons();
  }

  Button addButton(int x, int y, final int id)
  {
    final Button button = new Button(getActivity());
    button.setId(id);

    RelativeLayout.LayoutParams params = new RelativeLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.WRAP_CONTENT);
    params.leftMargin = x;
    params.topMargin = y;
    overlayView.addView(button, params);

    button.setOnClickListener(new OnClickListener()
    {
      @Override
      public void onClick(View v)
      {
        buttonClicked(button);
      }
    });

    return button;
  }

  void buttonClicked(Button button)
  {
    switch (button.getId())
    {
      case 1:
        if (hidden)
        {
          hidden = false;
          getView().setVisibility(View.VISIBLE);
        }
        else
        {
          hidden = true;
          getView().setVisibility(View.GONE);
        }

        break;

      case 2:
        if (detached)
        {
          detached = false;
          rootView.addView(getView());
        }
        else
        {
          detached = true;
          rootView.removeView(getView());
        }

        break;
    }

    refreshButtons();
  }

  void refreshButtons()
  {
    button1.setVisibility(detached ? View.GONE : View.VISIBLE);
    button2.setVisibility(hasRootView ? View.VISIBLE : View.GONE);

    button1.setText(hidden ? "show" : "hide");
    button2.setText(detached ? "attach" : "detach");
  }
  
  public void handleMessage(Message msg)
  {}
}
