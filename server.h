#include <arpa/inet.h>
#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <list>

#define MAX_MSG 100
#define LINE_ARRAY_SIZE (MAX_MSG+1)

using namespace std;

struct Client
{
    sockaddr_in clientAddress;
    int clientsocket;
};
typedef list<Client> ClientList;
ClientList clients;

class Server
{
    private:
        int listenSocket, connectSocket, i;
        unsigned short int listenPort;
        socklen_t clientAddressLength;
        struct sockaddr_in clientAddress, serverAddress;
        static void* ListenClient(void*);
    public:
        Server();
        ~Server();
        void ListenPort();
        void SendMessage();
        int RecvMessage();
        char line[LINE_ARRAY_SIZE];
};

Server::~Server()
{
    cout << "Server is closed." << endl;
}



Server::Server()
{
    listenPort = 1600;
//     cout << "Enter port number to listen on (between 1500 and 65000): ";
//     cin >> listenPort;

     listenSocket = socket(AF_INET, SOCK_STREAM, 0);
     if (listenSocket < 0)
     {
        cout << "cannot create listen socket";
        //exit(1);
     }

     serverAddress.sin_family = AF_INET;
     serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
     serverAddress.sin_port = htons(listenPort);

     if (bind(listenSocket,(struct sockaddr *) &serverAddress,sizeof(serverAddress)) < 0)
     {
        cout << "cannot bind socket";
        //exit(1);
     }
     listen(listenSocket,10);
}


void Server::ListenPort()
{

        cout << "Wating for TCP connection on port " << listenPort << "..." << endl;

        clientAddressLength = sizeof(clientAddress);
        connectSocket = accept(listenSocket,(struct sockaddr *) &clientAddress,&clientAddressLength);

        if (connectSocket < 0)
        {
            cout << "cannot accept connection ";
            //exit(1);
        }

        cout << "  connected to " << inet_ntoa(clientAddress.sin_addr);

        cout << ":" << ntohs(clientAddress.sin_port) << endl;

        Client client;
        client.clientAddress = clientAddress;
        client.clientsocket = connectSocket;

        clients.push_back(client);

        pthread_t recthread;


        int suc = pthread_create(&recthread, NULL, ListenClient, &client);



}


int Server::RecvMessage()
{
    return (recv(connectSocket, line, MAX_MSG, 0));
}


void Server::SendMessage()
{
    if (send(connectSocket, line, strlen(line) + 1, 0) < 0)
    {
        cout << "Error: cannot send modified data";
    }
}

void* Server::ListenClient(void* argv)
{

    ClientList tclients;
    //tclients = clients;


    Client client = *((Client *) argv);
    int clientsocket = client.clientsocket;


    cout << "Thread started " << endl;
    char msg[LINE_ARRAY_SIZE];
    while (recv(clientsocket, msg, MAX_MSG, 0))
    {
        list<Client>::iterator itclient;

        cout << msg << endl;
//        if (send(clientsocket, msg, strlen(msg) + 1, 0) < 0)
//        {
//
//            cout << clientsocket  << " - Connection Closed" << endl;
//        }


        for(itclient = clients.begin(); itclient != clients.end(); ++itclient)
        {
            int curclientsocket = (*itclient).clientsocket;
            if (send(curclientsocket, msg, strlen(msg) + 1, 0) < 0)
            {
                //cerr << "Error: cannot send modified data";
                cout << curclientsocket << " - Connection Closed" << endl;
                clients.erase(itclient);
                ++itclient;
            }

        }

    }
}
