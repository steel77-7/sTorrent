#include "../../lib/Peerjoin.h"
using namespace std;
// will have to feed in the self soc to connect to the diff memebers

unordered_map<string, int> PeerManager::peer_map;
PeerManager::PeerManager(int *soc, peerInfo p)
{
    selfsoc = soc;
    self_info = p;
}

void PeerManager::add_peer(peerInfo peer) // thsi will eb for an incoming connection request
{
    cout << "int hte add peer" << endl;
    char buffer[1024];
    sockaddr_in peer_addr;
    socklen_t peer_len = sizeof(peer_addr);
    int soc = socket(AF_INET, SOCK_STREAM, 0);

    int peer_soc = accept(*selfsoc, (sockaddr *)&peer_addr, &peer_len);
    if (peer_soc < 0)
    {
        cerr << "peer cant be connected" << endl;
        return;
    }
    // send_request(peer);
    int val_read = recv(peer_soc, &buffer, sizeof(buffer), 0);
    if (val_read < 0)
    {
        cerr << "empty message" << endl;
        return;
    }
    // someform of message ahndling and then the handlashake will eb done
    // handlashake logic ko change kr dunga abad mei
    string msg(buffer, val_read);
    if (!hand_shake(msg, "hash"))
    {
        close(peer_soc);
        cout << "handshake failed" << endl;
        return;
    }
    cout << "handshake successfull" << endl;

    // sending own hash
    string tbs = "hash";
    write(peer_soc, tbs.c_str(), tbs.size());
    // agr cu
    if (self_info.peer_id < peer.peer_id)
    {
        close(peer_soc);
        return;
    }
    // a lot of bugs but they will be solved
    peer_map.insert({peer.peer_id, soc});
    cout << endl;

    for (const auto &[id, peer] : peer_map)
    {
        cout << id << endl;
    }
    cout << endl;
}

void PeerManager::send_request(peerInfo peer)
{
    cout << "send request" << endl;

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
    // or just push all this logic into a message handler
    string msg(buffer, val_read);
    cout << "handshake string ::" << msg << endl;
    if (!hand_shake(msg, "hash"))
    {
        cerr << "handshake failed" << endl;
        close(peer_soc);
        return;
    }
    cout << "handshake successfull" << endl;
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
void PeerManager::message_handler(Message m, peerInfo p)
{
    // yaha pe mostly piece related loigc lagega
    string type = m.type;
    if (type == "have")
    {
        if (!pieceMap.count(m.message))
        {
            vector<peerInfo> tp = {p};
            pieceMap.insert({m.message, tp});
        }
        vector<peerInfo> tp = pieceMap[m.message];
        tp.push_back(p);
        // do something
    }
    else if (type == "interested")
    {
        // interested in a piece or connecting
        // setup some logic to check for the right candidate
        // and then
        sendMessage(Message{true, "accepted", ""}, p.socket);
    }

    else if (type == "request")
    { // requested piece

        json mess = json::parse(m.message);
        block b = mess.get<block>();
        ifstream inputFile(b.piece_id + "tmp", ios::binary | ios::trunc);
        inputFile.seekg(b.offset);
        int sent = 0;
        int pos = b.offset;
        char buffer[1024] = {0};
        int chunk_size = 1024;
        while (sent < b.size)
        {
            inputFile.read(buffer, chunk_size);
            send(p.socket, buffer, sizeof(buffer), 0);
            pos += chunk_size;
            inputFile.seekg(pos);
            sent += chunk_size;
        }
        // have
    }
    // and then  a function to see from all the peeras with piece to download
}

// error prone
void PeerManager::downloadHandler(Piece piece)
{ // maybe pass a call back for it
    char buffer[1024] = {0};
    vector<block> b = piece.blocks;
    vector<peerInfo> peerList = pieceMap[piece.piece_id];
    int i = 0;
    for (const auto &p : peerList)
    {

        sendMessage(Message{true, "interested", ""}, p.socket);
        // error line
        int len = recv(p.socket, buffer, sizeof(buffer), 0);
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
        sendMessage(Message{true, "request", j.dump()}, p.socket);
        i++;
        // make logic for re-requesting
    }
}

void PeerManager::sendMessage(Message m, int soc)
{
    json j = json{{"type", m.type}, {"success", m.success}, {"message", m.message}};
    string tbs = j.dump();
    if (send(soc, tbs.c_str(), sizeof(tbs), 0) < 0)
    {
        cerr << "failed to send message" << endl;
        return;
    }
}
