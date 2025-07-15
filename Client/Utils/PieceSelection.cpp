#include "PieceSelection.h"
// #include "Peerjoin.cpp"
#include "JSONSerializer.cpp"
// #include "Peerjoin.cpp"
#include <chrono>
using namespace std;
PieceManager::PieceManager(PeerManager *p)
{
    cout << "in the piece manager CONSTRUCTOR" << endl;
    this->p = p;
}

PieceManager::PieceManager()
{
}

bool PieceManager::initialPieceSelection()
{
    cout << p << endl;
    while (downloaded.size() > 5)
    {
        int low = 0;
        int high = to_download.size();
        random_device rd;
        mt19937 gen(rd());
        if (high == 0)
        {
            cerr << "No pieces to download." << endl;
            return false;
        }
        uniform_int_distribution<> distrib(low, high - 1);
        int ran = distrib(gen);
        p->downloadHandler(to_download[ran], &PieceManager::downloader, this);
        // tw::what if the
        to_download.erase(to_download.begin() + ran);
    }
    return true;
}

int find_index(vector<Piece> &to_download, string piece_id)
{
    for (int i = 0; i <= to_download.size(); i++)
    {
        if (to_download[i].piece_id == piece_id)
            return i;
    }
    return -1;
}

void PieceManager::rarest_piece_selection() // depedning upono the rearest fist
{
    cout << "RAREST PIECE SELECTION STARTED" << endl;
    // make some logic to repeat this after every piece is downloaded
    // have a state that will determine initial downalod
    vector<int> piece_count;
    vector<string> p_ids;
    for (const auto &val : p->pieceMap)
    {
        p_ids.push_back(val.first);
        piece_count.push_back(val.second.size());
    }

    auto min_el = min_element(piece_count.begin(), piece_count.end());
    if (min_el != piece_count.end())
    {
        // int min_val = *min_el;
        int index = distance(piece_count.begin(), min_el);
        //  auto download_index = find(to_download.begin(), to_download.end(), p_ids[index]);

        /* if (download_index != to_download.end())
            p->downloadHandler(to_download[distance(to_download.begin(), download_index)], &PieceManager::downloader, this);
        to_download.erase(download_index); */
        int download_index = find_index(to_download, p_ids[index]);

        if (download_index != -1)
            p->downloadHandler(to_download[download_index], &PieceManager::downloader, this);
        to_download.erase(to_download.begin() + download_index);
    }
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
    cout << "Download started for file : " << pieceid << endl;
    fstream outPutFile(pieceid + ".tmp", ios::binary);
    int lenght_of_block = block_info->size;
    int chunk_size = 1024;
    int downloaded_files = 0;
    char buffer[1024] = {0};
    int offset = block_info->offset;
    chrono::milliseconds interval;
    // recieve
    // this for for one piece
    // what about folders ??
    // a tree type structure ??
    while (downloaded_files < lenght_of_block)
    {

        auto start = chrono::high_resolution_clock::now();
        int size_of_chunk = read(soc, buffer, sizeof(buffer));
        auto finish = chrono::high_resolution_clock::now();
        interval = chrono::duration_cast<chrono::milliseconds>(finish - start);
        // fwrite(buffer , size_of_block, &outPutFile);
        outPutFile.seekp(offset);
        outPutFile.write(buffer, size_of_chunk);
        offset += chunk_size;
        downloaded_files += size_of_chunk;
    }
    // do somethign with this

    double speed = sizeof(buffer) / interval.count();
    downloaded.push_back(pieceid);
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
