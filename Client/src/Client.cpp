#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <thread>

#define PORT 6969
#define SERVER_EP "127.0.0.1:6969"
#define MAX_CONNECTIONS 10

using namespace std;

struct peerInfo
{
    string info_hash;
    string peer_id;
    int port;
    string event;
};

// one will try to connect ...and the other will accept

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

void accept_connection()
{
    char buffer[1024]; 
    int peer_soc ;
    while (int rec = recv(peer_soc , &buffer , sizeof(buffer),0))
    {
        cout<<"curr thread" << this_thread::get_id()<<endl;
    }
    close (peer_soc);
}

void peer_handler(int self_soc, int peer_soc, socklen_t peer_len)
{
    while (1)
    {
    }
}

int main()
{
    sockaddr_in address{};
    socklen_t address_len = sizeof(address);
    std::memset(&address, 0, sizeof(address));
    // initializing socket variables
    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    // address.sin_addr.s_addr = inet_addr(SERVER_EP);
    if (inet_pton(AF_INET, "127.0.0.1", &address.sin_addr) <= 0)
    {
        printf(
            "\nInvalid address/ Address not supported \n");
        return -1;
    }
    int soc = socket(AF_INET, SOCK_STREAM, 0);
    int n;
    if (n = connect(soc, (sockaddr *)&address, address_len) == -1)
    {
        std::cerr << "Client failed to connect to the server : " << SERVER_EP << ":" << PORT << std::endl;
        cout << n;
        return 1;
    }

    if (bind(soc, (sockaddr *)&address, sizeof(address)) < 0)
    {
        cerr << "socket failed to bind" << endl;
        return 1;
    }
    if (listen(soc, MAX_CONNECTIONS) < 0)
    {
        cerr << "failed to listen to hosts";
        return 1;
    }
    while (1)
    {
        // the code for individual peer
    }
    /*   std::string message = "holla amigo";
      while (1)
          send(soc, message.c_str(), strlen(message.c_str()), 0); */
    close(soc);
    return 0;
}

// connect to the different peers
