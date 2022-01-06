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

struct msgbuf{
    long type;
    char tab[MAX_length];
    
} send, receive;



void clear(){
    memset(send,'\0',strlen(send));
}




int main(int argc, char*argv[]){

    int id = mssget(argv[1], 0644 | IPC_CREAT);

    send.type = 0;

    bool valid = False;

    while(1){
        printf("Enter username:");
        scanf("%s", &send.tab);
        msgsnd(id, &send, strlen(send.tab)+1, 0);
        if(valid = True){
            break;
        }
        else{
            printf("This username is already taken! Please choose another one.")
        }
    }
    execlp("clear", "clear", NONE)
    
    valid = False;
    
    printf("Enter room name:");
    scanf("%s", &send.tab);
    msgsnd(id, &send, strlen(send.tab)+1, 0);

    return 0;
}