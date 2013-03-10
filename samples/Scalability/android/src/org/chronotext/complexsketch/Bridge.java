
package org.chronotext.complexsketch;

import android.app.Activity;
import android.os.Handler;
import chronotext.android.cinder.CinderDelegate;

public class Bridge extends CinderDelegate
{
  static
  {
    System.loadLibrary("ComplexSketch");
  }

  public Bridge(Activity activity, Handler handler)
  {
    super(activity, handler);
  }
}
