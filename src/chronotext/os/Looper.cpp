#include "chronotext/os/Looper.h"
#include "chronotext/os/Message.h"
#include "chronotext/os/Handler.h"

void Looper::run()
{
    while (true)
    {
        Message message = messageQueue.nextMessage();
        
        if (message.target)
        {
            message.target->handleMessage(message);
        }
        else
        {
            break;
        }
    }
}
