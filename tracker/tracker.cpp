#include <iostream>
#include <cstring>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <queue>
#define PORT 6969

using namespace std;

/* class peer
{
private:
    struct sockaddr_in address;

public:
    Server()
    {
    }

    void sendRequest()
    {
    }

    void ConnectionHandler()
    {
    }
}; */

int main()
{
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

    if (bind(server_fd, (sockaddr *)&address, sizeof(address) == -1))
    {
        std::cerr << "socket failed to bind...... aborting" << server_fd<<"\n";

        return 1;
    }

    if (listen(server_fd, 10) == -1)
    {
        std::cerr << "error occured while listening.....";
        return 1;
    }

    std::cout << "Server is listening and waiting for connection";

    // to store the individual connections(ids)
    queue<int> connections;

    // till now a socket is oppened that will listent to connections
    while (1)
    {
        // now to connect the peers
        sockaddr_in client_address;
        socklen_t client_address_len = sizeof(client_address);
        int client_fd = accept(server_fd, (sockaddr *)&client_address, &client_address_len);
        if (client_fd == -1)
        {
            std::cerr << "next";
            continue;
        }


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

            write(client_fd , response.c_str(),response.size() );
            close(client_fd);
        }
    return 0;
}