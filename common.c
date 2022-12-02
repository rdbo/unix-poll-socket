#include "common.h"
#include <pwd.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
	
const char *
get_home_dir()
{
	const char *home_dir;

	if (!(home_dir = getenv("HOME")))
		home_dir = getpwuid(getuid())->pw_dir;

	return home_dir;
}

void
get_socket_path(char *buf, size_t bufsiz, const char *home_dir)
{
	snprintf(buf, bufsiz, "%s/%s", home_dir, ".pollsock");
}
