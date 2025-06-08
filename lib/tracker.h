#ifndef TRACKER_CLASS
#define TRACKER_CLASS

#include <iostream>
#include <cstring>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <variant>
#include "Event.h"
// #include "../tracker/JoinHandler.cpp"

#define MAX_CONNECTIONS 10
#define PORT 6969
using namespace std;

using namespace std;


struct Message
{
    bool success;
    string type;
    string message;
};

using MessageType = variant<peerInfo, Message>;

class Peer
{
private:
    char buffer[1024];
    sockaddr_in *client_addr;
    Event* e;

public:
    int client_socket;

    Peer(int client_socket, socklen_t client_addr_len, sockaddr_in *client_addr, Event *e);
    void sendMessage();
    void sendMessage(Message message);
    void ConnectionHanlder();
    string recieveMessage();
    MessageType peerMessageSerializer(string s);
};

vector<peerInfo> peerList;
unordered_map<int, Peer> connections;

#endif