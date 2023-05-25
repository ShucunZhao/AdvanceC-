#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <error.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main(int argc, char * argv[]){
	if(argc!=3){
		printf("Usage:./client ip port\n");//KeyPoint!! U must open this port in your firewall and cloud safety setting.
		return -1;
	}

	int sockfd;
	struct sockaddr_in servaddr;
	char buff[1024];
	if((sockfd=socket(AF_INET,SOCK_STREAM,0))<0){
		printf("socket() failed.\n");
		return -1;
	}
	memset(&servaddr,0,sizeof(servaddr));
	servaddr.sin_family=AF_INET;
	servaddr.sin_port=htons(atoi(argv[2]));
	servaddr.sin_addr.s_addr=inet_addr(argv[1]);

	if(connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr))!=0){
		printf("connect(%s:%s) failed.\n", argv[1], argv[2]);
		close(sockfd);
		return -1;
	}
	printf("connect ok.\n");

	for(int i=0;i<10000;i++){
		//Input the content from command line.
		memset(buff, 0, sizeof(buff));
		printf("Please input:\n");
		scanf("%s", buff);
		//sprintf(buf, "1111111111111111111i%08d", i);
		if(write(sockfd,buff,strlen(buff))<=0){
			printf("write() failed\n");
			close(sockfd);
			return -1;
		}

		memset(buff,0,sizeof(buff));
		if(read(sockfd,buff,sizeof(buff))<=0){
			printf("read() failed.\n");
			close(sockfd);
			return -1;
		}
		printf("recv:%s\n", buff);

		//close(sockfd);
		//break;
	}
}
