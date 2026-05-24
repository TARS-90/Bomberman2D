#include "client.h"
#include "enums.h"
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#define WIDTH 17 
#define HEIGHT 17
#define MAX_PLAYERS 4

void print_game(GameState *gs) {
    system("clear");
    for (int row = 0; row < HEIGHT; row++) {
        for (int col = 0; col < WIDTH; col++) {
            int index = (col * HEIGHT) + row;
            int player_drawn = 0;

            for (int i = 0; i < MAX_PLAYERS; i++) {
                if (gs->players[i].x == col && gs->players[i].y == row && gs->players[i].x != -1) {
                    printf("%d", gs->players[i].color);
                    player_drawn = 1;
                    break; 
                }
            }

            if (!player_drawn) {
                if (gs->board[index] == OBJECT_EMPTY)    printf("_");
                else if (gs->board[index] == OBJECT_WALL) printf("X");
            }
        }
        printf("\n");
    }
}

void run_client() {
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(12345),
        .sin_addr.s_addr = inet_addr("127.0.0.1")
    };

    if (connect(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Nie udało się połączyć z serwerem");
        return;
    }

    GameState game_state;
    
    while (1) {
        if (recv(sock_fd, &game_state, sizeof(GameState), 0) <= 0) {
            printf("Połączenie z serwerem przerwane.\n");
            break;
        }
        
        print_game(&game_state);
        
        printf("\nPodaj zadanie (w/a/s/d/p): ");
        fflush(stdout); 
        
        char c;
        if (scanf(" %c", &c) != 1) continue; 
        
        MessageType msg = MSG_NONE;
        switch (c) {
            case 'w': msg = MSG_MOVE_UP;    break;
            case 's': msg = MSG_MOVE_DOWN;  break;
            case 'd': msg = MSG_MOVE_RIGHT; break;
            case 'a': msg = MSG_MOVE_LEFT;  break;
            case 'p': msg = MSG_PLACE_BOMB; break;
            default:  msg = MSG_NONE;       break;
        }
        
        if (msg != MSG_NONE) {
            send(sock_fd, &msg, sizeof(msg), 0);
        }
        
        usleep(FRAME_DURATION); // 50ms
    }

    close(sock_fd);
}
