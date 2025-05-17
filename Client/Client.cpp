#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

#define PORT 6969
#define SERVER_EP "127.0.0.1:6969"

using namespace std;
int main()
{
    sockaddr_in address{};
    socklen_t address_len = sizeof(address);
    std::memset(&address, 0, sizeof(address));
    // initializing socket variables
    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    //address.sin_addr.s_addr = inet_addr(SERVER_EP);
    if (inet_pton(AF_INET, "127.0.0.1", &address.sin_addr)
    <= 0) {
    printf(
        "\nInvalid address/ Address not supported \n");
    return -1;
}
    int soc = socket(AF_INET, SOCK_STREAM, 0);
    int n;
    if (n = connect(soc, (sockaddr *)&address, address_len) == -1 )
    {
        std::cerr << "Client failed to connect to the server : " << SERVER_EP << ":" << PORT << std::endl;
        cout << n;
        return 1;
    }

    std::string message = "holla amigo";
    while(1)
    send(soc, message.c_str(), strlen(message.c_str()), 0);
    close(soc);
    return 0;
}
