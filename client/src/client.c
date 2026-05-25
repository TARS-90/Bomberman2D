#include "client.h"
#include "enums.h"
#include "sdl_engine.h"
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>
#include <SDL3/SDL.h>

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

void *receive_game_state(void* args) {
	int *sock_fd = (int*) args;
	GameState game_state;

	while (1) {
		if (recv(*sock_fd, &game_state, sizeof(GameState), 0) <= 0) {
		    printf("Połączenie z serwerem przerwane.\n");
		    break;
		}		
		print_game(&game_state);
	}
}


int connect_to_server(int *sock_fd, struct sockaddr_in *server_addr) {
	if ((*sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("Creating socket failed!\n");
		return -1;
	}

	*server_addr = {
		.sin_family = AF_INET,
		.sin_port = htons(12345),
		.sin_addr.s_addr = inet_addr("127.0.0.1")
	};

	if (connect(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
		perror("Connecting with server failed!\n");
		return -1;
	}

	return 0;
}

void run_client() {
	int sock_fd;
	struct sockaddr_in server_addr;
	if (connect_to_server(&sock_fd, &server_addr) == -1) return;
	
	pthread_t pid;
	if (pthread_create(&pid, receive_game_state, (void*) sock_fd) != 0) {
		perror("Creating thread for receiving game state failed!\n");
		close(sock_fd);
		return;
	}
    

	// main loop 
	while (1) {
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
