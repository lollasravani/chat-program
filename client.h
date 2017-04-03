#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <sstream>
//#include <pthread>
#include <curses.h>
#define MAX_LINE 100
#define LINE_ARRAY_SIZE (MAX_LINE+1)

using namespace std;

//int sd;
//char buff[100],US[20];
WINDOW * win1;
WINDOW * win2;
WINDOW * wininp;
WINDOW * winb;

char username[20];
pthread_mutex_t lock;

class Client
{
    private:
        int socketDescriptor;
        unsigned short int serverPort;
        struct sockaddr_in serverAddress;
        struct hostent *hostInfo;
        char c;
    public:
        pthread_t RMsg,SMsg;
        static void* RecMsg(void*);
        static void* SendMsg(void*);
        char buf[LINE_ARRAY_SIZE];
       Client();
        ~Client();
        void curses();
        void ConnectToServer();
        void Input();
        void RecvMessage();
        void SendMessages();
        static void refreshwindows();
};


Client::Client()
{
    char add[LINE_ARRAY_SIZE];
    cout << "Enter server host name or IP address: ";
    cin.get(add, MAX_LINE, '\n');

    hostInfo = gethostbyname(add);
    if (hostInfo == NULL)
    {
        cout << "problem interpreting host: " << add << "\n";
        exit(1);
    }

    cout << "Enter server port number: ";
    cin >> serverPort;
    cin.get(c);
    cout << "Enter User Name: ";
    cin.get(username,20);
    //u = strlen(username);

    socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    //sd = socketDescriptor;
    if (socketDescriptor < 0)
    {
        cerr << "cannot create socket\n";
        exit(1);
    }
    serverAddress.sin_family = hostInfo->h_addrtype;
    memcpy((char *) &serverAddress.sin_addr.s_addr,hostInfo->h_addr_list[0], hostInfo->h_length);
    serverAddress.sin_port = htons(serverPort);


}


void Client::ConnectToServer()
{
    if (connect(socketDescriptor,(struct sockaddr *) &serverAddress,sizeof(serverAddress)) < 0)
    {
        cerr << "cannot connect\n";
        exit(1);
    }

    initscr();
    win1 = newwin(5,80,0,0);

    wininp = newwin(1,50,3,10);

    box(win1, '|', '-');
    mvwprintw(win1,1,2,"Enter message to send");
    mvwprintw(win1,3,2,"Input : ");
    wrefresh(win1);

    winb = newwin(17,80,5,0);
    box(winb, '|', '-');
    wrefresh(winb);


    win2 = newwin(15,76,6,2);
    scrollok(win2, true);
    wrefresh(win2);

    pthread_create(&RMsg, NULL, RecMsg, &socketDescriptor);

}

void Client::Input()
{

    pthread_mutex_lock(&lock);
    wclear(wininp);
    //wrefresh(wininp);
    //sleep(0.5);
    refreshwindows();
    pthread_mutex_unlock(&lock);

    wgetstr(wininp, buf);

}


void Client::SendMessages()
{

    char smsg[MAX_LINE];

    int ulen = strlen(username);
    int mlen = strlen(buf);
    memcpy(smsg, username, ulen);
    memcpy(smsg + ulen, " : ", 3);
    memcpy(smsg+ulen + 3, buf, mlen+1);

    string s;
    //cout << send(socketDescriptor, buf, strlen(buf) + 1, 0);
    if (send(socketDescriptor, smsg, strlen(smsg) + 1, 0) < 0)
    {
        cerr << "cannot send data ";
        close(socketDescriptor);
        exit(1);
    }
}


Client::~Client()
{
    char UL[MAX_LINE];
    int ulen = strlen(username);
    memcpy(UL, username, ulen);
    memcpy(UL+ulen," has left",9);
    send(socketDescriptor, UL, strlen(UL) + 1, 0);
    close(socketDescriptor);
}




void* Client::RecMsg(void* argv)
{


    int sd = *((int *) argv);

    char msg[MAX_LINE];
    while(recv(sd, msg, MAX_LINE, 0))
    {
        pthread_mutex_lock(&lock);
        //mvwprintw(win2,2+i,2,"> %s",msg);
        wprintw(win2,"> %s\n",msg);


        int ix, iy;
        getyx(wininp,iy, ix);
        wmove(wininp, iy, ix);

        //memset(msg, 0x0, LINE_ARRAY_SIZE);

        refreshwindows();
        pthread_mutex_unlock(&lock);
    }

    close(sd);
    exit(1);

}

void Client::refreshwindows()
{
    wrefresh(win2);
    wrefresh(win1);
    wrefresh(winb);
    wrefresh(wininp);
    sleep(0.5);

}
