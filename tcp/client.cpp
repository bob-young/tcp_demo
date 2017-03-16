//client.cpp
#include<sys/types.h>
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/socket.h>
#include<strings.h>
#include<string.h>
#include<arpa/inet.h>
#include<errno.h>

//#define SERVER_PORT 84
#define SEND 512

typedef unsigned char byte;


//@data :input message string
//@sockfd :sock handler
//return : -1 error
//		 : 0 success	
int str_cli(byte *data,int sockfd)
{
    ssize_t ret =write(sockfd,data,strlen((char*)data));
	if(ret == -1){
		printf("sending message error: %s\n",(char*)strerror(errno));
		return -1;
	}else if(ret >= 0){
		printf("send data bytes %ld\n",ret);
		return 0;
	}else{
		printf("unknow error!\n");	
	}
    return -1;
}
int main(int argc, char **argv)
{
    int sockfd;
    struct sockaddr_in servaddr;
	if(argc==1||argc==2){
		printf("error:please input (IP PORT)\n");
		return -1;
	}

    if(argc>4){
		printf("error:too many arguments!\n");
        return -1;
	}

	if(argc==3){
		printf("warning:you are sending an empty message to server\n");
	}

    sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd==-1)
    {
        printf("socket established error: %s\n",(char*)strerror(errno)); return -1;
    }

    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(atoi(argv[2]));//set port
    inet_pton(AF_INET,argv[1],&servaddr.sin_addr);//set ip

    printf("client try to connect\n");
    int conRes=connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
    if(conRes==-1)
    {    
        printf("connect error: %s\n",strerror(errno)); return -1;
    }else{
		printf("connect success\n");	
	}

	int remain=strlen((char*)argv[3]);
	printf("prepare to send %d messages\n",remain);
	
//message devide
	byte* send_ptr=(byte*)argv[3];
	for(;;){
		byte stream[SEND];
		memcpy(stream,send_ptr,SEND);
		remain-=SEND;
		stream[SEND]='\0';
    	str_cli(stream,sockfd);
		if(remain <= 0){
			break;
		}else{
			send_ptr+=(SEND*sizeof(char));
		}
	}
	//byte msg[]="_END_";
	//str_cli(msg,sockfd);
	close(sockfd);
    exit(0);
}
