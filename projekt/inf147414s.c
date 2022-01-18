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

#define MAX_users 5
#define MAX_rooms 10

struct tim{
    int hour;
    int minute;
    int sec;
};

struct history{
    char messages[10][string_size];
    char usernames[10][string_size];
    struct tim date[10];
    
};

struct validation
{
    long type;
    bool is_valid;
    int user_id;
} v_msg;



struct msgbuf{
  long type;
  struct tim date;
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
    // int myarray_size = sizeof(struct room) * l;
    int myarray_size = (sizeof(myarray) / sizeof(myarray[0])) + 1;
    struct room* myrealloced_array = realloc(myarray, myarray_size * sizeof( struct room));
    if (myrealloced_array) {
        myarray = myrealloced_array;
    }else {
        printf("nie dziala\n");
    }
}

void add_history_struct(struct history* myarray, int l){
    // int myarray_size = (sizeof(myarray) / sizeof(myarray[0])) + 1;
    int myarray_size =  l + 1;
    struct history* myrealloced_array = realloc(myarray, myarray_size * sizeof( struct history) * sizeof(struct tim));
    if (myrealloced_array) {
        myarray = myrealloced_array;
    }else {
        printf("nie dziala\n");
    }
}

void add_msg_to_history(struct history *h, struct msgbuf m){
    int hst = 10;
    for(int i = 0; i < hst; i++){
        if(h->messages[i][0] == '\0'){
            
            strcpy(h->messages[i], m.text);
            strcpy(h->usernames[i], m.sender);
            h->date[i].hour = m.date.hour;
            h->date[i].minute = m.date.minute;
            h->date[i].sec = m.date.sec;
            return;
            
        }
        
    }
    
    for(int i = 0 ; i < hst - 1; i++){
        strcpy(h->messages[i], h->messages[i + 1]);
        strcpy(h->usernames[i], h->usernames[i + 1]);
        h->date[i].hour = h->date[i + 1].hour;
        h->date[i].minute = h->date[i + 1].minute;
        h->date[i].sec = h->date[i + 1].sec;

    }
    strcpy(h->messages[hst - 1], m.text);
    strcpy(h->usernames[hst - 1], m.sender);
    h->date[hst - 1].hour = m.date.hour;
    h->date[hst - 1].minute = m.date.minute;
    h->date[hst - 1].sec = m.date.sec;
}



int main(int argc, char *argv[])
{
    struct room* rooms_list = malloc(1* sizeof(struct room));
    int number_of_rooms = 1;
    struct history* history_structs = malloc(1* sizeof(struct history));
    int number_of_history_structs = 1;
    
    for(int i = 0; i <  10; i++){
        memset(history_structs[0].messages[i], '\0', string_size);
    }
    

    for (int i = 0; i < MAX_users; i++)
    {
        memset(users_list.usernames[i], 0, string_size);
    }
    int user_count = 0;
    int q_number = atoi(argv[1]);
    int id = msgget(q_number, 0644 | IPC_CREAT);
    if (fork() == 0)
    {
        while (1)
        {   
            msgrcv(id, &receive, msg_size, -100, 0);
            // printf("received msg:\t%s\n", receive.text);
            // printf("received sender:\t%s\n", receive.sender);
            // printf("received receiver:\t%s\n", receive.receiver);
            switch (receive.type)
            {
                case 1:
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
                    bool valid = true;
                    


                    for (int i = 0; i < MAX_users; i++)
                    {
                        if(strcmp(receive.text, users_list.usernames[i]) == 0)
                        {
                            valid = false;
                            v_msg.is_valid = valid;
                            v_msg.type = 300;
                            msgsnd(id, &v_msg, sizeof(v_msg.is_valid), 0);
                            break;
                        }
                    }


                    if (valid == false)
                    {
                        break;
                    }


                    for (int i = 0; i < MAX_users; i++)
                    {

                        if(users_list.usernames[i][0] == '\0')
                        {
                            strcpy(users_list.usernames[i], receive.text);
                            users_list.ids[i] = 50 + i;
                            v_msg.user_id = users_list.ids[i];
                            user_count++;
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
                            strcpy(rooms_list[i].room_name, receive.text);
                            strcpy(rooms_list[i].usernames[0], username);
                            number_of_rooms += 1;
                            add_room(rooms_list);
                            
                            
                            number_of_history_structs += 1;
                            // add_history_struct(history_structs, number_of_history_structs);
                            for(int i = 0; i <  10; i++){
                                memset(history_structs[number_of_history_structs - 1].messages[i], '\0', string_size);
                            }
                            break;
                        }
                    }
                    // for (int i = 0; i < number_of_rooms; i++)
                    // {
                    //     printf("Nazwa pokoju:\t%s\n", rooms_list[i].room_name);
                    //     fflush(stdout);
                    //     int j = 0;
                    //     while (rooms_list[i].usernames[j][0] != '\0')
                    //     {
                    //         printf("\tNazwa uzytkownika:\t%s\n", rooms_list[i].usernames[j]);
                    //         fflush(stdout);
                    //         j++;
                    //     }


                    // }
                    // printf("____________\n");
                    
                    break;
                    
                case 2:
                // wyjście z klienta
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
                            strcpy(rooms_list[i].room_name, receive.text);
                            strcpy(rooms_list[i].usernames[0], receive.sender);
                            number_of_rooms += 1;
                            add_room(rooms_list);
                            
                            
                            number_of_history_structs += 1;
                            // add_history_struct(history_structs, number_of_history_structs);
                            for(int i = 0; i <  10; i++){
                                memset(history_structs[number_of_history_structs - 1].messages[i], '\0', string_size);
                            }
                            break;
                        }
                    }
                    // for (int i = 0; i < number_of_rooms; i++)
                    // {
                    //     printf("Nazwa pokoju:\t%s\n", rooms_list[i].room_name);
                    //     fflush(stdout);
                    //     int j = 0;
                    //     while (rooms_list[i].usernames[j][0] != '\0')
                    //     {
                    //         printf("\tNazwa uzytkownika:\t%s\n", rooms_list[i].usernames[j]);
                    //         fflush(stdout);
                    //         j++;
                    //     }


                    // }
                    break;
                    
                case 4:
                // wyjście z pokoju
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
                // wysyłanie prywatnych wiadomości
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
                                            send.date.hour = receive.date.hour;
                                            send.date.minute = receive.date.minute;
                                            send.date.sec = receive.date.sec;
                                            for (int l = 0; l < MAX_users; l++)
                                            {
                                                if(strcmp(users_list.usernames[l], receive.receiver) == 0)
                                                {
                                                    send.type = users_list.ids[l];
                                                    // printf("%d\n", msgsnd(id, &send, msg_size, 0));
                                                    msgsnd(id, &send, msg_size, 0);
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
                    // wysłanie wiadomości na pokój
                    strcpy(send.sender, receive.sender);
                    strcpy(send.receiver, receive.receiver);
                    send.date.hour = receive.date.hour;
                    send.date.minute = receive.date.minute;
                    send.date.sec = receive.date.sec;
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
                                    add_msg_to_history(&history_structs[i], receive);
                                }
                            }
                        }
                    }
                    break;
                    
                case 7:
                // wypisanie pokoi
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
                    send.date.hour = receive.date.hour;
                    send.date.minute = receive.date.minute;
                    send.date.sec = receive.date.sec;
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
                            send.date.hour = receive.date.hour;
                            send.date.minute = receive.date.minute;
                            send.date.sec = receive.date.sec;
                            // printf("%d\n", msgsnd(id, &send, msg_size, 0));
                            msgsnd(id, &send, msg_size, 0);
                            break;
                        }
                    }
                    break;

                case 9: ;
                // wypisanie użytkowników serwera
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
                            send.date.hour = receive.date.hour;
                            send.date.minute = receive.date.minute;
                            send.date.sec = receive.date.sec;
                            // send.date = receive.date;
                            // printf("%d\n", msgsnd(id, &send, msg_size, 0));
                            msgsnd(id, &send, msg_size, 0);
                            break;
                        }
                    }
                    break;
                    break;

                // case 10:
                //     wypisnie użytkownikowi historię wiadomości z pokoju
                //     for(int j = 0; j < number_of_rooms; j++){
                //     for(int i = 0; i < 10; i++){
                //         printf("wiadom: %s\n", history_structs[0].messages[i]);
                //     }
                //     }
                //     break;

                // default:
                //     printf("default\n");
                //     break;
            }
        }
    }
    else
    {
        char str[string_size];
        while (1)
        {
            printf("type in 'exit' to stop the server\n");
            scanf("%s", str);
            if(strcmp("exit", str) == 0)
            {
                msgctl(id, IPC_RMID, NULL);
                kill(0, SIGINT);
            }
        }
        
    }
    
    return 0;
}
