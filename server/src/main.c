#include "server.h"
#include <stdlib.h>

int players_count(int argc, char **argv) {
	// if there is no argument
	if (argc < 2) return -1;  

	int count = atoi(argv[1]);

	// if there is invalid argument
	if (count <= 1 || count > 4) return -1;

	return count;
}

int main(int argc, char **argv) {
	int n = players_count(argc, argv);
	if (n == -1) return 0;

	run_server(n);

	return 0;
}
