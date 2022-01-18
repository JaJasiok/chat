#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/time.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <time.h>

#define string_size 1024
#define msg_size 3084
#define bool_size sizeof(bool)

struct tim{
    int hour;
    int minute;
    int sec;
};

struct msgbuf{
  long type;
  struct tim date;
  char sender[string_size];
  char receiver[string_size];
  char text[string_size];
}send, receive;


struct validation{
    long type;
    bool is_valid;
    int user_id;
}v_msg1, v_msg2;



void get_time(struct tim *t){
    time_t rawtime = time(NULL);
    struct tm *pom = localtime(&rawtime);
    t->hour = pom->tm_hour;
    t->minute = pom->tm_min;
    t->sec = pom->tm_sec;
    // printf("godzine\t%d:%d:%d\n", t->hour, t->minute, t->sec);
}

int main(int argc, char *argv[])
{
    // time_t rawtime = time(NULL);
    int user_id;
    int q_number = atoi(argv[1]);

    int id = msgget(q_number, 0644 | IPC_CREAT);

    bool space_available = false;
    bool valid_username = false;

    char str1[string_size];

    while (1)
    {
        
        printf("Enter username: (all characters following the white space will be ignored)\n");
        char str1[string_size];
        scanf("%s", str1);
        int c;
        while ((c = getchar()) != '\n' && c != EOF) { }
        strcpy(send.text, str1);
        send.type = 1;
        memset(send.receiver, 0, string_size);
        memset(send.sender, 0, string_size);
        get_time(&send.date);
        msgsnd(id, &send, msg_size, 0);
        v_msg1.type = 200;
        msgrcv(id, &v_msg1, bool_size, v_msg1.type, 0);
        space_available = v_msg1.is_valid;
        if(space_available == false){
            printf("The server is full\n");
            return 0;
            break;
        }

        v_msg2.type = 300;
        msgrcv(id, &v_msg2, bool_size + sizeof(int), v_msg2.type, 0);
        valid_username = v_msg2.is_valid;
        user_id = v_msg2.user_id;

        if (valid_username == true)
        {
            
            break;
        }
        else
        {
            printf("This username is already taken! Please choose another one.\n");
        }
    }
   
    char username[string_size];
    strcpy(username, send.text);
    


    printf("Enter room name: (all characters following the white space will be ignored)\n");
    scanf("%s", str1);
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
    strcpy(send.text, str1);
    send.type = 1;
    get_time(&send.date);
    msgsnd(id, &send, msg_size, 0);
    printf("Access granted. Welcome!\n");
    printf("For commands type in 'help'\n");

    char str2[string_size];

    if (fork() != 0)
    {
        while (1)
        {
            memset(str1, 0, string_size);
            memset(str2, 0, string_size);
            scanf("%s", str1);
            if(strcmp(str1, "enter") == 0 || strcmp(str1, "logout") == 0 ||  strcmp(str1, "private") == 0 ||  strcmp(str1, "room") == 0 || strcmp(str1, "in_room") == 0 || strcmp(str1, "in_room") == 0)
            {
                scanf("%s", str2);
            }
            while ((c = getchar()) != '\n' && c != EOF) { }
            
            if(strcmp("exit", str1) == 0)
            {
                
                send.type = 2;
                strcpy(send.receiver, username);
                strcpy(send.text, str2);
                strcpy(send.sender, username);
                get_time(&send.date);
                
                msgsnd(id, &send, msg_size, 0);
                sleep(1);
                kill(0, SIGINT);
            }
            
            else if(strcmp("enter", str1) == 0)
            {
                
                send.type = 3;
                strcpy(send.receiver, str2);
                strcpy(send.text, str2);
                strcpy(send.sender, username);
                get_time(&send.date);
                
            }
            
            else if(strcmp("logout", str1) == 0)
            {
                
                send.type = 4;
                strcpy(send.receiver, str2);
                strcpy(send.text, str2);
                strcpy(send.sender, username);
                get_time(&send.date);
                
            }
            
            else if(strcmp("private", str1) == 0)
            {
                
                send.type = 5;
                strcpy(send.receiver, str2);
                memset(str2, 0, string_size);
                
                scanf("%[^\n]%*c", str2);
                
                strcpy(send.text, str2);
                strcpy(send.sender, username);
                get_time(&send.date);
                
            }
            
            else if(strcmp("room", str1) == 0)
            {
                
                send.type = 6;
                strcpy(send.receiver, str2);
                memset(str1, 0, string_size);
                
                scanf("%[^\n]%*c", str2);
                strcpy(send.text, str2);
                strcpy(send.sender, username);
                get_time(&send.date);
               ;
            }
            
            else if(strcmp("rooms", str1) == 0)
            {
                
                send.type = 7;
                strcpy(send.receiver, str2);
                strcpy(send.text, str2);
                strcpy(send.sender, username);
                get_time(&send.date);
                
            }
            
            else if(strcmp("in_room", str1) == 0)
            {
                
                send.type = 8;
                strcpy(send.receiver, str2);
                strcpy(send.text, str2);
                strcpy(send.sender, username);
                get_time(&send.date);
                
            }
            
            else if(strcmp("on_server", str1) == 0)
            {
                
                send.type = 9;
                strcpy(send.receiver, str2);
                strcpy(send.text, str2);
                strcpy(send.sender, username);
                get_time(&send.date);
                
            }
            
            // else if(strcmp("history", str1) == 0)
            // {
                
            //     send.type = 10;
            //     strcpy(send.receiver, str2);
            //     strcpy(send.text, str2);
            //     strcpy(send.sender, username);
            //     get_time(&send.date);
                
            // }
            
            else if(strcmp("help", str1) == 0)
            {
                send.type = -1;
                // pisze komendy i ich wytłumaczenie
                printf("\texit - ends program\n");
                printf("\tenter <roomname> - enters room with a given name if exist, otherwise creates one\n");
                printf("\tlogout <roomname> - exits room with a given name\n");
                printf("\tprivate <username> - sends private message to a user\n");
                printf("\troom <roomname> - sends message to a room\n");
                printf("\trooms - displays all rooms on server\n");
                printf("\tin_room <roomname> - displays all users in room\n");
                printf("\ton_server - displays all users on server\n");
                // printf("\thistory <roomname> - displays last 10 messages from room\n");
            }
            
            else
            {
                send.type = -1;
                printf("Unknown command, try 'help' for help\n");
            }
            

            if (send.type > 1)
            {
                msgsnd(id, &send, msg_size, 0);
                
            }
        }
    }
    else
    {
        while (1)
        {
            
            msgrcv(id, &receive, msg_size, user_id, 0);
            if (strcmp(receive.sender, "server") == 0)
            {
                printf(">> %s\n", receive.text);
            }
            else
            {
                printf("from %s to %s at (%d:%d:%d) : \n%s\n", receive.sender, receive.receiver, receive.date.hour, receive.date.minute, receive.date.sec, receive.text);
            } 
        }
    }





    return 0;
}
