#ifndef EVENT_CLASS
#define EVENT_CLASS
#include <functional>
#include <vector>
#include <unordered_map>
#include <string>
#include <iostream>
#include "json.hpp"
#include "../Client/Utils/JSONSerializer.cpp"

using namespace std;

class Event
{
public:
    using Listener = function<void(peerInfo)>;
    using ListenerID = int;

    ListenerID subscribe(Listener listener);
    void unsubscribe(ListenerID id);

    void emit(peerInfo data);

private:
    unordered_map<ListenerID, Listener> listeners;
    ListenerID nextId = 0;
};

#endif