#include "../../lib/Peerjoin.h"
using namespace std;
// will have to feed in the self soc to connect to the diff memebers
PeerManager::PeerManager(int soc,peerInfo  p)
{
    this->selfsoc = soc;
    this->self_info  = p; 
}

void PeerManager::add_peer(peerInfo peer) // thsi will eb for an incoming connection request
{
    char buffer[1024];
    sockaddr_in peer_addr;
    socklen_t peer_len = sizeof(peer_addr);

    int peer_soc = accept(selfsoc, (sockaddr *)&peer_addr, &peer_len);
    if (peer_soc < 0)
    {
        cerr << "peer cant be connected" << endl;
        return;
    }
    int val_read = recv(peer_soc, &buffer, sizeof(buffer), 0);
    if (val_read < 0)
    {
        cerr << "empty message" << endl;
        return;
    }
    // someform of message ahndling and then the handlashake will eb done
    // handlashake logic ko change kr dunga abad mei
    string msg(buffer, val_read);
    if (!hand_shake(msg, "some_hash rn "))
    {
        close(peer_soc);
        cout << "handshake failed" << endl;
        return;
    }

    //sending own hash 
    string tbs = "hash";
    write(peer_soc, tbs.c_str(), tbs.size());
    //agr cu
    if(self_info.peer_id< peer.peer_id){ 
        close(peer_soc); 
        return ; 
    }
    //a lot of bugs but they will be solved
    peer_map.insert({peer.peer_id, peer});
}

void PeerManager::send_request(peerInfo peer)
{
    char buffer[1024];
    sockaddr_in peer_addr;
    socklen_t peer_len = sizeof(peer_addr);
    peer_addr.sin_family = AF_INET;
    peer_addr.sin_addr.s_addr = INADDR_ANY;
    peer_addr.sin_port = htons(peer.port);
    int peer_soc = connect(selfsoc, (sockaddr *)&peer_addr, peer_len);
    if (peer_soc < 0)
    {
        cerr << "couldnt connect the peer" << endl;
        return;
    }
    string tbs = "hash";
    write(peer_soc, tbs.c_str(), tbs.size()); // if the handshake will fail then close the connection
    int val_read = recv(peer_soc, &buffer, sizeof(buffer), 0);
    if (val_read < 0)
    {
        cerr << "handshake empty" << endl;
        return;
    }
    // or just push all this logic into a message handler
    string msg(buffer, val_read);
    if (hand_shake(msg, "hash"))
    {
        cerr << "handshake failed" << endl;
        close(peer_soc);
        return;
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
void PeerManager::message_handler()
{
}
