#include "chronotext/android/cinder/CinderRendererJNI.h"

#include "Sketch.h"

chr::CinderDelegate* createDelegate()
{
    chr::CinderDelegate *delegate = new chr::CinderDelegate();
    delegate->sketch = new Sketch(delegate, delegate);

    return delegate;
}

extern "C"
{
	void android_main(struct android_app *state) {}
}
