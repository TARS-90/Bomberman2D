#include "client.h"
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <time.h>

void run_client() {
	int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(12345);
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	connect(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));

	while (1) {
		char msg[] = "w\0";
		send(sock_fd, msg, 2, 0);
		sleep(1);
	}
}
