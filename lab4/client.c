/*
 * Ryan Rishi
 * A. Amer
 * COEN 146L - Computer Networks Lab
 * 19 February 2016
 *
 * client.c
 *
 * Send packets containing message, checksum, and sequence number to server.
 *
 * Upon successful transmission, server will send back packet with message in
 * uppercase and different checksum. Print out packet contents.
 */

#include <stdio.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>

#define NUM_PACKETS_TO_SEND 5

struct packet {
	unsigned short seq;
	unsigned short checksum;
	char msg[64];
};

int checksum(char s[]) {
	int sum = 0, i = 0;
	while (s[i]) {
		sum += (int) s[i];
		i++;
	}
	return sum;
}

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
	// printf("sockfd\t%d\n", sockfd);

	int packet_to_send = 0;
	char * messages_to_send[5] = {\
		"The early bird gets the worm.",\
		"Two is blue because it lost its shoe.",\
		"Three is free so let it be!",\
		"Four score and seven years ago",\
		"Five went for a dive."\
	};

	for (;;) {

		/* create packet */
		struct packet p;
		strcpy(p.msg, messages_to_send[packet_to_send]);	//	used strcpy because assigning p.msg = messages_to_send[0] gave me an error about incompatible types
		p.checksum = checksum(p.msg);
		p.seq = packet_to_send;

		/* send data */
		sendto(sockfd, &p, sizeof(p), 0, (struct sockaddr *) & server_conn, sizeof(server_conn));
		printf("SENT {seq: %d, checksum: %d, msg: \"%s\"}\n", p.seq, p.checksum, p.msg);

		/* receive data */
		int num_bytes = recvfrom(sockfd, &p, /* size of packet */ 68, /* flags */ 0, (struct sockaddr *) &server_conn, &socket_length);
		printf("RECV {seq: %d, checksum: %d, msg: \"%s\"}\n", p.seq, p.checksum, p.msg);

		packet_to_send = p.seq;

		if (packet_to_send == NUM_PACKETS_TO_SEND) {
			break;
		}
	}
	return 0;
}
