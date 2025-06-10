#ifndef PEER_JOIN_H
#define PEER_JOIN_H
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdlib>
#include <cstring>

using namespace std ; 


struct peerInfo
{
    string ip;
    string info_hash;
    string peer_id;
    //  int port;
    //  string event;
};


class Peer{ 
    private : 
        struct peerInfo info ; 
        int soc ;
    public : 
        Peer(peerInfo i);
        void accept_connection_request();
        void send_connection_request() ;
        void hand_shake(); 
        void message_handler(); 
} ;

#endif