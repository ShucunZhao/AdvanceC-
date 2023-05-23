#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <error.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/fcntl.h>
#include <poll.h>

/*
   The difference between select multiplexing and poll is the data structure to save the socket set inside, the select uses bitmap while the poll uses array. So the poll has no maxValue 1024 resctriction.
*/

//The size restriction of array in poll which depends on the demand of task.
//(It's not like the bitmap size can only be the max is 1024, the max in here is the max in linux which can use command line "ulimit -n" to check.)
#define MAXNFDS 1024

//Initial the listening port of server.
int initserver(int port);

int main(int argc, char*argv[]){
	if(argc!=2){
		printf("Usage: ./selectServer port\n");
		return -1;
	}

	//Initial the socket for listening in server.
	int listensock = initserver(atoi(argv[1]));
	printf("listensock=%d\n", listensock);
	if(listensock < 0){
		printf("initserver() failed.\n");
		return -1;
	}

	//fd_set readfdset; //This is the bitmap under the select mode.
	int maxfd; // The size of socket we need to keep watch over in fds array.`
	struct pollfd fds[MAXNFDS];//fds is used for saving the socket which is need to be watched over. 

	//Initial the poll array and set all fd be -1.
	for(int i=0;i<MAXNFDS;i++){
		fds[i].fd = -1;
	}	
	
	//Add the listensock to the array.
	fds[listensock].fd = listensock;
	fds[listensock].events = POLLIN;//There are three data readable envent, including the new client connect, the client socket has data readable and the client disconnect cases.
	maxfd = listensock;

	while(1){
		int infds = poll(fds, maxfd+1, -1);
		//int infds = poll(fds, maxfd+1, 5000);
		//printf("poll infds=%d\n", infds);

		// Print error.
		if(infds<0){
			printf("poll() failed.\n");
			perror("poll()");
			break;
		}
		
		//Out of time limit: In this code the last parameter of select is empty, which means the out of time will not happen, but we still keep the code below:
		if(infds==0){
			printf("poll() timeout.\n");
			continue;
		}

		// Check the happening socket, including the listening and client socket.(We need to traverse the whole set beacause there may be multiple socket cases.)
		for(int eventfd=0;eventfd<=maxfd;eventfd++){
			if(fds[eventfd].fd<0){
				continue;
			}
			if((fds[eventfd].revents&POLLIN)==0){
				continue;
			}
			fds[eventfd].revents=0;//Clear up the revents.

			if(eventfd==listensock){
				//If the happened event was listensock, which means there had new client linked.
				struct sockaddr_in client;
				socklen_t len = sizeof(client);
				int clientsock = accept(listensock, (struct sockaddr*)&client, &len);
				if(clientsock<0){
					printf("accept() failed.\n");
					continue;
				}

				printf("client(socket=%d) connected ok.\n", clientsock);

				if(MAXNFDS<clientsock){
					printf("clientsock(%d)>MAXNFDS(%d)\n", clientsock, MAXNFDS);
					close(clientsock);
					continue;
				}

				fds[clientsock].fd = clientsock;
				fds[clientsock].events = POLLIN;
				fds[clientsock].revents = 0;
				if(maxfd<clientsock) maxfd = clientsock;
				
				printf("maxfd=%d\n", maxfd);
				continue;
			}
			else{
				//The case that client sent data here or the client sock was cut.
				char buffer[1024];
				memset(buffer,0,sizeof(buffer));

				//Read the data from client.
				ssize_t isize = read(eventfd, buffer, sizeof(buffer));

				//If error or socket was closed by other side.
				if(isize <=0){
					printf("client(eventfd=%d) disconnected.\n", eventfd);
					close(eventfd);//Close the client socket.
					
					fds[eventfd].fd = -1;

					//Re-calculate the maxfd and we only need to do this when eventfd==maxfd.
					if(eventfd==maxfd){
						for(int i=maxfd;i>=0;i--){
							if(fds[i].fd!=-1){
								maxfd = i;
								break;
							}
						}
						printf("maxfd=%d\n", maxfd);
					}
					continue;
				}
				printf("recv(eventfd=%d, size=%d):%s\n", eventfd, isize, buffer);
				//Sent the message back to the client after receiving.
				write(eventfd,buffer,strlen(buffer));
			}
		}
	}
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

