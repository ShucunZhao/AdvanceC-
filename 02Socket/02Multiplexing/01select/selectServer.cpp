#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <error.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/fcntl.h>

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

	fd_set readfdset; //The set of read event, including the listening socket and the client socket.
	int maxfd; // The max socket value of readfdset.
	
	//Initial the struct and add the listensock to the set.
	FD_ZERO(&readfdset);

	FD_SET(listensock, &readfdset);
	maxfd = listensock;

	while(1){
		// When the select func is called, the content of socket set will change, so we need to save the socket set and pass a temporary one to select.
		fd_set tmpfdset = readfdset;

		int infds = select(maxfd+1, &tmpfdset, NULL, NULL, NULL);
		//printf("select infds=%d\n", infds);

		// Print error.
		if(infds<0){
			printf("select() failed.\n");
			perror("select()");
			break;
		}
		
		//Out of time limit: In this code the last parameter of select is empty, which means the out of time will not happen, but we still keep the code below:
		if(infds==0){
			printf("select() timeout.\n");
			continue;
		}

		// Check the happening socket, including the listening and client socket.(We need to traverse the whole set beacause there may be multiple socket cases.)
		for(int eventfd=0;eventfd<=maxfd;eventfd++){
			if(FD_ISSET(eventfd, &tmpfdset)<=0){
				continue;
			}
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

				//Add the new client socket into the set.
				FD_SET(clientsock, &readfdset);

				if(maxfd<clientsock){
					maxfd = clientsock;
				}
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
					FD_CLR(eventfd, &readfdset);//Remove the client socket from set.
					//Re-calculate the maxfd and we only need to do this when eventfd==maxfd.
					if(eventfd==maxfd){
						for(int i=maxfd;i>=0;i--){
							if((FD_ISSET(i, &readfdset))){
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

