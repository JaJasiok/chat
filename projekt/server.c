#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<sys/time.h>

int MAX_length = 50;

struct history{
    char messages[10][MAX_length];
    int i = 0;
}

struct msgbuf{
    long type;
    char text[MAX_length];
}

int main(int argc, char*argv[]){

    char users[5][20];
    char rooms[5][20];

    int users_count = 0;
    int rooms_count = 0;

    char commands = {"exit", "enter", "logout", "private", "server", "room", "in_room", "on_server", "help", "history"};

    return 0;
}