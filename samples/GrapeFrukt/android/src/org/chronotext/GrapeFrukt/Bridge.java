
package org.chronotext.GrapeFrukt;

import org.chronotext.cinder.CinderDelegate;
import org.json.JSONException;
import org.json.JSONObject;

import android.app.Activity;
import android.content.res.Configuration;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.widget.Button;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.RelativeLayout;

public class Bridge extends CinderDelegate
{
  static
  {
    System.loadLibrary("GrapeFrukt");
  }

  protected RelativeLayout container;
  
  public Bridge(Activity activity)
  {
    super(activity);

    // ---

    activity.setContentView(getView());

    container = new RelativeLayout(activity);
    container.setDescendantFocusability(ViewGroup.FOCUS_BEFORE_DESCENDANTS);
    container.setFocusableInTouchMode(true);
    activity.addContentView(container, new ViewGroup.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT));

    // ---

    addButton("hide / show", 100, 100, 1);
  }

  void addButton(String text, int x, int y, final int id)
  {
    Button button = new Button(getActivity());
    button.setText(text);
    button.setId(1);

    RelativeLayout.LayoutParams params = new RelativeLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.WRAP_CONTENT);
    params.leftMargin = x;
    params.topMargin = y;
    container.addView(button, params);

    button.setOnClickListener(new OnClickListener()
    {
      @Override
      public void onClick(View v)
      {
        buttonClicked(id);
      }
    });
  }

  void buttonClicked(int id)
  {
    switch (id)
    {
      case 1:
        if (getView().getVisibility() != View.VISIBLE)
        {
          getView().setVisibility(View.VISIBLE);
        }
        else
        {
          getView().setVisibility(View.GONE);
        }
        break;
    }
  }
  
  public void handleMessage(Message msg)
  {}
}
