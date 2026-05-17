#include "client.h"
#include "enums.h"
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <time.h>

void run_client() {
	int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in server_addr = {
		.sin_family = AF_INET,
		.sin_port = htons(12345),
		.sin_addr.s_addr = inet_addr("127.0.0.1")
	};

	connect(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));

	while (1) {
		Message msg;
		printf("\nPodaj zadanie: \0");
		char c;
		scanf(" %c", &c);
		switch (c) {
			case 'w': {
				msg = MSG_MOVE_UP;
				break;
			}
			case 's': {
				msg = MSG_MOVE_DOWN;
				break;
			}
			case 'd': {
				msg = MSG_MOVE_RIGHT;
				break;
			}
			case 'a': {
				msg = MSG_MOVE_LEFT;
				break;
			}
			case 'p': {
				msg = MSG_PLACE_BOMB;
				break;
			}
		}
		send(sock_fd, &msg, sizeof(msg), 0);
	}
}
