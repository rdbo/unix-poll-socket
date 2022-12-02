#include "common.h"
#include <sys/un.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>

int
main()
{
	int sockfd;
	const char *home_dir;
	struct sockaddr_un sockaddr;
	struct message msg = {
		"John",
		"Hello, World!"
	};

	home_dir = get_home_dir();
	if (!home_dir) {
		perror("unable to get home directory");
		return -1;
	}

	sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
	if (sockfd == -1) {
		perror("unable to open unix socket");
		return -1;
	}

	sockaddr.sun_family = AF_UNIX;
	get_socket_path(sockaddr.sun_path, sizeof(sockaddr.sun_path), home_dir);
	printf("socket path: %s\n", sockaddr.sun_path);
	connect(sockfd, (struct sockaddr *)&sockaddr, sizeof(sockaddr));
	send(sockfd, (void *)&msg, sizeof(msg), 0);
	return 0;
}
