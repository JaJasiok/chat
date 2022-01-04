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
int MAX_users = 5;
int MAX_rooms = 5

struct history{
    char messages[10][MAX_length];
    int i = 0;
}

struct msgbuf{
    long type;
    char text[MAX_length];
}







int main(int argc, char*argv[]){

    char users[MAX_users][20];
    char rooms[MAX_rooms][20];

    for(int i = 0; i < MAX_users){
        memset(users[i],'\0',strlen(users[i]));
    }

    for(int i = 0; i < MAX_rooms){
        memset(rooms[i],'\0',strlen(rooms[i]));
    }

    char commands = {"exit", "enter", "logout", "private", "server", "room", "in_room", "on_server", "help", "history"};

    return 0;
}