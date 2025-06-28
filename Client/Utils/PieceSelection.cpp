#include "PieceSelection.h"
#include "JSONSerializer.cpp"
#include <chrono>
using namespace std;
PieceManager::PieceManager(PeerManager *p)
{
    this->p = p;
}

PieceManager::PieceManager()
{
}

void PieceManager::initialPieceSelection()
{

    int low = 0;
    int high = to_download.size();
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(low, high);
    int ran = distrib(gen);
    // then ask the peer manager to do the shit
    PieceManager pm;
    p->downloadHandler(to_download[ran], downloader, &pm);
    // downloader(to_download[ran]);
}

void PieceManager::pieceSelection() // depedning upono the rearest fist
{
}

void PieceManager::assembler()
{
    // piece assembler
    // all the temp files of the directory need to be assembled
    //  so first fdwonalod them
    /*
    and then assemlbe them
    if a piece has downaloded then do it
    */
}
void PieceManager::downloader(string pieceid, block *block_info, int soc)
{

    // make a file first
    fstream outPutFile(pieceid + ".tmp", ios::binary);
    int lenght_of_block = block_info->size;
    int chunk_size = 1024;
    int downloaded = 0;
    char buffer[1024] = {0};
    int offset = block_info->offset;
    chrono::milliseconds interval;
    // recieve
    // this for for one piece
    // what about folders ??
    // a tree type structure ??
    while (downloaded < lenght_of_block)
    {
        auto start = chrono::high_resolution_clock::now();
        int size_of_chunk = read(soc, buffer, sizeof(buffer));
        auto finish = chrono::high_resolution_clock::now();
        interval = chrono::duration_cast<chrono::milliseconds>(finish - start);
        // fwrite(buffer , size_of_block, &outPutFile);
        outPutFile.seekp(offset);
        outPutFile.write(buffer, size_of_chunk);
        offset += chunk_size;
        downloaded += size_of_chunk;
    }
    // do somethign with this

    double speed = sizeof(buffer) / interval.count();

    /*     json j = json{
            {"type", "download"},
            {"success", 200},
            {"message", pieceid}};

        string ready_tbs = j.dump();

        if (send(soc, ready_tbs.c_str(), sizeof(ready_tbs), 0) < 0)
        {
            cerr << "error in request" << endl;
            return;
        }
        char buffer[1024];
        int val = recv(soc, &buffer, sizeof(buffer), 0);

        if(val< 0){
            cerr<<"handshake not done"<<endl;
            return ;
        }

        string msg (buffer,val);  */

    //^ ^ ^ ^ ^^ ^ ^ ^ ^ ^ ^^^^^^^^^^^^^^^^^^^^
    // to be done in the message handler

    /*
    assemble the file here
    in blocks
    chunks
    */
}
