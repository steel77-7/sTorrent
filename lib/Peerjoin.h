#ifndef PEER_MANAGER_H
#define PEER_MANAGER_H
#include <iostream>
#include <unordered_map>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdlib>
#include <cstring>

using namespace std;

struct peerInfo
{
    string ip;
    string info_hash;
    string peer_id;
      int port;
    //  string event;
};

class PeerManager
{
private:
    static int selfsoc;
    static peerInfo self_info ; 
    static unordered_map<string, peerInfo> peer_map;

public:
    PeerManager(int soc,peerInfo p );
    void add_peer(peerInfo peer);
    void send_request(peerInfo peer) ; 
    bool hand_shake(string str, string local_hash);
    void message_handler();
};

#endif