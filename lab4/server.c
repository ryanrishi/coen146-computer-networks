/*
 * Ryan Rishi
 * A. Amer
 * COEN 146L - Computer Networks Lab
 * 19 February 2016
 *
 * server.c
 *
 * Listen for incoming packet. If packet is in sequence and has correct checksum, convert
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
#include <stdbool.h>
#include <ctype.h>
#include <unistd.h>

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
	// printf("checksum\t%d\n", checksum("HELLO"));
	struct sockaddr_in server;
	bzero((char *) &server, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(5080);
	socklen_t socket_length = sizeof(server);
	// printf("%d", socket_length);

	int sockfd = socket(PF_INET, SOCK_DGRAM, 0);

	bind(sockfd, (struct sockaddr *) &server, sizeof(server));

	int packets_recvd = 0;

	bool corrupt_packet = false;

	for (;;) {
		/* recieve data */
		struct packet p;
		int num_bytes = recvfrom(sockfd, &p, /*size of a packet*/ 68, /*flags*/ 0, (struct sockaddr *) &server, &socket_length);
		printf("RECV {seq: %d, checksum: %d, msg: \"%s\"}\n", p.seq, p.checksum, p.msg);

		/* check packet */
		if (p.seq == packets_recvd) {
			// intentionally make the 4th packet fail exactly once
			if (p.seq == 4 && corrupt_packet) {
				p.checksum = 1;
				corrupt_packet = true;
			}

			if (checksum(p.msg) != p.checksum) {
				printf("Bad checksum (recv'd: %d, expected: %d).\n", p.checksum, checksum(p.msg));
				return -1;
			}

			packets_recvd++;
			p.seq++;

			// intentionally corrupt sequence number
			if (p.seq == 4 && corrupt_packet) {
				packets_recvd--;	//	because it was incremented earlier
				p.seq--;
				corrupt_packet = false;
			}

			/* convert message to uppercase */
			int i = 0;
			while (p.msg[i]) {
				p.msg[i] = toupper(p.msg[i]);
				i++;
			}

			/* get new checksum */
			p.checksum = checksum(p.msg);

		}
		// else do not modify contents

		/* send data */
		sendto(sockfd, &p, sizeof(p), 0, (struct sockaddr *) &server, sizeof(server));
		printf("SENT {seq: %d, checksum: %d, msg: \"%s\"}\n", p.seq, p.checksum, p.msg);
	}

	close(sockfd);

	return 0;
}
