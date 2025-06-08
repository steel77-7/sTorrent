#include "../lib/Event.h"
using namespace std;
using Listener = function<void(peerInfo)>;
using ListenerID = int;

// should have the list of the member s passed to it

ListenerID Event::subscribe(Listener listener)
{
    int id = nextId++;
    listeners[id] = listener;
    return id;
}

void Event ::unsubscribe(ListenerID id)
{
    listeners.erase(id);
}

void Event::emit(peerInfo data)
{
   cout << "someone joined" << endl;
    // do something here with the soockets recieved
    for (const auto &[id, listener] : listeners)
    {
        listener(data); // usi listerner ke pass jaega data
    }
}