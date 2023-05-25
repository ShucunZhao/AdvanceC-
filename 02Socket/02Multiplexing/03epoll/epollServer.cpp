#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/epoll.h>

/*
   The epoll solved all the problem in select and poll, like the fdset copy and polling problem, use the most reasonalbe design and implementation scheme.
*/

#define MAXEVENTS 100

// Set the socket into non-blocking mode.
int setnonblocking(int sockfd);

//Initial the listening port of server.
int initserver(int port);

int main(int argc, char*argv[]){
	if(argc!=2){
		printf("Usage: ./epollServer port\n");//U must open this port firewall authority!
		return -1;
	}

	//Initial the socket for listening in server.
	int listensock = initserver(atoi(argv[1]));
	printf("listensock=%d\n", listensock);
	if(listensock < 0){
		printf("initserver() failed.\n");
		return -1;
	}

	int epollfd;

	char buffer[1024];
	memset(buffer, 0, sizeof(buffer));

	//Create a description symbol.
	epollfd = epoll_create(1);

	//Add the listening description symbol event.
	struct epoll_event ev;
	ev.data.fd = listensock;
	ev.events = EPOLLIN;
	epoll_ctl(epollfd, EPOLL_CTL_ADD, listensock, &ev);

	while(1){
		//int infds = poll(fds, maxfd+1, -1);
		//int infds = poll(fds, maxfd+1, 5000);
		
		struct epoll_event events[MAXEVENTS];//The struct array which store the happened events.

		//Waiting for the listening socket event happen.
		int infds = epoll_wait(epollfd, events,	MAXEVENTS, -1);
		//printf("epoll_wait infds=%d\n", infds);

		// Print error.
		if(infds<0){
			printf("epoll_wait() failed.\n");
			perror("epoll_wait()");
			break;
		}
		
		//Out of time limit: In this code the last parameter of select is empty, which means the out of time will not happen, but we still keep the code below:
		if(infds==0){
			printf("poll() timeout.\n");
			continue;
		}

		// Traverse the happened events struct array.
		for(int i=0;i<infds;i++){
			if((events[i].data.fd==listensock)&&(events[i].events&EPOLLIN)){
			//If the happened event was listensock, which means there had new client linked.
				struct sockaddr_in client;
				socklen_t len = sizeof(client);
				int clientsock = accept(listensock, (struct sockaddr*)&client, &len);
				if(clientsock<0){
					printf("accept() failed.\n");
					continue;
				}

				//Add the new client into epoll.
				memset(&ev, 0, sizeof(struct epoll_event));
				ev.data.fd = clientsock;
				ev.events = EPOLLIN;
				epoll_ctl(epollfd, EPOLL_CTL_ADD, clientsock, &ev);
				printf("client(socket=%d) connected ok.\n", clientsock);
				continue;
			}
			else if(events[i].events & EPOLLIN){
				//The case that client sent data here or the client sock was cut.
				char buffer[1024];
				memset(buffer,0,sizeof(buffer));

				//Read the data from client.
				ssize_t isize = read(events[i].data.fd, buffer, sizeof(buffer));

				//If error or socket was closed by other side.
				if(isize <=0){
					printf("client(eventfd=%d) disconnected.\n", events[i].data.fd);
					//Delete the disconnected client from epoll.
					memset(&ev, 0, sizeof(struct epoll_event));
					ev.events = EPOLLIN;
					ev.data.fd = events[i].data.fd;
					epoll_ctl(epollfd, EPOLL_CTL_DEL, events[i].data.fd, &ev);
					close(events[i].data.fd);
					continue;
				}
				printf("recv(eventfd=%d, size=%d):%s\n", events[i].data.fd, isize, buffer);
				//Sent the message back to the client after receiving.
				write(events[i].data.fd,buffer,strlen(buffer));
			}
		}
	}
	close(epollfd);
	return 0;
}

//Initial the listening port of server.
int initserver(int port){
	int sock = socket(AF_INET,SOCK_STREAM,0);
	if(sock<0){
		printf("socket() failed.\n");
		return -1;
	}
	
	//Linux env setting.
	int opt = 1;
	unsigned int len = sizeof(opt);
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, len);
	setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, &opt, len);
	
	struct sockaddr_in servaddr;
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(port);
	
	if(bind(sock,(struct sockaddr*)&servaddr,sizeof(servaddr)) < 0){
		printf("bind() failed.\n");
		close(sock);
		return -1;
	}

	if(listen(sock, 5)!=0){
		printf("listen() failed.\n");
		close(sock);
		return -1;
	}
	return sock;
}

// Func of setting the socket into non-blocking mode.
int setnonblocking(int sockfd){
	if(fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFD, 0)|O_NONBLOCK)==-1){
		return -1;
	}
	return 0;
}


