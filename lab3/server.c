/*
 * Ryan Rishi
 * A. Amer
 * COEN 146 - Computer Networks
 * 5 February 2016
 *
 * Lab 3 - Simple chat client
 * server.c
 *
 * init struct
 * open socket
 * call bind
 * forever loop {
 * 	  recv data
 * 	  print data
 * 	  read from stdin
 * 	  send data
 * }
 */

#include <stdio.h>
#include <netinet/in.h>
#include <strings.h>
#include <assert.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#include <unistd.h>

int main(int argc, char * argv[]) {
	struct sockaddr_in server;
	bzero((char *) &server, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(5080);
	socklen_t socket_length = sizeof(server);
	// printf("%d", socket_length);

	int sockfd = socket(PF_INET, SOCK_DGRAM, 0);
	// printf("sockfd:\t%d\n", sockfd);

	bind(sockfd, (struct sockaddr *) &server, sizeof(server));

	char buffer[256];

	for (;;) {
		/* recieve data */
		int num_bytes = recvfrom(sockfd, buffer, /* size of buffer */ 256, /* flags */ 0, (struct sockaddr *) &server, &socket_length);
		//printf("%d", num_bytes);

		if (strcmp(buffer, "exit\n") == 0) {
			printf("Your friend has disconnected. Goodbye!\n");
			break;
		}

		/* print data */
		printf("%s", buffer);

		/* read from stdin */
		printf("> ");
		fgets(buffer, 256, stdin);

		/* send data */
		sendto(sockfd, buffer, 256, 0, (struct sockaddr *) &server, sizeof(server));

		if (strcmp(buffer, "exit\n") == 0) {
			printf("Goodbye!\n");
			break;
		}

	}

	close(sockfd);

	return 0;
}
