/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
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
    synchronized(false),
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
                started = true;
                
                if (forceSync)
                {
                    synchronized = true;
                    run();
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
        
        if (started && !synchronized && !cancelRequired)
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
        if (_thread.joinable())
        {
            _thread.detach();
        }
    }
    
    void Task::performRun()
    {
        ThreadSetup forCocoa;
        
        run();
        
        started = false;
        ended = true;
        
        manager->post([=]{ manager->taskEnded(this); });
    }
}
