#include "CrashGuard.h"

using namespace std;

CrashGuard::CrashGuard() : mCount(0)
{
    mThread = shared_ptr<thread>(new thread(&CrashGuard::run, this));
}

void CrashGuard::run()
{
    while (true)
    {
        boost::this_thread::sleep(boost::posix_time::seconds(1));
        
        if (mCount)
        {
            mCount = 0;
        }
        else
        {
            exit(-1);
        }
    }
}

void CrashGuard::update()
{
    mCount++;
}
