/*=================================================
**Discription		:
**Author			: 	vincenzo
**Date			: 	2015.9.15 0:28
**File			: 	main.c
**=================================================
*/
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "main.h"

 int sockfd;
struct sockaddr_in server_addr;


void mono_socket_init(void)
{
    if(-1 == (sockfd=socket(AF_INET,SOCK_STREAM,0)))
    {
        mono_info("http socket init error!");
        EXIT(1);
    }

    MEMSET(&server_addr,0,sizeof(server_addr));
    server_addr.sin_family  =   AF_INET;
    server_addr.sin_port     =  HTONS(HTTP_PORT);
    server_addr.sin_addr    =   *((struct in_addr *));

}

void mono_http_send(void)
{
    char header[BUFSIZE]; 
    char *get_url[] = "/testpoe/em/poeem.ashx ";
    
    mono_debug("%s %d\n", __FUNCTION__, __LINE__);

    strcat(header,"GET ");
    strcat(header,get_url);  
    strcat(header,"HTTP/1.1\r\n");  
    strcat(header,"Accept: */*\r\n");  
    strcat(header,"User-Agent: Mozilla/4.0\r\n");  
    strcat(header,"Host: 120.24.56.252\r\n");  
    strcat(header,"Connection: Keep-Alive\r\n");  
    strcat(header,"Cache-Control: no-cache\r\n"); 
    strcat(header,"\r\n\r\n");  

    connect(sockfd,(struct sockaddr *)&server_addr, sizeof(server_addr));  
    send(sockfd,header,strlen(header),0);  

}

void mono_http_recv(void)
{
    mono_debug("%s %d\n", __FUNCTION__, __LINE__);
}

int main(int argc, char **argv)
{
    PID_T sid,rid;
    int ret1, ret2;

    mono_socket_init();

    ret1 = PTHREAD_CREATE (&sid, NULL, (void *)mono_http_send, NULL);	
    ret2 = PTHREAD_CREATE (&rid, NULL, (void *)mono_http_recv, NULL);
    
    if ((ret1 != 0) || (ret2 != 0)) {		
        mono_info("create thread failed!\n");     
        EXIT(1);    
    }
    
    mono_info("http thread create ok!\n");

    while(1){}
    
    return 0;
}

