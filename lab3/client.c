/*
 * Ryan Rishi
 * A. Amer
 * COEN 146 - Computer Networks
 * 5 February 2016
 *
 * Lab 3 - Simple chat client
 * client.c
 *
 * gethostbyname
 * init struct sockaddr
 * open socket
 * socklen_t socket_length = sizeof(struct)
 * 	- this way, recvfrom(,,,,, &len);
 * forever loop {
 * 	  read from stdin
 * 	  send data
 * 	  recv data from server
 * 	  print data
 * 	}
 *
 * 	error handling for socket (should be >0) creation and bind, as well as sendto and recvfrom
 * 	perror(char*) -- will print errors (in errno.h)
 *
 * 	both client and server should have 'exit' to get out of forever loop - check after send and recieve on both sides
 */

#include <stdio.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char * argv[]) {
	/* get hostname */
	struct hostent * server_addr = gethostbyname(argv[1]);

	/* init sockaddr_in struct */
	struct sockaddr_in server_conn;
	server_conn.sin_family = AF_INET;
	server_conn.sin_port = htons(5080);
	socklen_t socket_length = sizeof(server_conn);

	bcopy(server_addr->h_addr, &server_conn.sin_addr, server_addr->h_length);

	/* create socket */
	int sockfd = socket(PF_INET, SOCK_DGRAM, 0);
	// printf("sockfd:\t%d\n", sockfd);

	char buffer[256];

	for (;;) {
		/* read from stdin */
		printf("> ");
		fgets(buffer, 256, stdin);

		/* send data */
		sendto(sockfd, buffer, 256, 0, (struct sockaddr *) & server_conn, sizeof(server_conn));

		/* check for exit */
		if (strcmp(buffer, "exit\n") == 0) {
			printf("Goodbye!\n");
			break;
		}

		/* receive data */
		int num_bytes = recvfrom(sockfd, buffer, /* size of buffer */ 256, /* flags */ 0, (struct sockaddr *) &server_conn, &socket_length);
		//printf("%d", num_bytes);

		if (strcmp(buffer, "exit\n") == 0) {
			printf("Your friend has disconnected. Goodbye!\n");
			break;
		}

		/* print data */
		printf("%s", buffer);	//	fgets keeps newline

	}

	close(sockfd);

	return 0;

}
