//server.cpp
#include<sys/types.h>
#include<sys/socket.h>
#include<strings.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<errno.h>
#include<signal.h>
#include<sys/wait.h>

#define LISTEN_PORT 84 //server port
#define REV 1024*1024
#define RECEIVE 512

//@sockfd :client socket 
//@return : char* ptr of the instream
char* str_read(int sockfd)
{
    ssize_t n;
    char line[RECEIVE];
	char* instream=(char*)malloc(sizeof(char)*REV);
    //printf("ready to read\n");
    while( (n=read(sockfd,line,RECEIVE))>0 )
    {
            line[n]='\0';
			strcat(instream,line);
            bzero(&line,sizeof(line));
    }
	return instream;   
}

int main(int argc, char **argv)
{
    int listenfd, connfd;
    pid_t childpid;
    socklen_t chilen;

    struct sockaddr_in chiaddr,servaddr;

    listenfd=socket(AF_INET,SOCK_STREAM,0);
    if(listenfd==-1)
    {
        printf("socket established error: %s\n",(char*)strerror(errno)); return -1;
    }

    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
    servaddr.sin_port=htons(LISTEN_PORT);
//bind
    int bindc=bind(listenfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
    if(bindc==-1)
    {
        printf("bind error: %s\n",strerror(errno)); return -1;
    }
	printf("listening on %d::%d\n",servaddr.sin_addr.s_addr,servaddr.sin_port);
//listen
    listen(listenfd,5);
    for(;;)
    {
        chilen=sizeof(chiaddr);

        connfd=accept(listenfd,(struct sockaddr*)&chiaddr,&chilen);
        if(connfd==-1)
        {
			printf("accept client error: %s\n",strerror(errno)); return -1; 
		}else{
            printf("client connected\n");
		}
		
        if((childpid=fork())==0)
        {
			//char* msg_rev=inet_ntoa(chiaddr.sin_addr);
			//strcat(instream,msg_rev);
            close(listenfd);
            printf("thread %d :client from %s\n",childpid,inet_ntoa(chiaddr.sin_addr));
            char* instream = str_read(connfd);
			printf("end read:%s\n",instream);
			free(instream);
            exit(0);    
        }
        else if (childpid<0)
            printf("fork error: %s\n",strerror(errno));
        close(connfd);
    }
}