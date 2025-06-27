#ifndef JSON_H
#define JSON_H 

#include <vector>
#include "json.hpp"
#pragma once
using namespace std ; 
using json = nlohmann ::json;



struct block
{
    string piece_id;
    int offset;
    int size;
    string status; // downaloded , downloading , not downaloding
};

struct Piece
{
    string piece_id;
    vector<block> blocks;
    string status;
};

struct peerInfo
{
    string ip;
    string info_hash;
    string peer_id;
    int port;
    bool choked; 
    vector<string> pieces_hash; 
    int socket;
    // string event;
};

struct Message
{
    bool success;
    string type;
    string message;
};

void from_json(const json &j, peerInfo &p);
void to_json(json &j, peerInfo p);
void from_json(json &j, Message &m);
void to_json(json &j, Message &m);
void from_json(json &j, block &b);
void to_json(json &j , block &b);



#endif