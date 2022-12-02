#ifndef COMMON_H
#define COMMON_H
#include <stddef.h>

struct message {
	char name[10];
	char text[22];
};

extern const char *get_home_dir();
extern void get_socket_path(char *buf, size_t bufsiz, const char *home_dir);

#define MSGSIZ (sizeof(struct message))

#endif
