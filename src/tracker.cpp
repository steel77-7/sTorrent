#include "tracker.h"
#include "JoinHandler.cpp"
#include "JSONSerializer.cpp"
using json = nlohmann::json;

string self_info_hash = "the_super_secret_hash";
int peer_id_gen = 0;


Peer::Peer(int client_socket, socklen_t client_addr_len, sockaddr_in *client_addr, Event *e )
{
    this->client_socket = client_socket;
    this->client_addr = client_addr;
    this->e = e;
}



MessageType Peer::peerMessageSerializer(string s)
{
    json j = json::parse(s);
    json mess;
    if ( j["type"] == "join")
    {
        string str_mess = j["message"];
        json mess = json::parse(str_mess);
        // yaha pe event emit krna hai
        e->emit(mess.get<peerInfo>());
        // retrun pta ni kyu krwa raha
        return mess.get<peerInfo>();
    }
    return j.get<Message>();
}


void Peer::sendMessage() // to be modiefied later
{
    std::cout << "HTTP request received" << std::endl;
    string text_res = "yes here i am " + peer_id_gen;
    write(client_socket, text_res.c_str(), text_res.size());
}

void Peer::sendMessage(Message message)
{
    json j = json{
        {"success", message.success},
        {"type", message.type},
        {"message", message.message},
    };
    string tbs = j.dump();
    write(client_socket, tbs.c_str(), tbs.size());
}

string Peer::recieveMessage()
{
    int val_read = read(client_socket, buffer, sizeof(buffer));
    if (val_read < 0)
        return "";
    string msg(buffer, val_read);
    cout << "message:  " << msg << endl;
    return msg;
}

void Peer::ConnectionHanlder()
{
    while (true)
    {
        string m = recieveMessage();
        if (m == "")
        {
            cout << "message empty" << endl;
            break;
        }
        peerMessageSerializer(m);
    }
    cout << "closed" << endl;
    close(client_socket);
}

// bassic framework set
// yaha pe emit handle hua but baaki peers ko bhejna bhi hai
json arr_j = json ::array();

void update_peer_list(peerInfo info)
{
    // json j;
    peerList.push_back(info); // list is updated no to send it

    for (auto &p : peerList)
    {
        // pushing the peeringo in to the json array
        arr_j.push_back(json{{"info_hash", p.info_hash}, {"ip", p.ip}, {"peer_id", p.peer_id} , {"port"  , p.port}});
        cout << p.info_hash << endl
             << p.ip << endl
             << p.peer_id << endl;
    }
    string updated_list = arr_j.dump();
    for (auto &[id, peer] : connections)
    {

        peer.sendMessage(Message{true, "join", updated_list}); // for now this works
    }
}

int main()
{
    Event event;
    int eventId = event.subscribe(update_peer_list);
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
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    std::cout << "Server is listening and waiting for connection\n";

    // to store the individual connections(ids)...later it will be transformed into a map
    // till now a socket is oppened that will listent to connections
    int next_connection = 0;

    while (1)
    {
        if (connections.size() >= MAX_CONNECTIONS)
        {
            std::cout << "Max connection of " << MAX_CONNECTIONS << "recahed\n";
        }

        sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_socket = accept(server_fd, (sockaddr *)&client_addr, &client_len);

        Peer p(client_socket, client_len, &client_addr, &event);
        connections.insert({peer_id_gen++, p});
        char ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(client_addr.sin_addr), ip, INET_ADDRSTRLEN);
        int port = ntohs(client_addr.sin_port);
        // doing the emitting here
        if (p.client_socket < 0)
        {
            std::cerr << "next\n";
            continue;
        }
        thread t(&Peer::ConnectionHanlder, p);
        t.detach();
    }
    close(server_fd);
    event.unsubscribe(eventId);
    return 0;
}