#ifndef PIECE_SELECTION
#define PIECE_SELECTION
#include <unordered_map>
#include <iostream>
#include <vector>
#include <set>
#include <random>
#include <fstream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "../../lib/Peerjoin.h"
using namespace std;
// have a piece
/*
divide it into blocks
have a pieve id */

struct block
{
    string piece_id;
    int offset;
    int size;
    string status ; //downaloded , downloading , not downaloding
};

struct Piece
{
    string piece_id;
    int size ; 
    vector<block> blocks;
    string status;
};

class PieceManager
{
private:
   // unordered_map<string, vector<Piece>> peerPieceMap;
    set<Piece> downloaded;
    vector<Piece> to_download; // later will all rarity and stuff
    PeerManager *p;
public:
    PieceManager(PeerManager *p);
    void initialPieceSelection();
    void pieceSelection();
    void downloader(string pieceid,block *block_info, int soc);
    void uploader();
    void assembler();
};

#endif