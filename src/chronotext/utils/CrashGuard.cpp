#include "chronotext/utils/CrashGuard.h"

#include "cinder/app/AppBasic.h"

using namespace std;
using namespace ci;
using namespace ci::app;

CrashGuard::CrashGuard() : mCount(0)
{
    mThread = shared_ptr<thread>(new thread(&CrashGuard::run, this));
}

void CrashGuard::run()
{
    ThreadSetup threadSetup;
    
    while (true)
    {
        boost::this_thread::sleep(boost::posix_time::seconds(2));
        
        if (mCount)
        {
            mCount = 0;
        }
        else
        {
            AppBasic *app = AppBasic::get();
            
            if (app->isFullScreen())
            {
                app->setFullScreen(false);
                boost::this_thread::sleep(boost::posix_time::seconds(1));
            }
            
            exit(-1);
        }
    }
}

void CrashGuard::update()
{
    mCount++;
}
