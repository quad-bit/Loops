#include <EventBus.h>

EventBus* EventBus::instance = nullptr;
EventBus * EventBus::GetInstance()
{
    if (instance == nullptr)
    {
        instance = new EventBus();
    }
    return instance;
}
