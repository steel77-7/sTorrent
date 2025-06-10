#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <thread>
#include <string.h>
#include <cstdlib>
#include "../../lib/json.hpp"
#include "../Utils/Peer_id_gen.cpp"
#include "../Utils/PeerJoinEventHandler.cpp"

using json = nlohmann::json;
// #include "../../lib/nlohmann/json.hpp"
#define PORT 6969
#define SERVER_EP "127.0.0.1"
#define MAX_CONNECTIONS 10
#define MAX_PEER_CONNECTIONS 5 

// transfer the list from client to server and then server to the client
using namespace std;

struct peerInfo
{
    string ip;
    string info_hash;
    string peer_id;
    //  int port;
    //  string event;
};

struct Message
{
    bool success;
    string type;
    string message;
};
vector<peerInfo> peerList;
// nlohmann fucntion
void from_json(const json &j, peerInfo &p)
{
    j.at("ip").get_to(p.ip);
    j.at("peer_id").get_to(p.peer_id);
    j.at("info_hash").get_to(p.info_hash);
}

void to_json(json &j, peerInfo p)
{
    j = json{{"info_hash", p.info_hash},
             {"ip", p.ip},
             {"peer_id", p.peer_id}};
}
void to_json(json &j, Message &m)
{
    j = json{
        {"success", m.success},
        {"type", m.type},
        {"message", m.message},
    };
}

string self_info_hash = "the_super_secret_hash";

// one will try to connect...and the other will accept

void messageSerializer(string s)
{
    json j = json::parse(s);
    if (j["type"] == "join")
    {
        string str_mess = j["message"];
        cout << str_mess << endl;
        json mess = json::parse(str_mess);
        vector<peerInfo> peerList = mess.get<vector<peerInfo>>();
    }
}

void read_message(int self_soc, int server_soc)
{
    cout << "read messgae" << endl;
    char buffer[1024];
    json j;
    while (1)
    {
        cout << 1 << endl;
        int val_read = read(self_soc, buffer, sizeof(buffer));
        if (val_read < 0)
        {
            cerr << "message ws empty" << endl;
            return;
        }
        string msg(buffer, val_read);
        cout << "message :" << msg << endl;
       // j = json::parse(msg);
       // vector<peerInfo> peerList = j.get<vector<peerInfo>>();
       messageSerializer(msg);
    }
}

void sendMessage(int s_socket, Message message)
{
    json j = json{
        {"success", message.success},
        {"type", message.type},
        {"message", message.message}};
    string tbs = j.dump();
    cout << "send message" <<tbs<< endl;
    write(s_socket, tbs.c_str(), tbs.size());
}

void connect_to_peer(int soc, string ip)
{
    sockaddr_in peer_address;
    peer_address.sin_family = AF_INET;
    // peer_address.sin_port = peerInfo->port;
    //  peer_address.sin_addr.s_addr= inet_pton(peerInfo->address);

    if (connect(soc, (sockaddr *)&peer_address, (socklen_t)sizeof(peer_address)) < 0)
    {
        cerr << "failed to connect  to peer";
        return;
    }
}

bool hand_shake(string peer_info_hash)
{
    return self_info_hash.compare(peer_info_hash);
}

void accept_connection(int peer_soc)
{
    char buffer[1024];
    // int peer_soc;
    while (int rec = recv(peer_soc, &buffer, sizeof(buffer), 0))
    {
        // hand_shake()
        // here the info hash will be recieved and then it will eb checked with the available info hash
        cout << "curr thread" << this_thread::get_id() << endl;
    }
    close(peer_soc);
}

int main()
{
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
    // binding the socket to the port

    // conneting ot the server
    int n = connect(soc, (sockaddr *)&address, address_len);
    if (n < 0)
    {
        std::cerr << "Client failed to connect to the server : " << SERVER_EP << ":" << std::endl;
        cout << n << endl;
        return 1;
    }

    char ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(address.sin_addr), ip, INET_ADDRSTRLEN);
    int port = ntohs(address.sin_port);
    json j;
    j = json{{"info_hash", self_info_hash}, {"ip", ip}, {"peer_id", peer_id_gen()}};
    string message = j.dump();

    sendMessage(soc, Message{true, "join", message});
    read_message(soc, n);

    /////////////////***************** */
    // this can be assigned to someother thread

    close(soc);
    return 0;
}

// connect to the different peers
