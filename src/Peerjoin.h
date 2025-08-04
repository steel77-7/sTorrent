#ifndef PEER_MANAGER_H
#define PEER_MANAGER_H
#include <iostream>
#include <unordered_map>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <vector>
#include <thread>
#include <set>
#include <mutex>
#include <openssl/sha.h>
#include <unordered_map>
#include "PeerJoinEventHandler.h"
#include "JSONSerializer.h"
#include "PieceSelection.h"

using namespace std;

enum message_type
{
    HAVE,
    REQUEST,
    INTERESTED,
    SIZE
};

class PieceManager;
class PeerManager
{
private:
    int *selfsoc;
    peerInfo self_info;
    bool seed; 

public:
    unordered_map<string, set<peerInfo*>> pieceMap; // piece id with the peers that have it
    unordered_map<string, peerInfo> peer_map;
    unordered_map<string, peerInfo*> choke_map;

    PieceManager *ps;
    PeerManager(int *soc, peerInfo p, bool seed);
    PeerManager();
    void add_peer(peerInfo peer);
    void send_request(peerInfo peer);
    bool hand_shake(string str, string local_hash);
    void peerSelection(); // will select the file to be downloaded from atmost 5 peers
    peerInfo* get_peer(string peer_id);
    void message_handler( string peer_id);
    void sendMessage(Message m, int soc);
    void downloadHandler(Piece piece, void (PieceManager::*downloader)(std::string, block *, int), PieceManager *pm);
    void chokingManager(string peer_id);//on each peer 
    void data_poll(); //will check if data is being recieved or not if not then new peer will be searched for downaloding the data

    void optimistic_unchoke();
    void seeder();
};

#endif