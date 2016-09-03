#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <endian.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "fastcgi.h"
#include "fcgiapp.h"
#include "log.h"

#define SOCK_PATH	"/var/run/fastcgi.sock"

#define FCGI_LISTENSOCK_FILENO	0

int main()
{
	/* delete the socket file */
	unlink(SOCK_PATH);

	/* create a socket */
	int fcgi_fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (fcgi_fd < 0) {
		perror("socket()");
		return -1;
	}

	struct sockaddr_un server_addr;
	server_addr.sun_family = AF_UNIX;
	strcpy(server_addr.sun_path, SOCK_PATH);

	/* bind with the local file */
	int ret = bind(fcgi_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
	if (ret < 0) {
		perror("bind()");
		return -1;
	}

	/* listen */
	ret = listen(fcgi_fd, 5);
	if (ret < 0) {
		perror("listen()");
		return -1;
	}

	unsigned char buff[32768];
	struct sockaddr_un client_addr;
	socklen_t len = sizeof(client_addr);

	char html[128];
	int count = 0;

	if (fcgi_fd != FCGI_LISTENSOCK_FILENO) {
		close(FCGI_LISTENSOCK_FILENO);
		dup2(fcgi_fd, FCGI_LISTENSOCK_FILENO);
		close(fcgi_fd);
	}

	for (;;) {
		/* accept a connection */
		int client_sockfd = accept(fcgi_fd, (struct sockaddr *)&client_addr, &len);
		if (client_sockfd < 0) {
			perror("accept()");
			return -1;
		}

		/* exchange data */
		int n = read(client_sockfd, buff, sizeof(buff));
		if (n < 0) {
			perror("read");
			return -1;
		}

		FILE *fp = fopen("recv", "w");
		if (!fp) {
			perror("fopen");
			return -1;
		}
		fwrite(buff, n, 1, fp);

 		snprintf (html, sizeof(html), "Content-Type: text/html\r\n\r\n<html><h1>Hello FastCGI!!!</h1>Request %d times</html>", ++count);
		int len = strlen(html);
		int padding_len = 8 - len % 8;

		fcgi_header header;

		/* FCGI_STDOUT */
		header = make_header(FCGI_STDOUT, 1, len, padding_len);
		n = write(client_sockfd, (unsigned char *)&header, sizeof(header));
		if (n < 0) {
			perror("write()");
			return -1;
		}
		fwrite((unsigned char *)&header, sizeof(header), 1, fp);

		n = write(client_sockfd, (unsigned char *)html, len);
		if (n < 0) {
			perror("write()");
			return -1;
		}
		fwrite((unsigned char *)html, len, 1, fp);

		unsigned char padding_buf[padding_len];
		memset(padding_buf, 0, padding_len);
		n = write(client_sockfd, padding_buf, padding_len);
		if (n < 0) {
			perror("write()");
			return -1;
		}
		fwrite(padding_buf, padding_len, 1, fp);

		/* FCGI_STDERR */
		header = make_header(FCGI_STDERR, 1, 0, 0);
		n = write(client_sockfd, (unsigned char *)&header, sizeof(header));
		if (n < 0) {
			perror("write()");
			return -1;
		}
		fwrite((unsigned char *)&header, sizeof(header), 1, fp);

		/* FCGI_END_REQUEST */
		header = make_header(FCGI_END_REQUEST, 1, 8, 0);
		n = write(client_sockfd, (unsigned char *)&header, sizeof(header));
		if (n < 0) {
			perror("write()");
			return -1;
		}
		fwrite((unsigned char *)&header, sizeof(header), 1, fp);

		fcgi_end_request_body body = make_end_request_body(0, FCGI_REQUEST_COMPLETE);
		fwrite((unsigned char *)&body, sizeof(body), 1, fp);

		fflush(fp);
#if 0
		for (i = 0; i < n; i++) {
			if (isascii(buff[i]))
				printf("%c", buff[i]);
			else
				printf("0x%02x ", buff[i]);
		}
		printf("\n");
#endif

		/* close the socket */
		close(client_sockfd);
	}

	return 0;
}
