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

using namespace std;
// have a piece
/*
divide it into blocks
have a pieve id */

struct block
{
    string piece_id;
    int begin;
    int size;
    string status ; //downaloded , downloading , not downaloding
};

struct Piece
{
    string piece_id;
    vector<block> blocks;
    string status;
};

class PieceManager
{
private:
    unordered_map<string, vector<string>> peerPieceMap;
    set<Piece> downloaded;
    vector<Piece> to_download; // later will all rarity and stuff

public:
    PieceManager();
    void initialPieceSelection();
    void pieceSelection();
    void downloader(string pieceid,block block_info, int soc);
    void uploader();
    // void
};

#endif