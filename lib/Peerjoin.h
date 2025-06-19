#ifndef PEER_MANAGER_H
#define PEER_MANAGER_H
#include <iostream>
#include <unordered_map>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <unordered_map>
#include "PeerJoinEventHandler.h"

using namespace std;

struct Message
{
    bool success;
    string type;
    string message;
};

class PeerManager
{
private:
    int *selfsoc;
    peerInfo self_info;
    unordered_map<string, vector<string>> pieceMap;

public:
    static unordered_map<string, peerInfo> peer_map;
    PeerManager(int *soc, peerInfo p);
    void add_peer(peerInfo peer);
    void send_request(peerInfo peer);
    bool hand_shake(string str, string local_hash);
    void message_handler(Message m, peerInfo p);
};

#endif