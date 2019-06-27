/* 
 * recvudp.c 
 * 
 * A simple UDP receiver with multicast addressing
 * This mechanism based on the Simple Service Discovery Protocol (SSDP)
 * Reference:  https://tools.ietf.org/html/draft-cai-ssdp-v1-00
 *
 * THE SOFTWARE IS PROVIDED "AS IS", 
 * WITHOUT WARRANTY OF ANY KIND!
 *
 * Author: Andrejs Tihomirovs
 * Email: armtech@its.lv
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

int main(int argc, char *argv[])
{
	char* group = argv[1]; 		// must have: 239.255.255.250
	int port = atoi(argv[2]);	// must have: 1900

	if (argc != 3) {
		printf("recvudp: Invalid argument\n");
		printf("Try `recvudp 239.255.255.250 1900`\n");
		return 1;
	}

	// create UDP socket
    int sockfd;
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("recvudp");
		return 1;
	}

	// allow multiple sockets
    int optname = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, 
		(char *) &optname, sizeof(optname)) < 0) {
		perror("recvudp");
		return 1;
	}

	// set up destination address
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(port);

	// socket binding
	if (bind(sockfd, (struct sockaddr*) &addr, sizeof(addr)) < 0) {
		perror("recvudp");
		return 1;
	}

	// join to multicast group
	struct ip_mreq mreq;
	mreq.imr_multiaddr.s_addr = inet_addr(group);
	mreq.imr_interface.s_addr = htonl(INADDR_ANY);

	if (setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, 
		(char *) &mreq, sizeof(mreq)) < 0) {
		perror("recvudp");
		return 1;
	}

	// main loop
	while (1) {
		#define BUFSIZE 256
		char buf[BUFSIZE];
		int numbytes, addrlen = sizeof(addr);
		memset(buf, 0, sizeof(buf));

		// receive UDP message
		if ((numbytes = recvfrom(sockfd, buf, BUFSIZE-1 , 0,
			(struct sockaddr *)&addr, &addrlen)) == -1) {
			perror("recvudp");
			return 1;
		}

		printf("\n%sLEN:%d bytes\n", buf, numbytes);

	}

	return 0;
}
