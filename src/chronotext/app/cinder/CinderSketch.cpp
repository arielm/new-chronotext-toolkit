/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "CinderSketch.h"

#include "chronotext/app/cinder/CinderDelegate.h"

using namespace std;
using namespace ci;
using namespace ci::app;

namespace chr
{
    CinderDelegate* CinderSketch::getDelegate() const
    {
        return delegate;
    }
    
    void CinderSketch::setDelegate(CinderDelegate *delegate)
    {
        CinderSketch::delegate = delegate;
    }
    
#pragma mark ---------------------------------------- GETTERS ----------------------------------------
    
    double CinderSketch::getElapsedSeconds() const
    {
        return delegate->getElapsedSeconds();
    }
    
    uint32_t CinderSketch::getElapsedFrames() const
    {
        return delegate->getElapsedFrames();
    }
    
    bool CinderSketch::isEmulated() const
    {
        return delegate->isEmulated();
    }
    
    const WindowInfo& CinderSketch::getWindowInfo() const
    {
        return delegate->getWindowInfo();
    }
    
#pragma mark ---------------------------------------- SKETCH <-> DELEGATE COMMUNICATION ----------------------------------------
    
    void CinderSketch::action(int actionId)
    {
        delegate->action(actionId);
    }
    
    void CinderSketch::sendMessageToDelegate(int what, const string &body)
    {
        delegate->receiveMessageFromSketch(what, body);
    }
}
