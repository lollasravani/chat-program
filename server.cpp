#include "server.h"

Server server;

int main()
{
    while (true)
    {
        server.ListenPort();
    }
    return 0;
}
