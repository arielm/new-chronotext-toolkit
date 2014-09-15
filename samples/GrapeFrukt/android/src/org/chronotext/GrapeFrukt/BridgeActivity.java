
package org.chronotext.GrapeFrukt;

import android.app.Activity;
import android.os.Bundle;

public class BridgeActivity extends Activity
{
  Bridge bridge;

  @Override
  public void onCreate(Bundle savedInstanceState)
  {
    super.onCreate(savedInstanceState);
    bridge = new Bridge(this);
  }

  @Override
  protected void onPause()
  {
    super.onPause();
    bridge.onPause();
  }

  @Override
  protected void onResume()
  {
    super.onResume();
    bridge.onResume();
  }
  
  @Override
  protected void onDestroy()
  {
    super.onDestroy();
    bridge.onDestroy();
  }
  
  @Override
  public void onBackPressed()
  {
    if (!bridge.onBackPressed())
    {
      bridge.onBackPressed();
    }
  }
}
