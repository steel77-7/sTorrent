#ifndef PIECE_SELECTION
#define PIECE_SELECTION

#include <unordered_map>
#include <iostream>
#include <vector>
#include <unordered_set>
#include <random>
#include <fstream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <algorithm>
#include "../../lib/Peerjoin.h"
using namespace std;
// have a piece
/*
divide it into blocks
have a pieve id */
class PeerManager;
class PieceManager
{
private:
    // unordered_map<string, vector<Piece>> peerPieceMap;
    vector<Piece> to_download; 
   // unordered_set<std::string , Piece> to_download; // later with all rarity and stuff
    PeerManager *p;

public:
    vector<string> downloaded;
    PieceManager(PeerManager *p);
    PieceManager();
    bool initialPieceSelection();
    void rarest_piece_selection();
    void downloader(string pieceid, block *block_info, int soc);
    void uploader();
    void assembler();
};

#endif