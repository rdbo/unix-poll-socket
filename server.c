#include "common.h"
#include <sys/un.h>
#include <sys/socket.h>
#include <unistd.h>
#include <signal.h>
#include <poll.h>
#include <stdio.h>

int sigcaught = 0;

void
sighandler(int sig)
{
	sigcaught = 1;
}

int
main()
{
	int sockfd;
	struct message msg;
	struct sockaddr_un sockaddr;
	const char *home_dir;
	struct pollfd pollfd;
	int ready;

	home_dir = get_home_dir();
	if (!home_dir) {
		perror("unable to get home directory");
		return -1;
	}

	/* create unix socket */
	sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
	if (sockfd == -1) {
		perror("unable to open unix socket");
		return -1;
	}

	/* bind sockaddr to socket */
	sockaddr.sun_family = AF_UNIX;
	get_socket_path(sockaddr.sun_path, sizeof(sockaddr.sun_path), home_dir);
	printf("socket path: %s\n", sockaddr.sun_path);
	bind(sockfd, (struct sockaddr *)&sockaddr, sizeof(sockaddr));

	/* enable signal handler to stop process during loop */
	signal(SIGINT, sighandler);

	/* enable polling for read events in socket file descriptor */
	pollfd.fd = sockfd;
	pollfd.events = POLLIN;
	while (!sigcaught) {
		ready = poll(&pollfd, 1, -1);
		if (ready == -1) {
			perror("poll not ready");
			continue;
		}

		if (pollfd.revents & POLLIN == 0)
			continue;

		printf("poll event caught!\n\n");

		recv(sockfd, (void *)&msg, sizeof(msg), 0);
		printf("===== MESSAGE =====\n");
		printf("name: %s\n", msg.name);
		printf("text: %s\n", msg.text);
		printf("===================\n\n");
	}

	printf("signal caught\n");
	close(sockfd);
	remove(sockaddr.sun_path);

	return 0;
}
