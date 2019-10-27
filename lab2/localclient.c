#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#define ADDR "/Users/a1100/Desktop/lab2/local"
#define BUFSIZE 64

int ipc_close(int fd);
int socket_create(void);

/**
*
* Haiyu Wang	
*
* Usage: ./localclient
*
* request for connection to server through socket.
* write strings and send them to the server
* when sending 'quit', the server can be closed.
* use man 2 bind example code as a starting point. 
*
*/

int main (int argc, char *argv[])
{
	// int sockfd;
	// struct sockaddr_un serverAdr;
	// socklen_t clientAdr_size;
	// char buf[1000];
	// int bufsize;
	// extern int errno;

	// // open socket
	// sockfd = socket(AF_LOCAL, SOCK_STREAM, 0);
	// if (sockfd == -1)
	// {
	// 	printf("Program error, reason: %s\n", strerror(errno));
	// 	exit(EXIT_FAILURE);
	// }

	// // clear serverAdr and set serverAdr
	// memset(&serverAdr, 0, sizeof(struct sockaddr_un));

	// serverAdr.sun_family = AF_LOCAL;

	// strncpy(serverAdr.sun_path, ADDR, 
	// 		sizeof(serverAdr.sun_path)-1);

	// // connect to server
	// if (connect(sockfd,(struct sockaddr *) &serverAdr,
	// 		sizeof(serverAdr)) == -1)
	// {
	// 	printf("Program error, reason: %s\n", strerror(errno));
	// 	exit(EXIT_FAILURE);
	// }
	

	int fd1 = open("Makefile", O_RDONLY);
	ipc_close(fd1);

	
	

	// close(sockfd);
	
	return 0;

}

int ipc_close(int fd)
{
	int sockfd = socket_create();
	char msg[BUFSIZE];
	strcpy(msg,"ipc_close ");
	char cfd[20];
	sprintf(cfd,"%d",fd);
	strcat(msg,cfd);
	send(sockfd,msg,BUFSIZE,0);
	char buf[BUFSIZE];
	recv(sockfd,buf,BUFSIZE,0);
	printf("%s",buf);
	close(sockfd);
}

int socket_create(void)
{
	int sockfd;
	struct sockaddr_un serverAdr;
	socklen_t clientAdr_size;
	char buf[1000];
	int bufsize;
	extern int errno;

	// open socket
	sockfd = socket(AF_LOCAL, SOCK_STREAM, 0);
	if (sockfd == -1)
	{
		printf("Program error, reason: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	// clear serverAdr and set serverAdr
	memset(&serverAdr, 0, sizeof(struct sockaddr_un));

	serverAdr.sun_family = AF_LOCAL;

	strncpy(serverAdr.sun_path, ADDR, 
			sizeof(serverAdr.sun_path)-1);

	// connect to server
	if (connect(sockfd,(struct sockaddr *) &serverAdr,
			sizeof(serverAdr)) == -1)
	{
		printf("Program error, reason: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	return sockfd;
}
