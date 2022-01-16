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

#define string_size 1024
#define msg_size 3072
#define bool_size sizeof(bool)

struct msgbuf{
  long type;
  // data
  char sender[string_size];
  char receiver[string_size];
  char text[string_size];
}send, receive;

// struct msgbuf
// {
//     long type;
//     char text[string_size];
// } send, receive;

struct validation{
    long type;
    bool is_valid;
    int user_id;
}v_msg1, v_msg2;

// void clear(){
//     memset(send,'\0',strlen(send));
// }

int main(int argc, char *argv[])
{
    int user_id;
    int q_number = atoi(argv[1]);

    int id = msgget(q_number, 0644 | IPC_CREAT);

    bool space_available = false;
    bool valid_username = false;

    char str1[string_size];

    while (1)
    {
        printf("Enter username:\n");
        char str1[string_size];
        scanf("%s", str1);
        printf("%s\n", str1);
        strcpy(send.text, str1);
        send.type = 1;
        memset(send.receiver, 0, string_size);
        memset(send.sender, 0, string_size);
        msgsnd(id, &send, msg_size, 0);
        // printf("%d\n", msgsnd(id, &send, msg_size, 0));
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
            printf("Access granted.\n");
            printf("Welcome!\nYour Id is %d\n", user_id);
            break;
        }
        else
        {
            printf("This username is already taken! Please choose another one.\n");
        }
    }
    // execlp("clear", "clear", NULL);

    // valid = false;

    char username[string_size];
    strcpy(username, send.text);
    printf("%s\n", username);


    printf("Enter room name:\n");
    scanf("%s", str1);
    strcpy(send.text, str1);
    send.type = 1;
    msgsnd(id, &send, msg_size, 0);

    char str2[string_size];

    if (fork() != 0)
    {
        while (1)
        {
            memset(str1, 0, string_size);
            memset(str2, 0, string_size);
            // scanf("%s %s",  str1, str2);
            scanf("%s", str1);
            if(strcmp(str1, "exit") != 0 && strcmp(str1, "on_server") != 0 &&  strcmp(str1, "help") != 0 &&  strcmp(str1, "rooms") != 0){
                scanf("%s", str2);
            }
            
            
            if(strcmp("exit", str1) == 0)
            {
                // scanf("%s", str2);
                send.type = 2;
                strcpy(send.receiver, username);
                strcpy(send.text, str2);
                strcpy(send.sender, username);
                msgsnd(id, &send, msg_size, 0);
                sleep(1);
                kill(0, SIGKILL);
            }
            
            else if(strcmp("enter", str1) == 0)
            {
                // scanf("%s", str2);
                send.type = 3;
                strcpy(send.receiver, str2);
                strcpy(send.text, str2);
                strcpy(send.sender, username);
            }
            
            else if(strcmp("logout", str1) == 0)
            {
                // scanf("%s", str2);
                send.type = 4;
                strcpy(send.receiver, str2);
                strcpy(send.text, str2);
                strcpy(send.sender, username);
            }
            
            else if(strcmp("private", str1) == 0)
            {
                // scanf("%s", str2);
                send.type = 5;
                strcpy(send.receiver, str2);
                memset(str2, 0, string_size);
                // // scanf("%[^\n]%*c", str2);
                // fflush(stdin);
                // for (int i = 0; i < string_size-1; i++)
                // {
                //     scanf("%c", &str2[i]);
                //     if (str2[i] == '\n')
                //     {
                //         break;
                //     }  
                // }
                scanf("%s", str2);
                // fgets(str2, 1024, stdin);
                strcpy(send.text, str2);
                strcpy(send.sender, username);
            }
            
            else if(strcmp("room", str1) == 0)
            {
                // scanf("%s", str2);
                send.type = 6;
                strcpy(send.receiver, str2);
                memset(str1, 0, string_size);
                scanf("%s", str2);
                strcpy(send.text, str2);
                strcpy(send.sender, username);
            }
            
            else if(strcmp("rooms", str1) == 0)
            {
                // scanf("%s", str2);
                send.type = 7;
                strcpy(send.receiver, str2);
                strcpy(send.text, str2);
                strcpy(send.sender, username);
            }
            
            else if(strcmp("in_room", str1) == 0)
            {
                // scanf("%s", str2);
                send.type = 8;
                strcpy(send.receiver, str2);
                strcpy(send.text, str2);
                strcpy(send.sender, username);
            }
            
            else if(strcmp("on_server", str1) == 0)
            {
                // scanf("%s", str2);
                send.type = 9;
                strcpy(send.receiver, str2);
                strcpy(send.text, str2);
                strcpy(send.sender, username);
            }
            
            else if(strcmp("history", str1) == 0)
            {
                // scanf("%s", str2);
                send.type = 10;
                strcpy(send.receiver, str2);
                strcpy(send.text, str2);
                strcpy(send.sender, username);
            }
            
            else if(strcmp("help", str1) == 0)
            {
                send.type = -1;
                // pisze komendy i ich wytłumaczenie
            }
            
            else
            {
                send.type = -1;
                printf("Unknown command, try \"help\" for help\n");
            }
            

            if (send.type > 1)
            {
                msgsnd(id, &send, msg_size, 0);
                // printf("%d\n", msgsnd(id, &send, msg_size, 0));
                // printf("received username:\t%s\n", send.text);
                // printf("received username:\t%s\n", send.sender);
                // printf("received username:\t%s\n", send.receiver);
            }
        }
    }
    else
    {
        while (1)
        {
            // printf("%ld\n", msgrcv(id, &receive, msg_size, user_id, 0));
            msgrcv(id, &receive, msg_size, user_id, 0);
            if (strcmp(receive.sender, "server") == 0)
            {
                printf("%s\n", receive.text);
            }
            else
            {
                printf("from %s to %s: \n%s\n", receive.sender, receive.receiver, receive.text);
            } 
        }
    }





    return 0;
}
