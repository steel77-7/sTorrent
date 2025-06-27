#ifndef PEER_MANAGER_H
#define PEER_MANAGER_H
#include <iostream>
#include <unordered_map>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <vector>
#include <unordered_map>
#include "PeerJoinEventHandler.h"
#include "JSONSerializer.h"

using namespace std;

/*
struct Message
{
    bool success;
    string type;
    string message;
}; 
*/

class PeerManager
{
private:
    int *selfsoc;
    peerInfo self_info;
    unordered_map<string, vector<peerInfo>> pieceMap; //piece id with the peers that have it
    
public:
    static unordered_map<string, int> peer_map;
    PeerManager(int *soc, peerInfo p);
    void add_peer(peerInfo peer);
    void send_request(peerInfo peer);
    bool hand_shake(string str, string local_hash);
    void peerSelection(); //will select the file to be downloaded from atmost 5 peers 
    void message_handler(Message m, peerInfo p);
    void sendMessage(Message m ,int soc); 
    void downloadHandler(Piece piece);
    void chokingManager(); 
    void optimisticUnchoke(); 
};

#endif