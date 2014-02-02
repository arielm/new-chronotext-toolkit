#include "Sketch.h"

#include "chronotext/utils/GLUtils.h"

#if defined(CINDER_COCOA_TOUCH)
#include "ImageSourceFileStbImage.h"
#endif

using namespace std;
using namespace ci;
using namespace chr;

const float SCALE = 600;
const float FPS = 30;

Sketch::Sketch(void *context, void *delegate)
:
CinderSketch(context, delegate)
{}

void Sketch::setup(bool renewContext)
{
    if (renewContext)
    {
        /*
         *  NECESSARY AFTER OPEN-GL CONTEXT-LOSS (OCCURS ON ANDROID WHEN APP GOES TO BACKGROUND)
         */
        textureManager.unload();
        textureManager.reload();
    }
    else
    {
#if defined(CINDER_COCOA_TOUCH)
        /*
         * ImageSourceFileStbImage IS NECESSARY IN ORDER TO SOLVE THE "PREMULTIPLIED PNG CURSE" ON iOS
         * REQUIRES THE FOLLOWING "USER-DEFINED SETTING" IN THE iOS XCode PROJECT:
         * IPHONE_OPTIMIZE_OPTIONS = "-skip-PNGs";
         */
        ImageIoRegistrar::registerSourceType("png", ImageSourceFileStbImage::createRef, 1);
#endif
        
        auto atlas = make_shared<TextureAtlas>(textureManager, InputSource::getResource("MonocleMan.xml"), true);
        animation = Animation(atlas, InputSource::getResource("sheets.xml"), InputSource::getResource("animations.xml"), FPS);
    }
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
}

void Sketch::resize()
{
    scale = getWindowHeight() / SCALE;
}

void Sketch::start(int flags)
{
    clock.start();
}

void Sketch::stop(int flags)
{
    clock.stop();
}

void Sketch::draw()
{
    gl::clear(Color::gray(1.0f), false);
    gl::setMatricesWindow(getWindowSize(), true);

    gl::translate(getWindowCenter());
    glScalef(scale, scale, 1);
    
    animation.play(clock.getTime());
}
