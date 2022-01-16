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


#define string_size 1024
#define msg_size 3072
#define bool_size sizeof(bool)

#define MAX_users 5
#define MAX_rooms 10

// struct history{
//     char messages[10][string_size];
//     int i = 0;
// }

struct validation
{
    long type;
    bool is_valid;
    int user_id;
} v_msg;

// struct msgbuf
// {
//     long type;
//     char text[string_size];
// } receive;

struct msgbuf{
  long type;
  // data
  char sender[string_size];
  char receiver[string_size];
  char text[string_size];
}send, receive;

struct users
{
    char usernames[MAX_users][string_size];
    int ids[MAX_users];

} users_list;

struct room
{
    char room_name[string_size];
    char usernames[5][string_size];

};

void add_room(struct room* myarray){
    int myarray_size = (sizeof(myarray) / sizeof(myarray[0])) + 1;
    struct room* myrealloced_array = realloc(myarray, myarray_size * sizeof( struct room));
    if (myrealloced_array) {
        myarray = myrealloced_array;
    }else {
        printf("nie dziala\n");
        //free(myarray);
    // deal with realloc failing because memory could not be allocated.
    }
}



int main(int argc, char *argv[])
{
    struct room* rooms_list = malloc(1* sizeof(struct room));
    int number_of_rooms = 1;

    // int a = sizeof(rooms_list)/ sizeof(struct room);
    // rooms_list[0].room_name[0] = 'j';
    // rooms_list[0].room_name[1] = 'p';
    // printf("%s\n", rooms_list[0].room_name);
    // add_room(rooms_list);
    // int b = sizeof(rooms_list)/ sizeof(struct room);
    // rooms_list[1].room_name[0] = 'h';
    // rooms_list[1].room_name[1] = 'w';
    // rooms_list[1].room_name[2] = 'd';
    // rooms_list[1].room_name[3] = 'p';
    // printf("%s\n", rooms_list[1].room_name);
    // printf("%s\n", rooms_list[0].room_name);

    for (int i = 0; i < MAX_users; i++)
    {
        memset(users_list.usernames[i], 0, string_size);
    }
    int user_count = 0;
    // printf("%d", user_count);
    // printf("%s", argv[1]);

    // int string_size = string_size;
    // int bool_size = sizeof(bool);
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
    // char commands = {"exit", "enter", "logout", "private", "room", "rooms", "in_room", "on_server", "history", "help"};

    while (1)
    {

        // receive.type = 1;
        msgrcv(id, &receive, msg_size, -100, 0);
        printf("received username:\t%s\n", receive.text);
        printf("received username:\t%s\n", receive.sender);
        printf("received username:\t%s\n", receive.receiver);
        // fflush(stdout);
        // printf("received type:\t%ld\n", receive.type);
        // fflush(stdout);
        // printf("od nowa:\n");
        // fflush(stdout);
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
            // char c, buf[string_size];
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
                    // printf("nie dobry\n");
                    // fflush(stdout);
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

                // printf("%d:\t%s\n", users_list.ids[i], users_list.usernames[i]);
                // fflush(stdout);
                // char buf[string_size];
                // strcpy(buf, users_list.usernames[i]);
                if(users_list.usernames[i][0] == '\0')
                {
                    strcpy(users_list.usernames[i], receive.text);
                    users_list.ids[i] = 50 + i;
                    v_msg.user_id = users_list.ids[i];
                    user_count++;
                    // printf("liczab uzytkownikow na serwerze:\t%d\n", user_count);
                    // fflush(stdout);
                    break;
                }
            }

            v_msg.is_valid = true;
            v_msg.type = 300;
            msgsnd(id, &v_msg, sizeof(v_msg.is_valid) + sizeof(int), 0);

            char username[string_size];
            strcpy(username, receive.text);

            receive.type = 1;
            msgrcv(id, &receive, msg_size, receive.type, 0);
            bool already_added = false;

            for (int i = 0; i < number_of_rooms; i++)
            {
                if(strcmp(rooms_list[i].room_name, receive.text) == 0)
                {
                    for (int j = 0; j < MAX_users; j++)
                    {
                        if(rooms_list[i].usernames[j][0] == '\0')
                        {
                            strcpy(rooms_list[i].usernames[j], username);
                            already_added = true;
                            break;
                        }
                    }
                }
                if(rooms_list[i].room_name[0] == '\0' && already_added != true)
                {
                    // index = 1;
                    strcpy(rooms_list[i].room_name, receive.text);
                    strcpy(rooms_list[i].usernames[0], username);
                    add_room(rooms_list);
                    number_of_rooms += 1;
                    break;
                }
            }
            for (int i = 0; i < number_of_rooms; i++)
            {
                printf("Nazwa pokoju:\t%s\n", rooms_list[i].room_name);
                fflush(stdout);
                int j = 0;
                while (rooms_list[i].usernames[j][0] != '\0')
                {
                    printf("\tNazwa uzytkownika:\t%s\n", rooms_list[i].usernames[j]);
                    fflush(stdout);
                    j++;
                }


            }
            printf("____________\n");
            // if (index != MAX_rooms)
            // {
            //     strcpy(rooms_list[index].room_name, receive.text);
            //     strcpy(rooms_list[index].usernames[0], username);
            // }


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
            
        case 2:
            for (int i = 0; i < MAX_users; i++)
            {
                if(strcmp(users_list.usernames[i], receive.sender) == 0)
                {
                    memset(users_list.usernames[i], '\0', string_size);
                    user_count--;
                    break;
                }
            }
            for(int i = 0; i < number_of_rooms; i++){
                for(int j = 0; j < MAX_users; j++){
                    if(strcmp(rooms_list[i].usernames[j], receive.sender) == 0){
                        memset(rooms_list[i].usernames[j], '\0', string_size);
                    }
                }
            }
            break;
        case 3: ;
            // dodanie użytkownika do pokoju
            bool already_added_2 = false;

            for (int i = 0; i < number_of_rooms; i++)
            {
                if(strcmp(rooms_list[i].room_name, receive.receiver) == 0)
                {
                    for (int j = 0; j < MAX_users; j++)
                    {
                        if(rooms_list[i].usernames[j][0] == '\0')
                        {
                            strcpy(rooms_list[i].usernames[j], receive.sender);
                            already_added_2 = true;
                            break;
                        }
                    }
                }
                if(rooms_list[i].room_name[0] == '\0' && already_added_2 != true)
                {
                    // index = 1;
                    strcpy(rooms_list[i].room_name, receive.text);
                    strcpy(rooms_list[i].usernames[0], receive.sender);
                    add_room(rooms_list);
                    number_of_rooms += 1;
                    break;
                }
            }
            for (int i = 0; i < number_of_rooms; i++)
            {
                printf("Nazwa pokoju:\t%s\n", rooms_list[i].room_name);
                fflush(stdout);
                int j = 0;
                while (rooms_list[i].usernames[j][0] != '\0')
                {
                    printf("\tNazwa uzytkownika:\t%s\n", rooms_list[i].usernames[j]);
                    fflush(stdout);
                    j++;
                }


            }
            break;
            
        case 4:
            for(int i = 0; i <  number_of_rooms; i++)
            {
                if(strcmp(rooms_list[i].room_name, receive.text) == 0)
                {
                    for(int j = 0; j < MAX_users; j++)
                    {
                        if(strcmp(rooms_list[i].usernames[j], receive.sender) == 0)
                        {
                            memset(rooms_list[i].usernames[j], '\0', string_size);
                            break;
                        }
                    }
                }
            }
            break;
            
        case 5: ;
            bool already_sent = false;
            for (int i = 0; i < number_of_rooms; i++)
            {
                if (already_sent == false)
                {
                    for (int j = 0; j < MAX_users; j++)
                    {
                        if(strcmp(rooms_list[i].usernames[j], receive.sender) == 0)
                        {
                            for (int k = 0; k < MAX_users; k++)
                            {
                                if(strcmp(rooms_list[i].usernames[k], receive.receiver) == 0)
                                {
                                    strcpy(send.text, receive.text);
                                    strcpy(send.sender, receive.sender);
                                    strcpy(send.receiver, receive.receiver);
                                    for (int l = 0; l < MAX_users; l++)
                                    {
                                        if(strcmp(users_list.usernames[l], receive.receiver) == 0)
                                        {
                                            send.type = users_list.ids[l];
                                            printf("%d\n", msgsnd(id, &send, msg_size, 0));
                                            break;
                                        }
                                    }
                                    already_sent = true;
                                    break;
                                }
                            }
                            break;
                        }
                    }
                }
                else
                {
                   break; 
                }
                
            }
            break;

        case 6:
            // wytsłanie wiadomości na pokój
            strcpy(send.sender, receive.sender);
            strcpy(send.receiver, receive.receiver);
            for(int i = 0; i < number_of_rooms; i++){
                if(strcmp(receive.receiver, rooms_list[i].room_name) == 0){
                    for(int l = 0; l < MAX_users; l++){
                        if(strcmp(receive.sender, rooms_list[i].usernames[l]) == 0){
                            for(int j = 0; j < MAX_users; j++){
                                for(int k = 0; k < MAX_users; k++){
                                    if(strcmp(rooms_list[i].usernames[j], users_list.usernames[k]) == 0){
                                        send.type = users_list.ids[k];
                                        strcpy(send.text, receive.text);
                                        msgsnd(id, &send, msg_size, 0);
                                    }
                                }
                            }
                        }
                    }
                }
            }
            break;
            
        case 7:
            strcpy(send.receiver, receive.sender);
            strcpy(send.sender, "server");
            memset(send.text, '\0', string_size);
            for (int i = 0; i < user_count; i++)
            {
                if (strcmp(receive.sender, users_list.usernames[i]) == 0)
                {
                    send.type = users_list.ids[i];
                    break;
                }
            }
            int index = 0;
            for (int i = 0; i < number_of_rooms; i++)
            {
                for (int j = 0; j < strlen(rooms_list[i].room_name); j++)
                {
                    // printf("%s\n", rooms_list[i].room_name);
                    send.text[index] = rooms_list[i].room_name[j];
                    index ++;
                }
                send.text[index] = ' ';
                index ++;
            }
            // printf("%d\n", msgsnd(id, &send, msg_size, 0));
            msgsnd(id, &send, msg_size, 0);
            break;

        case 8: ;
            // wypisnie użytkownikowi użytkowników w pokoju
            char msg_2[string_size];
            memset(msg_2, '\0', string_size);
            for(int i = 0; i < number_of_rooms; i++){
                if(strcmp(rooms_list[i].room_name, receive.text) == 0){
                    for(int j = 0; j < MAX_users; j++){
                        strcat(msg_2, rooms_list[i].usernames[j]);
                        strcat(msg_2, " "); 
                    }
                }
            }


            strcpy(send.text, msg_2);
            for (int l = 0; l < MAX_users; l++)
            {
                if(strcmp(users_list.usernames[l], receive.sender) == 0)
                {
                    send.type = users_list.ids[l];
                    strcpy(send.sender, "server");
                    printf("%d\n", msgsnd(id, &send, msg_size, 0));
                    break;
                }
            }
            break;

        case 9: ;
            char msg[string_size];
            memset(msg, '\0', string_size);
            for(int i = 0; i < MAX_users; i++){
                if(users_list.usernames[i][0] != '\0'){
                    strcat(msg, users_list.usernames[i]);
                    strcat(msg, " "); 
                }
            }
            strcpy(send.text, msg);
            for (int l = 0; l < MAX_users; l++)
            {
                if(strcmp(users_list.usernames[l], receive.sender) == 0)
                {
                    send.type = users_list.ids[l];
                    strcpy(send.sender, "server");
                    printf("%d\n", msgsnd(id, &send, msg_size, 0));
                    break;
                }
            }
            break;
            break;

        case 10:
            //wypisnie użytkownikowi historię wiadomości z pokoju
            break;

        // default:
        //     printf("default\n");
        //     break;
        }
    }


    return 0;
}
