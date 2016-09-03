#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <fcgi_stdio.h>

#include "log.h"

#define SOCK_PATH	"/var/run/fastcgi.sock"

#define FCGI_LISTENSOCK_FILENO	0

static int create_unix_server(const char *sock_path)
{
	/* delete the socket file */
	unlink(sock_path);

	int sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (sockfd < 0) {
		perror("socket()");
		return -1;
	}

	struct sockaddr_un server_addr;
	server_addr.sun_family = AF_UNIX;
	strcpy(server_addr.sun_path, sock_path);

	int ret = bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
	if (ret < 0) {
		perror("bind()");
		goto err;
	}

	ret = listen(sockfd, 5);
	if (ret < 0) {
		perror("listen()");
		goto err;
	}

	return sockfd;

err:
	close(sockfd);
	return -1;
}

static void main_process(int count)
{
	printf("Content-type: text/html\r\n\r\n"
			"FastCGI Hello!\n"
			"Request number %d running on %s "
			"Process ID: %d\n",
			count,
			getenv("SERVER_NAME"),
			getpid());
}

int main(int argc, char **argv)
{
	int fcgi_fd = create_unix_server(SOCK_PATH);
	if (fcgi_fd < 0)
		return -1;

	if (fcgi_fd != FCGI_LISTENSOCK_FILENO) {
		close(FCGI_LISTENSOCK_FILENO);
		dup2(fcgi_fd, FCGI_LISTENSOCK_FILENO);
		close(fcgi_fd);
	}

	int count = 0;

	pid_t pid = fork();
	if (pid < 0) {
		return -1;
	} else if (pid == 0) {
		while (FCGI_Accept() >= 0) {
			main_process(++count);
		}
	} else {
		exit(0);
	}

	return 0;
}
