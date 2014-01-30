Temporarely using the Scalability sample in order to test memory-warning-detection on Android.
 
[Activity.onLowMemory()](http://developer.android.com/reference/android/content/ComponentCallbacks.html#onLowMemory()) is used, but it is not working as intended (or at least in a way similar to `applicationDidReceiveMemoryWarningNotification` on iOS...)

**Description**

- Each frame, a 512x1024 RGB image is loaded from a JPG file and then uploaded to the GPU.  
- On a Xoom 1 tablet (Android 3.1), the application is crashing after about 360 images.
- The problem is that Activity.onLoadMemory is **never invoked**.
