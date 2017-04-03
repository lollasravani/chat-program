
#include "client.h"


Client client;


int main()
{
    cout << "Chat Client is starting...";
    client.ConnectToServer();
    while(true)
    {
    //cout << "Loop Running";
        client.Input();
        client.SendMessages();
        memset(client.buf, 0x0, LINE_ARRAY_SIZE);
        //clienti.RecvMessage();
    }
    return 0;
}
