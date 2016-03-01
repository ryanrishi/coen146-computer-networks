/**
 * Ryan Rishi
 * A. Amer
 * COEN 146L - Computer Networks
 * 1/29/16
 *
 * Lab 2
 * server.c
 *
 * init sockaddr_in struct
 * create socket
 * bind socket
 * recvfrom data (from client)
 * write text data
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

int main() {
	/* init sockaddr_in socket */
	struct sockaddr_in server;
	bzero((char*) &server, sizeof(server));	//	zero out server (is this necessary?)
	server.sin_family = AF_INET;			//	IPV4
	server.sin_addr.s_addr = INADDR_ANY;	//	default IPV4 address for this machine
	server.sin_port = htons(5080);			//	host order --> network order short

	/* create socket */
	int sockfd = socket(PF_INET, /* UDP */ SOCK_DGRAM, /* flag options */ 0);
	//printf("%d", sockfd);	//	this should be >= 0 if successful

	/* bind socket */
	bind(sockfd, (struct sockaddr *) &server, sizeof(server));	//	map socket to this program

	/* recv data from client (recvfrom) */
	char buffer[256];
	int num_bytes = recvfrom(sockfd, buffer, /* size of buffer */ 256, /* flags */0, (struct sockaddr *) &server, NULL);
	if(num_bytes < 0)
	{
	/**
	 * num_bytes < 0 when I compile via gcc v4.4.7 on the linux workstations, but
	 * not when I compile on my laptop with gcc (OS X 10.11.2, Apple LLVM 6.1.0)
	 */
		printf("Number of bytes received:\t%d\n", num_bytes);	//	should be >= 0
		perror("recvfrom error");
	}

	/* write to file */
	FILE * fp = fopen("out.txt", "w");
	fputs(buffer, fp);
	fclose(fp);
}
