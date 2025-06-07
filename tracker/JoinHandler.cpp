#include "../lib/tracker.h"
using namespace std;

// should have the list of the member s passed to it
// just make it work first then care about the optimisation
class Event
{
public:
    using Listener = function<void(peerInfo)>;
    using ListenerID = int;
    ListenerID subscribe(Listener listener)
    {
        int id = nextId++;
        listeners[id] = listener;
        return id;
    }

    void unsubscribe(ListenerID id)
    {
        listeners.erase(id);
    }

    // pass the list of the peers as well here
    void emit(auto data)
    {
        cout<<"someone joined"<<endl;
        // do something here with the soockets recieved
        for (const auto &[id, listener] : listeners)
        {
            listener(data); // usi listerner ke pass jaega data
        }
    }

private:
    unordered_map<ListenerID, Listener> listeners;
    ListenerID nextId = 0;
};