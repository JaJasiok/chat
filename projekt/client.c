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

// int MAX_length = 50;

struct msgbuf
{
    long type;
    char tab[1024];

} send, receive;

struct validation{
    long type;
    bool is_valid;
}v_msg1, v_msg2;

// void clear(){
//     memset(send,'\0',strlen(send));
// }

int main(int argc, char *argv[])
{
    int q_number = atoi(argv[1]);

    int id = msgget(q_number, 0644 | IPC_CREAT);

    bool space_available = false;
    bool valid_username = false;
    
    int string_size = 1024;
    int bool_size = sizeof(bool);

    while (1)
    {
        printf("Enter username:\n");
        char str[1024];
        scanf("%s", str);
        // printf("%s\n", str);
        strcpy(send.tab, str);
        send.type = 1;
        msgsnd(id, &send, strlen(send.tab) + 1, 0)
        // printf("%d\n", msgsnd(id, &send, strlen(send.tab) + 1, 0));
        v_msg1.type = 2;
        msgrcv(id, &v_msg1, bool_size, v_msg1.type, 0);
        space_available = v_msg1.is_valid;
        if(space_available == false){
            printf("The server is full\n");
            break;
        }
        v_msg2.type = 3;
        msgrcv(id, &v_msg2, bool_size, v_msg2.type, 0);
        valid_username = v_msg2.is_valid;
        
        if (valid_username == true)
        {
            printf("Access granted.\n");
            printf("Welcome!\n");
            break;
        }
        else
        {
            printf("This username is already taken! Please choose another one.\n");
        }
    }
    // execlp("clear", "clear", NULL);

    // valid = false;

    printf("Enter room name:\n");
    scanf("%s", send.tab);
    msgsnd(id, &send, strlen(send.tab) + 1, 0);

    return 0;
}
