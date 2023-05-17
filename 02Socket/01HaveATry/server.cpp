#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(int argc, char * argv[]){
	if(argc!=2){
		printf("Using:./server port\nExample:./server 5005\n\n");
		return -1;
	}

	//Step1: Create the socket of server.
	int listenfd;
	if((listenfd=socket(AF_INET,SOCK_STREAM,0))==-1){
		perror("socket");
		return -1;
	}

	//Step2: Bind the communicate address and port to socket in server.
	struct sockaddr_in servaddr; //The data structure of address info in server.
	memset(&servaddr,0,sizeof(servaddr));
	servaddr.sin_family = AF_INET;//protocol family, which only can be AF_INET in socket.
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);//Any ip address.
	//servaddr.sin_addr.s_addr = inet_addr("198.168.190.134");//Specific ip.
	servaddr.sin_port = htons(atoi(argv[1]));//Designate port of communication.
	if(bind(listenfd,(struct sockaddr *)&servaddr, sizeof(servaddr))!=0){
		perror("bind");
		close(listenfd);
		return -1;
	}

	//Step3: set the socket as listening mode.
	if(listen(listenfd,5)!=0){
		perror("listen");
		close(listenfd);
		return -1;
	}

	//Step4: receive the link with client.
	int clientfd; //the socket of client.
	int socklen=sizeof(struct sockaddr_in);//the size of struct sockaddr_in.
	struct sockaddr_in clientaddr; //The address info of client.
	clientfd = accept(listenfd, (struct sockaddr *)&clientaddr, (socklen_t*)&socklen);
	printf("Client (%s) was already connected.\n", inet_ntoa(clientaddr.sin_addr));

	//Step5: communicate with client, after receiving the message from client, reponse ok.
	char buffer[1024];
	while(1){
		int iret;
		memset(buffer, 0, sizeof(buffer));
		if((iret=recv(clientfd,buffer,sizeof(buffer),0))<=0){//Receive the request from client.
			printf("iret=%d\n", iret);
			break;
		}
		printf("Receive: %s\n", buffer);


		strcpy(buffer, "ok");

		if((iret=send(clientfd,buffer,strlen(buffer),0))<=0){//Send the response result to client.
			perror("send");
			break;
		}
		printf("Send: %s\n", buffer);
	}

	//Step6: Close the socket and free the resource.
	close(listenfd);
	close(clientfd);
}
