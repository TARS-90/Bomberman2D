#include "client.h"
#include <unistd.h>
#include <arpa/inet.h>

void run_client() {
	int sock_fd = socket(AF_NET, SOCK_STREAM, 0);
	struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_NET;
	server_addr.sin_port = htons(12345);
	server_addr.sin_addr.s_addr = intet_addr("127.0.0.1");

	connect(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));

	char *msg = "Player connected!\n";
	send(sock_fd, msg, 18, 0);
}
