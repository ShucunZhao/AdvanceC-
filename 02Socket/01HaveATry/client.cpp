#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(int argc, char * argv[]){

	if(argc!=3){
		printf("Using:./client ip port\nExample:./client 127.0.0.1 5005\n\n");
		return -1;
	}

	//Step1: Create the socket of client.
	int sockfd;
	if((sockfd = socket(AF_INET,SOCK_STREAM, 0))==-1){
		perror("socket");
		return -1;
	}

	//Step2: Request for linking with server.
	struct hostent * h;
	if((h=gethostbyname(argv[1]))==0){//Designate the ip address of server.
		printf("gethostbyname failed.\n");
		close(sockfd);
		return -1;
	}
	struct sockaddr_in servaddr;
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(atoi(argv[2]));//Designate the communicate port of server.
	memcpy(&servaddr.sin_addr, h->h_addr, h->h_length);
	if(connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr))!=0){//Ask for linking with server. 
		perror("connect");
		close(sockfd);
		return -1;
	}
	char buffer[1024];

	//Step3: communicate with server, send a message and wait for replying, then send the next one.
	for(int i=0;i<10;i++){
		int iret;
		memset(buffer,0,sizeof(buffer));
		sprintf(buffer, "This is the %d message, id%03d.", i+1, i+1);
		if((iret=send(sockfd,buffer,strlen(buffer),0))<=0){//Send the request message to server.
			perror("send");
			break;
		}
		printf("Send: %s\n", buffer);
		memset(buffer, 0, sizeof(buffer));
		if((iret=recv(sockfd,buffer,sizeof(buffer),0))<=0){//Receive the response message from sever.
			printf("iret=%d\n", iret);
			break;
		}
		printf("Receive: %s\n", buffer);
		sleep(1);
	}
	
	//Step4: Close the socket and free the source.
	close(sockfd);
}
