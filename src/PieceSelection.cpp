#include "PieceSelection.h"
// #include "Peerjoin.cpp"
#include "JSONSerializer.cpp"
// #include "Peerjoin.cpp"
#include <chrono>
using namespace std;

int piece_size = 8192; 
int block_length = piece_size/8;
std::mutex m;
void PieceManager::initialize_to_download(int num_pieces)
{
    //initialize the piece size as well
    std::lock_guard<std::mutex> init_lock(m);
    cout << "initialize downalad" << endl;
    to_download.clear();
    for (int i = 1; i <= num_pieces; ++i)
    {
        char buf[16];
        snprintf(buf, sizeof(buf), "%03d.tmp", i);
        Piece piece;
        piece.piece_id = string(buf);
        piece.status = "not_downloading";
        int j =0; 
        while(j<8){
          //  cout<<"adding blocks to pieces"<<endl;  
            piece.blocks.push_back(block{piece.piece_id,1024*j,1024,"not_downloading"}); 
            j++; 
        }
        to_download.push_back(piece);
        //pushing the pieces inside as well 

    }
}

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
    cout << "To download size : " << to_download.size() << endl;
    while (downloaded.size() < 5)
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
        cout << "Piece to be downloaded : " << to_download[ran].piece_id << endl;
        // std::thread download_thread(&PeerManager::downloadHandler, p, to_download[ran]);
        // download_thread.detach(); // Detach thread to run independently
        p->downloadHandler(to_download[ran]);
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
        cout << "Piece to be downloaded : " << to_download[download_index].piece_id << endl;
        if (download_index != -1)
            //   p->downloadHandler(to_download[download_index], &PieceManager::downloader, this);
            p->downloadHandler(to_download[download_index]);

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
void PieceManager::downloader(string pieceid, block block_info, string data)
{

    // make a file first
    cout<<"Data : "<<data<<endl ; 
    cout << "Download started for file : " << pieceid << endl;
    string filename = "down/" + pieceid + ".tmp";
    ofstream outPutFile(filename, ios::binary | ios::out | ios::trunc);
    if (!outPutFile)
    {
        cerr << "Error creating file : " << filename << endl;
        exit(1);
    }
    int lenght_of_block = block_info.size;
    int chunk_size = 1024;
    int downloaded_files = 0;
    char buffer[1024] = {0};
    int offset = block_info.offset;
    chrono::milliseconds interval;
    // recieve
    // this for for one piece
    // what about folders ??
    // a tree type structure ??
    while (downloaded_files < lenght_of_block)
    {

        auto start = chrono::high_resolution_clock::now();
        //   int size_of_chunk = read(soc, buffer, sizeof(buffer));
        int size_of_chunk = data.size();
        auto finish = chrono::high_resolution_clock::now();
        interval = chrono::duration_cast<chrono::milliseconds>(finish - start);
        // fwrite(buffer , size_of_block, &outPutFile);
        outPutFile.seekp(offset);
        outPutFile.write(data.c_str(), size_of_chunk);
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
