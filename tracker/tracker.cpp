#include <iostream>
#include <cstring>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <queue>
#include <map>
#define MAX_CONNECTIONS 10
#define PORT 6969

using namespace std;

class Peer
{
private:
    sockaddr_in client_addr;
    socklen_t client_addr_len;

public:
    int client_fd;
    Peer(int server_fd)
    {
        client_addr_len = sizeof(client_addr);
        client_fd = accept(server_fd, (sockaddr *)&client_addr, &client_addr_len);
    }

    void sendRequest()
    {
    }

    void ConnectionHandler()
    {
        // the magic happens here
    }
};

int main()
{
    cout << "hello";
    // socket is created here but not alloted.....
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1)
    {
        perror("Socket failed to connect....\raborting.....");
        return 1;
    }

    sockaddr_in address;
    std::memset(&address, 0, sizeof(address)); // giving it a space in memeory
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (sockaddr *)&address, sizeof(address)) == -1)
    {
        std::cerr << "socket failed to bind...... aborting" << server_fd << "\n";

        return 1;
    }

    if (listen(server_fd, 10) == -1)
    {
        std::cerr << "error occured while listening.....";
        return 1;
    }

    std::cout << "Server is listening and waiting for connection";

    // to store the individual connections(ids)...later it will be transformed into a map
    std::map<int, Peer> connections;
    // till now a socket is oppened that will listent to connections
    while (1)
    {
        // now to connect the peers
        // the peer id will be managed afterwards
        int next_connection = 0 ;
        Peer p(server_fd);
        if (p.client_fd == -1)
        {
            std::cerr << "next";
            continue;
        }
        if(connections.size()>= MAX_CONNECTIONS){ 
            std::cout<<"Max connection of "<<MAX_CONNECTIONS<<"recahed\n";
        }
        // psuhing the connection in it
        connections.insert({p.client_fd, p});

        std::cout << "HTTP request received" << std::endl;
        // Send the HTTP response
        std::string response_body = "<html><body><h1>Hello, World!</h1></body></html>";
        std::string response =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: " +
            std::to_string(response_body.size()) + "\r\n"
                                                   "\r\n" +
            response_body;

        write(p.client_fd, response.c_str(), response.size());
        close(p.client_fd);
    }
    return 0;
}