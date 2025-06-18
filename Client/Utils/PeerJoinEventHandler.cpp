#include "../../lib/PeerJoinEventHandler.h"
using ListenerID = int;
using Listener = function<void(int)>;

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


template<>
void Event::emit(peerInfo data)
{

    for (const auto &[id, listener] : listeners)
    {
        listener(data);
    }
}