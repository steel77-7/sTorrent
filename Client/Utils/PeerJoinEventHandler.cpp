#include "../../lib/PeerJoinEventHandler.h"
using ListenerID = int;
using Listener = function<void()>;

ListenerID Event::subscribe(Listener listener)
{
    listenerId++;
    listeners[listenerId] = listener;
    return listenerId;
}

void Event::unsubscribe(ListenerID id)
{
    listeners.erase(id);
}

void Event::emit(auto data)
{

    for (const auto &[id, listener] : listeners)
    {
        listener(data);
    }
}