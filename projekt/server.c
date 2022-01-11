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

struct validation
{
    long type;
    bool is_valid;
} v_msg;

struct msgbuf
{
    long type;
    char text[1024];
} send, receive;

struct users
{
    char usernames[5][1024];
    int ids[5];
    
} users_list;



int main(int argc, char *argv[])
{   
    for (int i = 0; i < MAX_users; i++)
    {
        memset(users_list.usernames[i], 0, 1024);
    }
    int user_count = 0;
    // printf("%d", user_count);
    // printf("%s", argv[1]);
    
    int string_size = 1024;
    int bool_size = sizeof(bool);
    // char endl[] = "\n";
    
    // int *fd = malloc(2*sizeof(int));
    
    // fd[0] = open("users.txt", O_RDWR | O_CREAT | O_APPEND, 0644);
    // fd[1] = open("rooms.txt", O_RDWR | O_CREAT | O_APPEND, 0644);
    
    
    // size_t myarray_size = 1000;
    // int* myarray = malloc(myarray_size * sizeof(int));

    // myarray_size += 1000;
    // int* myrealloced_array = realloc(myarray, myarray_size * sizeof(int));
    // if (myrealloced_array) {
    //     myarray = myrealloced_array;
    // }else {
    //     printf("nie dziala");
    //     free(myarray);
    // // deal with realloc failing because memory could not be allocated.
    // }
    // free(myarray);
    
    // printf("1\n");
    int q_number = atoi(argv[1]);
    // printf("1\n");
    // int id = 3;
    int id = msgget(q_number, 0644 | IPC_CREAT);
    // printf("1\n");
    // char users[MAX_rooms][20];
    // printf("qweq\n");
    // printf("yuiyu\n");
    // printf("dfg\n");
    // char rooms[MAX_rooms][20];
    
    // for (int i = 0; i < MAX_users; i++)
    // {
    //     memset(users[i], '\0', strlen(users[i]));
    // }

    // for (int i = 0; i < MAX_rooms; i++)
    // {
    //     memset(rooms[i], '\0', strlen(rooms[i]));
    // }
    // printf("to jednak nie to\n");
    // char commands = {"exit", "enter", "logout", "private", "room", "rooms", "in_room", "on_server", "help", "history"};

    while (1)
    {
        // receive.type = 1;
        msgrcv(id, &receive, string_size, -100, 0);
        printf("Received username:\t%s\n", receive.text);
        printf("Received type:\t%ld\n", receive.type);
        switch (receive.type)
        {
        case 1:
            // printf("case one");
            if (user_count == MAX_users)
            {
                v_msg.is_valid = false;
                v_msg.type = 200;
                msgsnd(id, &v_msg, sizeof(v_msg.is_valid), 0);
                break;
            }
            else
            {
                v_msg.is_valid = true;
                v_msg.type = 200;
                msgsnd(id, &v_msg, sizeof(v_msg.is_valid), 0);
                
            }
            // char c, buf[1024];
            // int index = 0;
            bool valid = true;
            // printf("to ten pierwszy");
            // for (int i = 0; i < MAX_users; i++)
            // {
            //    printf("%s\n", users_list.usernames[i]);
            // }
            
            
            for (int i = 0; i < MAX_users; i++)
            {
                if(strcmp(receive.text, users_list.usernames[i]) == 0)
                {
                    printf("nie dobry\n");
                    valid = false;
                    v_msg.is_valid = valid;
                    v_msg.type = 300;
                    msgsnd(id, &v_msg, sizeof(v_msg.is_valid), 0);
                    break;
                }
            }
            
            // printf("to ten drugi");
            
            if (valid == false)
            {
                break;
            }
            
            
            for (int i = 0; i < MAX_users; i++)
            {
                
                printf("%d:\t%s\n", users_list.ids[i], users_list.usernames[i]);
                // char buf[1024];
                // strcpy(buf, users_list.usernames[i]);
                if(users_list.usernames[i][0] == '\0')
                {
                    strcpy(users_list.usernames[i], receive.text);
                    users_list.ids[i] = 50 + i;
                    user_count++;
                    printf("whatever\n");
                    break;
                }
            }
            
            v_msg.is_valid = true;
            v_msg.type = 300;
            msgsnd(id, &v_msg, sizeof(v_msg.is_valid), 0);
            
            // receive.type = 1;
            // msgrcv(id, &receive, string_size, receive.type, 0);
            // printf("%s\n", receive.text);
        
        
            
            // lseek(fd[0], 0, SEEK_SET);
            // while ((read(fd[0], &c, 1)) > 0)
            // {
            //     if (c != '\n')
            //     {
            //         strcpy(&buf[index], &c);
            //         index++;
            //     }           
            //     else
            //     {
            //         if (strcmp(buf, receive.text) == 0)
            //         {
            //             valid = false;
            //             v_msg.is_valid = valid;
            //             v_msg.type = 3;
            //             msgsnd(id, &v_msg, sizeof(v_msg.is_valid), 0);
            //             break;
            //         }
            //         index = 0;
            //         memset(buf, 0, strlen(buf));
            //     }
            // }
            // if (valid == true)
            // {
            //     write(fd[0], receive.text, strlen(receive.text));
            //     write(fd[0], endl, 1);
            //     v_msg.is_valid = true;
            //     v_msg.type = 3;
            //     msgsnd(id, &v_msg, sizeof(v_msg.is_valid), 0);
            //     receive.type = 1;
            //     msgrcv(id, &receive, string_size, receive.type, 0);
            //     printf("%s\n", receive.text);
            // }
            //
            // index = 0;
            // lseek(fd[1], 0, SEEK_SET);
            // while ((read(fd[1], &c, 1)) > 0)
            // {
            //     if (c != '\n')
            //     {
            //         strcpy(&buf[index], &c);
            //         index++;
            //     }
            //     else
            //     {
            //         if (strcmp(buf, receive.text) == 0)
            //         {
            //             // dopisanie użytkownika do pliku pokoju
            //             break;
            //         }
            //         index = 0;
            //         memset(buf, 0, strlen(buf));
            //     }
            //
            //     //stworzenie nowego pliku pokoju i dopisanie go do listy pokoi
            break;

        default:
            printf("default\n");
            break;
        }
    }

    return 0;
}
