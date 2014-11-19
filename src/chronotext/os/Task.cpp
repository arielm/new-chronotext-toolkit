/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "Task.h"
#include "TaskManager.h"

#include "cinder/Utilities.h"

using namespace std;
using namespace ci;

namespace chronotext
{
    Task::Task()
    :
    started(false),
    ended(false),
    cancelRequired(false),
    synchronous(false),
    manager(nullptr)
    {}
    
    Task::~Task()
    {
        performDetach(); // OTHERWISE: OSX APPLICATION CAN CRASH UPON WINDOW CLOSING, ETC.
    }
    
    void Task::sleep(float milliseconds)
    {
        ci::sleep(milliseconds);
    }
    
    bool Task::start(bool forceSync)
    {
        if (!started && !ended)
        {
            if (manager)
            {
                synchronous = forceSync;
                started = true;
                
                if (synchronous)
                {
                    performRun();
                }
                else
                {
                    _thread = thread(&Task::performRun, this);
                }
                
                return true;
            }
        }
        
        return false;
    }
    
    bool Task::cancel()
    {
        boost::mutex::scoped_lock lock(_mutex);
        
        if (!synchronous && !cancelRequired && started)
        {
            cancelRequired = true;
            return true;
        }
        
        return false;
    }
    
    bool Task::isCancelRequired()
    {
        boost::mutex::scoped_lock lock(_mutex);
        return cancelRequired;
    }
    
    void Task::performDetach()
    {
        if (!synchronous)
        {
            if (_thread.joinable())
            {
                _thread.detach();
            }
        }
    }
    
    void Task::performRun()
    {
        if (synchronous)
        {
            run();
        }
        else
        {
            ThreadSetup forCocoa; // XXX: MANDATORY CALL (USED FOR OSX AND iOS, DUMMY ON OTHER PLATFORMS)
            run();
        }
        
        started = false;
        ended = true;
        
        if (synchronous)
        {
            manager->taskEnded(this);
        }
        else
        {
            manager->post([=]{ manager->taskEnded(this); }, false); // TODO: DOUBLE-CHECK LAMBDA
        }
    }
    
    template <typename F>
    bool Task::post(F &&fn)
    {
        return manager->post(forward<F>(fn), synchronous);
    }
}
