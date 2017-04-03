#include<iostream>
#include <curses.h>

using namespace std;

int main()
{
    initscr();
    WINDOW * win1 = newwin(20,40, 0,0);
    box(win1, '|','-');
    //mvwprintw(win1, 2,2, "Hi this is Sravani");
    wprintw(win1,"HELLO");
    //wprintw(win1,"Hii this is Sravani.");
    wrefresh(win1);
}

