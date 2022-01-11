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

int MAX_users = 5;
int MAX_rooms = 5;

// struct history{
//     char messages[10][1024];
//     int i = 0;
// }

struct validation{
    long type;
    bool is_valid;
} v_msg;

struct msgbuf
{
    long type;
    char text[1024];
} send, receive;

int main(int argc, char *argv[])
{   
    int user_count = 0;
    // printf("%d", user_count);
    // printf("%s", argv[1]);
    
    int string_size = 1024;
    int bool_size = sizeof(bool);
    
    int fd = open("users.txt", O_RDWR | O_CREAT | O_APPEND, 0644);
    // printf("1\n");
    int q_number = atoi(argv[1]);
    printf("1\n");
    // int id = 3;
    int id = msgget(q_number, 0644 | IPC_CREAT);
    // printf("1\n");
    char users[MAX_rooms][20];
    // printf("qweq\n");
    // printf("yuiyu\n");
    // printf("dfg\n");
    char rooms[MAX_rooms][20];
    
    for (int i = 0; i < MAX_users; i++)
    {
        memset(users[i], '\0', strlen(users[i]));
    }

    for (int i = 0; i < MAX_rooms; i++)
    {
        memset(rooms[i], '\0', strlen(rooms[i]));
    }
    // printf("to jednak nie to\n");
    // char commands = {"exit", "enter", "logout", "private", "server", "room", "in_room", "on_server", "help", "history"};

    while (1)
    {
        receive.type = 1;
        msgrcv(id, &receive, string_size, receive.type, 0);
        printf("%s\n", receive.text);
        switch (receive.type)
        {
        case 1:
            if (user_count == MAX_users-2)
            {
                v_msg.is_valid = false;
                v_msg.type = 2;
                msgsnd(id, &v_msg, sizeof(v_msg), 0);
                break;
            }
            else
            {
                v_msg.is_valid = true;
                v_msg.type = 2;
                msgsnd(id, &v_msg, sizeof(v_msg), 0);    
            }
            char c, buf[1024];
            int index = 0;
            bool valid = true;
            lseek(fd, 0, SEEK_SET);
            while ((read(fd, &c, 1)) > 0)
            {
                if (c != '\n')
                {
                    strcpy(&buf[index], &c);
                    index++;
                }
                else
                {
                    if (strcmp(buf, receive.text) == 0)
                    {
                        v_msg.is_valid = false;
                        v_msg.type = 3;
                        msgsnd(id, &v_msg, sizeof(v_msg.is_valid), 0);
                        break;
                    }
                    index = 0;
                    memset(buf, 0, strlen(buf));
                }
            }
            if (valid == true)
            {
                write(fd, receive.text, strlen(receive.text));
                write(fd, '\n', 1);
            }
            break;

        default:
            break;
        }
    }

    return 0;
}
