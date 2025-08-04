#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <thread>
#include <string.h>
#include <cstdlib>
#include "json.hpp"
#include "Peer_id_gen.cpp"
#include "PeerJoinEventHandler.cpp"
#include "Peerjoin.cpp"
#include "PieceSelection.cpp"

using json = nlohmann::json;
// #include "../../lib/nlohmann/json.hpp"
#define PORT 6969
#define SELF_PORT 5001
#define SERVER_EP "127.0.0.1"
#define MAX_CONNECTIONS 10
#define MAX_PEER_CONNECTIONS 5

// transfer the list from client to server and then server to the client
using namespace std;

const string peer_id = peer_id_gen();

string self_info_hash = "the_super_secret_hash";

// one will try to connect...and the other will accept

bool isPeerKnown(const vector<peerInfo> &knownPeers, const peerInfo &peer)
{
    return std::any_of(knownPeers.begin(), knownPeers.end(),
                       [&](const peerInfo &p)
                       { return p.peer_id == peer.peer_id; });
}

vector<peerInfo> peers;
void messageSerializer(string s, Event *add_peer_event, Event *send_request_event)
{
    try
    {
        json j = json::parse(s);
        if (j["type"] == "join")
        {
            // tw::the peer list alaways renews ....fix it
            cout<<j["message"]<<endl;
            string str_mess = j["message"];
            cout << "size of the strimg" << str_mess.size() << endl;
            json mess = json::parse(str_mess);
            vector<peerInfo> peerList = mess.get<vector<peerInfo>>();

            for (const auto &peer : peerList)
            {
                if (peer.peer_id != peer_id && !isPeerKnown(peers, peer))
                {
                    peers.push_back(peer);
                    std::thread t1([&]()
                                   { add_peer_event->emit(peer); });

                    std::thread t2([&]()
                                   { send_request_event->emit(peer); });
                  //  cout << "exited the listner for now" << endl;
                    t1.detach();
                    t2.detach();
                }
            }
        }
    }
    catch (exception &ex)
    {
        cout << "exception occured in the message serializer: " << ex.what() << endl;
    }
}


//this is just the event listenener for the tracker 
void read_message(int self_soc, Event *add_peer_event, Event *send_request_event)
{
    cout << "read messgae" << endl;
    char buffer[2048];
    json j;
    while (1)
    {
        int val_read = read(self_soc, buffer, sizeof(buffer));
        if (val_read < 0)
        {
            cerr << "message ws empty" << endl;
            return;
        }
        string msg(buffer, val_read);
        messageSerializer(msg, add_peer_event, send_request_event);
    }
}

//for the tracker messages
void sendMessage(int s_socket, Message message)
{
    json j = json{
        {"success", message.success},
        {"type", message.type},
        {"message", message.message}};
    string tbs = j.dump();
    write(s_socket, tbs.c_str(), tbs.size());
}

int main(int argc, char *argv[])
{
    int self_port;
    int seeder;

    cout << "Enter the port" << endl;

    cin >> self_port;
    cout << "Enter the seeder val" << endl;

    cin >> seeder;

    Event add_peer_event;
    Event send_request_event;
    char buffer[1024];
    sockaddr_in address{};
    socklen_t address_len = sizeof(address);
    std::memset(&address, 0, sizeof(address));
    // initializing socket variables
    address.sin_family = AF_INET;
    address.sin_port = htons((int)PORT);
    // address.sin_addr.s_addr = inet_addr(SERVER_EP);
    if (inet_pton(AF_INET, SERVER_EP, &address.sin_addr) <= 0)
    {
        cerr << "Invalid address/ Address not supported" << endl;
        return -1;
    }
    int soc = socket(AF_INET, SOCK_STREAM, 0);
    if (soc < 0)
    {
        cerr << "Socket could not be created...." << endl;
        return 2;
    }
    int opt = 1;
    setsockopt(soc, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // conneting ot the server
    int n = connect(soc, (sockaddr *)&address, address_len);
    if (n < 0)
    {
        std::cerr << "Client failed to connect to the server : " << SERVER_EP << ":" << std::endl;
        cout << n << endl;
        return 1;
    }
    int self_soc = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in self_address;
    memset(&self_address, 0, sizeof(self_address));
    self_address.sin_family = AF_INET;
    self_address.sin_addr.s_addr = INADDR_ANY;
    self_address.sin_port = htons(self_port);
    // binding the socket to the port
    if (bind(self_soc, (sockaddr *)&self_address, sizeof(self_address)) < 0)
    {
        cerr << "failed to bind to a port" << endl;
        return 1;
    }

    if (listen(self_soc, MAX_PEER_CONNECTIONS) < 0)
    {
        cerr << "failed to listen";
        return 1;
    }
    char ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(address.sin_addr), ip, INET_ADDRSTRLEN);
    int port = ntohs(address.sin_port);
    json j;
    j = json{{"info_hash", self_info_hash}, {"ip", ip}, {"peer_id", peer_id}, {"port", self_port}};
    string message = j.dump();

    PeerManager peermanager(&self_soc, peerInfo{"127.0.0.1", self_info_hash, peer_id, port}, seeder);
    int add_peer_eventid = add_peer_event.subscribe([&peermanager](peerInfo p)
                                                    { peermanager.add_peer(p); });

    int send_request_eventid = send_request_event.subscribe([&peermanager](peerInfo p)
                                                            { peermanager.send_request(p); });
    sendMessage(soc, Message{true, "join", message});
    thread messageThread([&]()
                         { read_message(soc, &add_peer_event, &send_request_event); });
    PieceManager piecemanager(&peermanager);
    peermanager.ps = &piecemanager;
    if (seeder == 1)
    {
        cout << "seeder processes" << endl;
        peermanager.seeder();
    }
    else
    {
        // tw:: blockiing expresion that woill be cured in the future
        piecemanager.initialize_to_download(127);
        if (piecemanager.initialPieceSelection())
            piecemanager.rarest_piece_selection();
    }

    messageThread.join();

    /////////////////***************** */
    // this can be assigned to someother thread

    close(soc);
    close(self_soc);
    // messageThread.detach();
    return 0;
}

// connect to the different peers
