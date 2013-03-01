#include "chronotext/os/Handler.h"
#include "chronotext/os/Message.h"
#include "chronotext/os/MessageQueue.h"
#include "chronotext/os/Looper.h"

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
