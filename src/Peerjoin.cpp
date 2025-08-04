#include "Peerjoin.h"
#include <chrono>

using json = nlohmann ::json;

using namespace std;
using Clock = std::chrono::steady_clock;
// will have to feed in the self soc to connect to the diff memebers

/* std::string sha1(const std::vector<char>& data) {
    unsigned char hash[SHA_DIGEST_LENGTH]; // SHA_DIGEST_LENGTH = 20
    SHA1(reinterpret_cast<const unsigned char*>(data.data()), data.size(), hash);
    std::stringstream ss;
    for (int i = 0; i < SHA_DIGEST_LENGTH; ++i)
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    return ss.str(); // return hex string
} */

// unordered_map<string, int> PeerManager::peer_map;

std::mutex mtx;
PeerManager::PeerManager(int *soc, peerInfo p, bool seed)
{
    selfsoc = soc;
    self_info = p;
    this->seed = seed;
}

// just a helper fucntion
peerInfo *PeerManager::get_peer(string peer_id)
{
    return &peer_map[peer_id];
}
// change these cause everytie they connect they will have a new peer info
void PeerManager::add_peer(peerInfo peer) // thsi will eb for an incoming connection request
{
    // std::lock_guard<std::mutex> lock(mtx);
    char buffer[1024];
    sockaddr_in peer_addr;
    socklen_t peer_len = sizeof(peer_addr);
    int soc = socket(AF_INET, SOCK_STREAM, 0);
    try
    {

        int peer_soc = accept(*selfsoc, (sockaddr *)&peer_addr, &peer_len);
        if (peer_soc < 0)
        {
            cerr << "peer cant be connected" << endl;
            return;
        }
        // send_request(peer);
        int val_read = recv(peer_soc, &buffer, sizeof(buffer), 0);
        if (val_read <= 0)
        {
            cerr << "Empty handshake message :" << val_read << endl;
            return;
        }
        // someform of message ahndling and then the handlashake will eb done
        // handlashake logic ko change kr dunga abad mei
        string msg(buffer, val_read);
        if (!hand_shake(msg, "hash"))
        {
            close(peer_soc);
            cout << "Handshake failed" << endl;
            return;
        }
        cout << "Handshake successfull with :" << peer.peer_id << endl;

        peer.socket = peer_soc;
        cout << "Socket fd in add request : " << peer.socket << endl;
        // sending own hash
        string tbs = "hash";
        write(peer_soc, tbs.c_str(), tbs.size());
        if (self_info.peer_id < peer.peer_id)
        {
            cout << "Connection closed form this side (add peer):" << peer.peer_id << endl;
            close(peer_soc);
            return;
        }
        // addign the peer vals ;

        // a lot of bugs but they will be solved
        peer_map.insert({peer.peer_id, peer});

        // create a new  thread for now
        std::thread thread_messages(&PeerManager::message_handler, this, peer.peer_id);
        //  message_handler(peer.peer_id);
        thread_messages.detach();
        for (const auto &[id, peer] : peer_map)
        {
            cout << "Connected to peer: " << id << " " << peer_map.size() << endl;
        }
        vector<string> piece_ids;
        if (ps->downloaded.size() == 0)
        {
            cout << "Downloaded pieces empty" << endl;
        }
        for (const auto p : ps->downloaded)
        {
            piece_ids.push_back(p);
        }
        json j = piece_ids;
        sendMessage(Message{true, "have", j.dump()}, peer_soc);
    }
    catch (exception &e)
    {
        cout << "error in add peer" << endl;
    }
}

void PeerManager::send_request(peerInfo peer)
{
    // std::lock_guard<std::mutex> lock(mtx);

    try
    {

        char buffer[1024];
        sockaddr_in peer_addr;
        socklen_t peer_len = sizeof(peer_addr);
        peer_addr.sin_family = AF_INET;
        peer_addr.sin_addr.s_addr = INADDR_ANY;
        peer_addr.sin_port = htons(peer.port);
        int soc = socket(AF_INET, SOCK_STREAM, 0);

        int peer_soc = connect(soc, (sockaddr *)&peer_addr, peer_len);

        if (peer_soc < 0)
        {
            cerr << "couldnt connect the peer" << endl;
            return;
        }
        string tbs = "hash";
        write(soc, tbs.c_str(), tbs.size()); // if the handshake will fail then close the connection
        int val_read = recv(soc, &buffer, sizeof(buffer), 0);
        int count = 0;
    again:
        if (val_read < 0)
        {
            count++;
            if (count < 10)
                goto again;
            cerr << "handshake empty" << endl;
            return;
        }

        if (self_info.peer_id > peer.peer_id)
        {
            cout << "Connection closed from this side (send request): " << peer.peer_id << endl;

            close(soc);
            return;
        }
        // or just push all this logic into a message handler
        string msg(buffer, val_read);
        cout << "handshake string ::" << msg << endl;
        if (!hand_shake(msg, "hash"))
        {
            cerr << "handshake failed" << endl;
            close(peer_soc);
            return;
        }
        cout << "Handshake successfull with :" << peer.peer_id << endl;
        peer.socket = soc;
        cout << "Socket fd in send request : " << peer.socket << endl;
        std::thread thread_messages(&PeerManager::message_handler, this, peer.peer_id);

        //  message_handler(peer.peer_id);
        thread_messages.detach();
        peer_map.insert({peer.peer_id, peer});
        message_handler(peer.peer_id);
        for (const auto &[id, peer] : peer_map)
        {
            cout << "Connected to peer: " << id << " " << peer_map.size() << endl;
        }
        vector<string> piece_ids;
        if (ps->downloaded.size() == 0)
        {
            cout << "Downloaded pieces empty" << endl;
        }
        for (const auto p : ps->downloaded)
        {
            piece_ids.push_back(p);
        }
        json j = piece_ids;
        sendMessage(Message{true, "have", j.dump()}, soc);
    }
    catch (exception &e)
    {
        cout << "error in send request" << endl;
    }
}

// this iwll be done after the connection is made if false the the connnection will be severed
bool PeerManager::hand_shake(string str, string local_hash)
{
    if (str == local_hash)
        return true;
    return false;
}

// probably use the external message handler
// do do kyu use krne
void PeerManager::message_handler(string peer_id)
{
    char buffer[1024];
    try
    {
        while (true)
        {
            cout << "Peer in message handler" << peer_id << endl;
            peerInfo *p = get_peer(peer_id);
            int soc = p->socket;

            int val = recv(soc, buffer, sizeof(buffer), 0);
            cout << "Size of message and the socket fd in Mess_hand :" << val << " " << soc << endl;
            if (val <= 0)
            {
                cout << "Message handler: message size error from peer: " << peer_id << endl;
                // close(soc);
                return;
            }
            buffer[val] = '\0'; // Null-terminate the buffer
            string msg(buffer);
            json j = json::parse(msg);
            Message m = j.get<Message>();
            cout << "Message from peer :\t\t" << peer_id << m.message << endl;
            // yaha pe mostly piece related loigc lagega
            string type = m.type;
            cout << "Message type" << m.type << endl;
            cout<< "Message : "<<m.message<<endl;
            if (type == "have")
            {
                // do something
                vector<string> pieceList = json::parse(m.message);
                for (const string &piece : pieceList)
                {
                    if (!pieceMap.count(piece))
                    {
                        set<peerInfo *> peers = {get_peer(peer_id)};
                        pieceMap.insert({piece, peers});
                    }
                }
            }

            if (type == "have")
            {
                vector<string> pieceList = json::parse(m.message);
                for (const string &piece : pieceList)
                {
                    if (!pieceMap.count(piece))
                    {

                        set<peerInfo *> peers = {get_peer(peer_id)};
                        pieceMap.insert({piece, peers});
                    }
                    else
                    {
                        set<peerInfo *> p_ids = pieceMap[piece];
                        p_ids.insert(get_peer(peer_id));
                        pieceMap[piece] = p_ids;
                    }
                }
            }
            else if (type == "interested")
            {
                // interested in a piece or connecting
                // setup some logic to check for the right candidate
                // and then
                cout << "Interested peer : " << peer_id << endl;
                peerInfo *peer = get_peer(peer_id);
                // tw::another condition to check the other choking and unchoking criteria of the peers
                if (choke_map.size() >= 5)
                {
                    sendMessage(Message{true, "rejected", ""}, peer->socket);
                    return;
                }
                sendMessage(Message{true, "accepted", ""}, peer->socket);
                // tw :: work to be done here
                peer->choked = false;
                choke_map.insert({peer_id, peer});
            }

            else if (type == "request")
            { // requested piece
                json mess = json::parse(m.message);
                block b = mess.get<block>();
                cout << "Requested piece :" << b.piece_id << endl;
                ifstream inputFile(b.piece_id + "tmp", ios::binary | ios::trunc);
                inputFile.seekg(b.offset);
                int sent = 0;
                int pos = b.offset;
                char buffer[1024] = {0};
                int chunk_size = 1024;
                peerInfo *peer = get_peer(peer_id);
                while (sent < b.size)
                {
                    inputFile.read(buffer, chunk_size);
                    sendMessage(Message{true, "data" , buffer} , soc); 
                    //send(peer->socket, buffer, sizeof(buffer), 0);
                    pos += chunk_size;
                    inputFile.seekg(pos);
                    sent += chunk_size;
                }
                // have
            }
            else if (type == "rejected")
            {
                cout << "Request rejected from peer : " << peer_id << endl;
                return;
            }
            else if(type == "data"){ 
                ps->downloader()
            }
        }
    }
    catch (exception &e)
    {
        cout << "excpetion occured in the message handler " << e.what() << endl;
    }

    // and then  a function to see from all the peeras with piece to download
}

// error prone
void PeerManager::downloadHandler(Piece piece, void (PieceManager::*downloader)(string pieceid, block *block_info, int soc), PieceManager *pm)
{
  //  cout << "DOWNLOAD \t HANDLER" << endl;
    char buffer[1024] = {0};
    vector<block> b = piece.blocks;
    set<peerInfo *> peerList = pieceMap[piece.piece_id];
    int i = 0;
    for (const auto &p : peerList)
    {
        cout << "Sending interested message to the peer : " << p->peer_id << endl;
        sendMessage(Message{true, "interested", ""}, p->socket);
        // error line
        int len = recv(p->socket, buffer, sizeof(buffer), 0);
        if (len < 0)
        {
            cerr << "not interested" << endl;
            return;
        }
        string msg(buffer, len);
        if (msg != "accepted")
        {
            cout << "not accepted" << endl;
            return;
        }

        json j = json{
            {"piece_id", b[i].piece_id},
            {"offset", b[i].offset},
            {"size", b[i].size},
        };
        cout << "Sending requested piece " << piece.piece_id << "to peer :<< " << p->peer_id << endl;
        sendMessage(Message{true, "request", j.dump()}, p->socket);
       // (pm->downloader)(piece.piece_id, &b[i], p->socket);
        i++;
        // make logic for re-requesting
    }
}

void PeerManager::sendMessage(Message m, int soc)
{
    std::lock_guard<std::mutex> lock(mtx);
    cout << "Message being sent : " << m.type << endl;
    json j = json{{"type", m.type}, {"success", m.success}, {"message", m.message}};
    string tbs = j.dump();
    if (soc <= 0)
    {
        cerr << "Socket closed: " << soc << endl;
        return;
    }
    if (send(soc, tbs.c_str(), tbs.size(), 0) < 0)
    {
        cerr << "failed to send message" << endl;
        exit(0);
    }
}

void PeerManager::chokingManager(string peer_id)
{
    // do it on each peer
    // the choking manager will only see over the peer while there connected
    // not initiation of choking
    // it can unchoke them tho
    peerInfo *p = get_peer(peer_id);
    auto prev_time = Clock::now();

    while (true)
    {
        auto now = Clock::now();
        if (std::chrono::duration_cast<chrono::seconds>(now - prev_time).count() >= 30)
        {
            // but i have to chek if data is recieved or not ...... if not then rebuild that piece by downaloding it form different peers
            // tw:: check if the download is finished or not
            p->choked = true;
            choke_map.erase(peer_id);
        }
        prev_time = now;
    }
}

void PeerManager::optimistic_unchoke()
{
    auto prev_time = Clock::now();
    while (true)
    {
        auto now = Clock::now();
        if (std::chrono::duration_cast<chrono::seconds>(now - prev_time).count() >= 30)
        {
            // perform the optimitic unchoke
            int low = 0;
            int high = peer_map.size();
            random_device rd;
            mt19937 gen(rd());
            if (high == 0)
            {
                cerr << "No pieces to download." << endl;
                return;
            }
            uniform_int_distribution<> distrib(low, high - 1);
            int ran = distrib(gen);
            int i = 1;
            for (const auto &[key, val] : peer_map)
            {
                if (ran == i)
                {
                    peerInfo *p = &peer_map[key];
                    p->choked = false;
                }
                i++;
            }
            prev_time = now;
        }
    }
}

void PeerManager::seeder()
{
    cout << "Splitting the file";
    ifstream infile("test.txt", ios::binary);
    int size = 8192;
    int index = 0;
    while (!infile.eof())
    {
        vector<char> buffer(size);
        infile.read(buffer.data(), size);
        streamsize read_size = infile.gcount();
        std::ostringstream filename;
        filename << "test/" << std::setw(3) << std::setfill('0') << index++ << ".tmp";
        string name = filename.str();
        cout<<name<<endl;
        ps->downloaded.push_back(name.substr(5));
        if (read_size > 0)
        {
            buffer.resize(read_size);
            ofstream out(filename.str(), ios::binary);
            // i = i[0]++;
            out.write(buffer.data(), buffer.size());
            out.close();
        }
    }
    cout << "\rFile Divied into pieces by the seeder" << endl;
}
