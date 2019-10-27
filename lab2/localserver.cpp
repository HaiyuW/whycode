
#include "local.h"

#define ADDR "/Users/a1100/Desktop/lab2/local"

using namespace std;

/**
*
* Haiyu Wang	
*
* Usage: ./localserver
*
* create local socket named local and listen for connection
* accept clients and print what clients write.
* use man 2 bind example code as a starting point. 
*
*/

int main (int argc, char *argv[])
{
	int sockfd,clientfd;
	struct sockaddr_un serverAdr, clientAdr;
	socklen_t clientAdr_size;
	char buf[1000];
	int bufsize;
	extern int errno;

	// create a socket for connection.
	sockfd = socket(AF_LOCAL, SOCK_STREAM, 0);
	if (sockfd == -1){
		printf("Program error, reason: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	// clear the server struct
	memset(&serverAdr, 0, sizeof(struct sockaddr_un));

	serverAdr.sun_family = AF_LOCAL;

	// Assign a path to server 
	strncpy(serverAdr.sun_path, ADDR, 
			sizeof(serverAdr.sun_path)-1);

	// bind socket with server address
	if (bind(sockfd,(struct sockaddr *) &serverAdr,
			sizeof(serverAdr)) == -1){
		printf("Program error, reason: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	// listen for connection, time limit is 20
	if (listen(sockfd, 20)== -1){
		printf("Program error, reason: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	clientAdr_size = sizeof(clientAdr);

	int redzone = shm_open("redzone",O_RDWR, S_IRWXU);
	struct redzone *redzone_start;
	redzone_start = (struct redzone *) mmap(NULL, sizeof(redzone_start), PROT_READ | PROT_WRITE, MAP_SHARED, redzone, 0);


	clientfd = accept(sockfd, (struct sockaddr *) &clientAdr, &clientAdr_size);


	if (clientfd == -1)	{
		printf("Program error, reason: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	} else {
		printf("New Connection Established\n");
	}

	// use a loop to accept multiple client
	while (1) {
		
		char msg[BUFSIZE];

		recv(clientfd, msg, BUFSIZE,0);
		cout << "Signal Reveived: " << msg << endl;
		
		if (strcmp(msg,"ipc_close")==0) {
			struct redzone *s = (struct redzone *)redzone_start;
			int close_ret_value = close(s->fd);
			if (close_ret_value<0)
				handle_error("close");
			char ret[BUFSIZE];
			sprintf(ret, "%d", close_ret_value);
			send(clientfd, ret, sizeof(ret),0);
		} else if (strcmp(msg,"ipc_open")==0) {
			struct redzone *s = (struct redzone *)redzone_start;
			int fd = open(s->filename,s->flags,s->mode);
			char ret[BUFSIZE];
			sprintf(ret, "%d", fd);
			send(clientfd, ret, sizeof(ret),0);
		} else if (strcmp(msg,"ipc_read")==0)	{
			// int read_fd = shm_open("read", O_RDWR, S_IRWXU);
			struct redzone *s = (struct redzone *)redzone_start;
			// cout << "s->fd:" << s->fd << endl;
			// cout<< "s->index: "<<s->index<<endl;
			ssize_t count=read(s->fd,s->str_mat[s->index],s->read_size);
			if (count<0)
				handle_error("read");
			// cout << s->str_mat[s->index] << endl;
			// printf("%d %d\n", s->buf, s->str_mat[s->index]);

			char ret[BUFSIZE];
			sprintf(ret, "%d", count);
			send(clientfd, ret, sizeof(ret),0);
		} else if (strcmp(msg,"ipc_write")==0) {
			struct redzone *s = (struct redzone *)redzone_start;
			ssize_t count=write(s->fd,s->str_mat[s->index],s->read_size);
			if (count<0)
				handle_error("write");
			char ret[BUFSIZE];
			sprintf(ret, "%d", count);
			send(clientfd, ret, sizeof(ret),0);
			// ssize_t count = write(s->fd,s->buf,s->read_size);
		} else if (strcmp(msg,"ipc_terminate")==0) {
			break;
		}
	}

	close(sockfd);
	unlink(ADDR);

	return 0;
}
