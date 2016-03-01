/**
 * Ryan Rishi
 * A. Amer
 * COEN 146L - Computer Networks
 * 1/29/16
 *
 * Lab 2
 * client.c
 *
 * gethostbyname
 * init sockaddr_in struct
 * create socket
 * read file
 * sendto data --> server
 *
 * ./client linux604xx file.txt
 *
 */

#include <stdio.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <strings.h>

int main(int argc, char * argv[]) {
	/* get hostname */
	struct hostent * server_addr = gethostbyname(argv[1]);

	/* init sockaddr_in struct */
	struct sockaddr_in server_conn;
	server_conn.sin_family = AF_INET;
	server_conn.sin_port = htons(5080);		//	htons: host order --> network order short (endian)
	bcopy(server_addr->h_addr, &server_conn.sin_addr, server_addr->h_length);

	/* create socket */
	int sockfd = socket(PF_INET, SOCK_DGRAM, 0);
	// printf("%d\n", sockfd);				//	should be >= 0 if successful

	/* read file */
	char * buffer[256];
	FILE *fp = fopen(argv[2], "r");
	fread(buffer, 1, 256, fp);
	// fgets(*buffer, /* how much to read in */ 256, fp);	// this gives a segfault
	fclose(fp);

	int num_bytes = sendto(sockfd, buffer, 256, 0, (struct sockaddr *) &server_conn, sizeof(server_conn));
	printf("Number of bytes sent:\t%d\n", num_bytes);
}
