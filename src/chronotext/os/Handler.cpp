#include "chronotext/os/Handler.h"

bool Handler::sendMessage(const Message &message)
{
    if (looper)
    {
        Message tmp = message;
        tmp.target = this;

        return looper->messageQueue.enqueueMessage(tmp);
    }
    
    return false;
}
