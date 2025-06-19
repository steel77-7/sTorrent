#ifndef PIECE_SELECTION
#define PIECE_SELECTION
#include <unordered_map>
#include <iostream>
#include <vector>
#include <set>
using namespace std;
// have a piece
/*
divide it into blocks
have a pieve id */
class PieceManager
{
private:
    unordered_map<string, vector<string>> peerPieceMap;
    set<string> downloaded ; 
    set<string> to_download ;  //later will all rarity and stuff

public:
    PieceManager();
    void initialPieceSelection(){ 
        
    }
}

#endif