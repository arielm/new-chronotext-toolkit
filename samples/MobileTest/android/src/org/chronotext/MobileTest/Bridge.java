
package org.chronotext.MobileTest;

import org.chronotext.cinder.CinderDelegate;
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

public class Bridge extends CinderDelegate
{
  static
  {
    System.loadLibrary("MobileTest");
  }

  static final int GLVIEW_ATTACHED_AND_VISIBLE_AT_START = 1;
  static final int GLVIEW_ATTACHED_AND_HIDDEN_AT_START = 2;
  static final int GLVIEW_NOT_ATTACHED_AT_START = 3; // NOT RECOMMENDED!

  int testMode = GLVIEW_ATTACHED_AND_VISIBLE_AT_START;
  // boolean contentViewIsGLView = true;

  // int testMode = GLVIEW_NOT_ATTACHED_AT_START;
  boolean contentViewIsGLView = false;

  RelativeLayout rootView;
  RelativeLayout overlayView;

  Button button1;
  Button button2;

  boolean hidden;
  boolean detached;

  public Bridge(Activity activity)
  {
    super(activity);

    // ---

    if (!contentViewIsGLView)
    {
      rootView = new RelativeLayout(activity);
      rootView.setBackgroundColor(Color.YELLOW);
      activity.setContentView(rootView);
    }

    if (testMode == GLVIEW_ATTACHED_AND_VISIBLE_AT_START || testMode == GLVIEW_ATTACHED_AND_HIDDEN_AT_START)
    {
      if (contentViewIsGLView)
      {
        activity.setContentView(getView());
      }
      else
      {
        rootView.addView(getView());
      }
     
      if (testMode == GLVIEW_ATTACHED_AND_HIDDEN_AT_START)
      {
        getView().setVisibility(View.GONE);
      }  
    }
    else if (testMode == GLVIEW_NOT_ATTACHED_AT_START)
    {
      if (contentViewIsGLView)
      {
        Utils.LOGE("INVALID TEST MODE");
        return;
      }
    }
    else
    {
      Utils.LOGE("UNDEFINED TEST MODE");
      return;
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
          getView().setVisibility(View.VISIBLE);
        }
        else
        {
          getView().setVisibility(View.GONE);
        }
        break;

      case 2:
        if (detached)
        {
          rootView.addView(getView());
        }
        else
        {
          rootView.removeView(getView());
        }
        break;
    }

    refreshButtons();
  }

  void refreshButtons()
  {
    hidden = (getView().getVisibility() != View.VISIBLE);
    detached = (getView().getParent() == null);

    button1.setVisibility(detached ? View.GONE : View.VISIBLE);
    button2.setVisibility(contentViewIsGLView ? View.GONE : View.VISIBLE);

    button1.setText(hidden ? "show" : "hide");
    button2.setText(detached ? "attach" : "detach");
  }
  
  public void handleMessage(Message msg)
  {}
}
