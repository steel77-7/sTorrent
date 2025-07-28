#ifndef EVENT_HANDLER
#define EVENT_HANDLER
#include <iostream>
#include <functional>
#include <unordered_map>
#include <vector>
#include "JSONSerializer.h"

using namespace std;
class Event
{
public:
    using ListenerID = int;
    using Listener = function<void(peerInfo)>;
    ListenerID subscribe(Listener listener);
    void unsubscribe(ListenerID id);
    void emit(peerInfo data);

private:
    ListenerID listenerId = 0;
    unordered_map<ListenerID, Listener> listeners;
};

#endif
