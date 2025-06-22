#include "PieceSelection.h"
#include "JSONSerializer.cpp"
using namespace std;
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

    // downloader(to_download[ran]);
}

void PieceManager::pieceSelection()
{
}

void PieceManager::assembler()
{
}
void PieceManager::downloader(string pieceid, block *block_info, int soc)
{

    // make a file first
    fstream outPutFile(pieceid + ".tmp", ios::binary);
    int lenght_of_block= block_info->size;
    int chunk_size = 1024;
    int downloaded = 0;
    char buffer[1024] = {0};
    int offset = block_info->offset;
    // recieve
    // this for for one piece
    // what about folders ??
    // a tree type structure ??
    while (downloaded < lenght_of_block)
    {
        int size_of_block = read(soc, buffer, sizeof(buffer));
        // fwrite(buffer , size_of_block, &outPutFile);
        outPutFile.seekp(offset);
        offset += chunk_size;
        outPutFile.write(buffer, downloaded);
        downloaded += size_of_block;
    }

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
