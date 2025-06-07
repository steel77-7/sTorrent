#ifndef TRACKER_CLASS
#define TRACKER_CLASS

#include <iostream>
#include <cstring>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <map>
#include <thread>
#include <unordered_map>
#include <functional>

#define MAX_CONNECTIONS 10
#define PORT 6969
using namespace std;

struct peerInfo
{
    string ip;
    string info_hash;
    string peer_id;
    // int port;
    // string event;
};

class Peer
{
private:
    char buffer[1024];
    sockaddr_in *client_addr;

public:
    int client_socket;

    Peer(int client_socket, socklen_t client_addr_len, sockaddr_in *client_addr);
    void sendMessage();
    void sendMessage(string message);
    void ConnectionHanlder();
};

vector<peerInfo> peerList;
unordered_map<int, Peer> connections;

#endif