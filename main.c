/*=================================================
**Discription		:
**Author			: 	vincenzo
**Email                  :      <liyulinga@qq.com>
**Date			: 	2015.9.20 9:28
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
#include <time.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "main.h"

 int sockfd;
struct sockaddr_in server_addr;

uint8_t token[]="F9EEBDDAE5D79EB0E64043E2DEE0FCDC";
uint8_t data[]="{\"type\":\"PING\",\"gw_id\":\"70:3A:D8:00:22:48\"}";
uint8_t recv_buf[RECV_BUF_SIZE] = {0};

static void random_chars(char *buf, uint32_t len) 
{
    struct timeval tv; 
    uint8_t src[] = "0123456789";
    uint32_t i ;
    
    if (!buf || len == 0) {
        return;
    }
    
    gettimeofday(&tv, NULL);
    srand(tv.tv_sec + tv.tv_usec);
    
    for ( i = 0; i < len; ++i) {
        buf[i] = src[rand() % 10];
    }
}

#if 0
static unsigned char char_to_hex( unsigned char x ) 
{ 
    return (unsigned char)(x > 9 ? x + 55: x + 48); 
} 

static int is_alpha_number_char( unsigned char c ) 
{ 
    if ( (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') ) 
        return 1; 
    return 0; 
} 

void urlencode( unsigned char * src, int  src_len, unsigned char * dest, int  dest_len ) 
{ 
    unsigned char ch; 
    int  len = 0; 

    while (len < (dest_len - 4) && *src) 
    { 
        ch = (unsigned char)*src; 
        if (*src == ' ') 
        { 
            *dest++ = '+'; 
        } 
        else if (is_alpha_number_char(ch) || strchr("-_.!~*'()", ch)) 
        { 
            *dest++ = *src; 
        } 
        else 
        { 
            *dest++ = '%'; 
            *dest++ = '%';
            *dest++ = char_to_hex( (unsigned char)(ch >> 4) ); 
            *dest++ = char_to_hex( (unsigned char)(ch % 16) ); 
        }  
        ++src; 
        ++len; 
    } 
    *dest = 0; 
    return ; 
} 

unsigned char* urldecode(unsigned char* encd,unsigned char* decd) 
{ 
    int j,i; 
    char *cd = encd; 
    char p[2]; 
    unsigned int num; 
     j=0; 

    for( i = 0; i < strlen(cd); i++ ) 
    { 
        memset( p, '/0', 2 ); 
        if( cd[i] != '%' ) 
        { 
            decd[j++] = cd[i]; 
            continue; 
        } 

        p[0] = cd[++i]; 
        p[1] = cd[++i]; 

        p[0] = p[0] - 48 - ((p[0] >= 'A') ? 7 : 0) - ((p[0] >= 'a') ? 32 : 0); 
        p[1] = p[1] - 48 - ((p[1] >= 'A') ? 7 : 0) - ((p[1] >= 'a') ? 32 : 0); 
        decd[j++] = (unsigned char)(p[0] * 16 + p[1]); 

    } 
    decd[j] = '/0'; 

    return decd; 
} 
 #endif

 void mono_dump_hex(const char *buf, int len)
{
    int i;

    for(i = 0; i < len; i++)
    {
        mono_debug("%02x ", buf[i]);
        if (i%16==15)
        {
            mono_debug("\n");
        }
    }
    mono_debug("\n");
}

void mono_dojson(const char *jtext)
{   
    char *out;
    cJSON *json;      

    json=cJSON_Parse(jtext); 
    if (!json) 
    {
        mono_info("Error before: [%s]\n",cJSON_GetErrorPtr());
    }    
    else    
    {       
        out = cJSON_Print(json);      
        cJSON_Delete(json);     
        mono_info("%s\n",out);     
        FREE(out);  
    }
}

void mono_delchar(char  *p, char ch)
{
    int i,j=0;

    if (NULL == p)
    {
        return;
    }
    
    for ( i = 0; p[i]!='\0'; i ++ ) 
    {
        if(p[i] != ch)
        p[j++] = p[i];
    }
    
    p[j] = '\0';
}


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
    server_addr.sin_addr.s_addr   =   inet_addr("119.29.83.85");

}


void mono_http_send(void)
{
    uint8_t header[BUFSIZE]; 
    int time1;
    uint8_t sha[128], rand[5];

    while(1)
    {
        MEMSET(header, 0, BUFSIZE);
        MEMSET(sha, 0, 128);
        MEMSET(rand, 0, 5);
        time1 = 0;
        
        time1 = time(NULL);
        random_chars(rand, 5);
        mono_debug("time: %d rand: %s \n", time1, rand);
        
        sprintf(header, "%d", time1);
        strcat(header,  rand);
        strcat(header,  data);
        strcat(header,  token);
        
        mono_debug("orgsha1: %s\n", header);
        sha1(header, sha);

        MEMSET(header, 0, BUFSIZE);
        sprintf(header,"GET /testpoe/em/poeem.ashx?timestamp=%d&nonce=%c%c%c%c%c&data=%%7b%%22type%%22%%3a%%22PING%%22%%2c%%22gw_id%%22%%3a%%2270%%3a3A%%3a+D8%%3a+00%%3a22%%3a48%%22%%7d&signature=%s  HTTP/1.1\r\n",
            time1, rand[0], rand[1],rand[2],rand[3],rand[4], sha);
        strcat(header,"Accept: text/html, application/xhtml+xml, image/jxr, */*\r\n"); 
        strcat(header,"Accept-Language: en-US,en;q=0.8,zh-Hans-CN;q=0.5,zh-Hans;q=0.3\r\n"); 
        strcat(header,"User-Agent: Mozilla/5.0 (Windows NT 10.0; WOW64; Trident/7.0; rv:11.0) like Gecko\r\n"); 
        strcat(header,"Host: cloud.owafa.com\r\n"); 
        strcat(header,"DNT: 1\r\n"); 
        strcat(header,"Connection: Keep-Alive\r\n\r\n"); 

        connect(sockfd,(struct sockaddr *)&server_addr, sizeof(server_addr)); 
        if (send(sockfd,header,strlen(header),0) <0)
        {
            mono_info("Send http request failed!\n");
            continue;
        }
        sleep(50);
    }
}

/*PONG: {"timestamp":"1432192686","nonce":"36678","data":{"type":"PONG"},"signature":"396CE82D80A225C82AC1E47381BD95D19FE469A0"}*/
/*RESTART: {"timestamp":"1432192686","nonce":"36678","data":{"type":"RESTART"},"signature":"396CE82D80A225C82AC1E47381BD95D19FE469A0"}*/
void mono_http_recv(void)
{
    int bytesRecv;
    uint8_t *pjson;
    cJSON *root;
    cJSON *msignature;
    cJSON *mdata;
    cJSON *mtime;
    cJSON *mnonce;
    cJSON *mtype;
    uint8_t header[512] = {0};
    uint8_t sha[128] = {0};
    uint8_t *mdata_buf;

    while (1)
    {
        if ((bytesRecv = recv(sockfd, recv_buf, RECV_BUF_SIZE, 0)) <= 0)
        {
            continue;
        }

        pjson = strchr(recv_buf, '{');

        mono_debug("Receive json: %s\n",pjson);

        root = cJSON_Parse(pjson);
        if (root)mono_debug("root type: %d\n", root->type);
        
        msignature = cJSON_GetObjectItem(root,"signature");  
        if (msignature)mono_debug("signature type: %d\n", msignature->type);

        mnonce = cJSON_GetObjectItem(root,"nonce");
        if (mnonce)mono_debug("nonce type: %d v:%s\n", mnonce->type, mnonce->valuestring);
        
        mtime = cJSON_GetObjectItem(root,"timestamp");
        if (mtime)mono_debug("timestamp type: %d, v:%s\n", mtime->type, mtime->valuestring);

        mdata = cJSON_GetObjectItem(root,"data"); 
        if (mdata)mono_debug("mdata type: %d\n", mdata->type);

        mtype = cJSON_GetObjectItem(mdata,"type");
        if(mtype)mono_debug("mtype type: %d\n", mtype->type);

        /*check signature*/
        sprintf(header, "%s%s", mtime->valuestring, mnonce->valuestring);
        mdata_buf = cJSON_Print(mdata);
        mono_dump_hex(mdata_buf, strlen(mdata_buf));
        mono_debug("%s\n", mdata_buf);
        mono_delchar(mdata_buf, '\n');
        mono_delchar(mdata_buf, '\t');
        strcat(header,  mdata_buf);
        strcat(header,  token);

        mono_debug("recvorgsha: %s\n", header);
        sha1(header, sha);
        mono_debug("recvsha: %s localsha: %s\n", msignature->valuestring, sha);

        if (0 != MEMCMP(msignature->valuestring, sha, strlen(sha)))
        {
            mono_info("Signature failed!local: %s server: %s\n", sha, msignature->valuestring);
            continue;
        }

        if (mtype->type == cJSON_String)
        {
            mono_debug("mtype valuestring: %s\n", mtype->valuestring);
            
            if (0 == MEMCMP(mtype->valuestring, "PONG", 4))
            {
                /*do nothing*/
                mono_info("PONG from server\n");
                goto out;
            }
            
            if (0 == MEMCMP(mtype->valuestring, "RESTART", 7))
            {
                /*restart machine*/
                mono_info("Restart macine now!!!\n");
            }
        }

out:
        MEMSET(recv_buf, 0, RECV_BUF_SIZE);
        MEMSET(header, 0, 512);
        MEMSET(sha, 0, 128);
        FREE(mdata_buf);
    }  
}

int main(int argc, char **argv)
{
    PID_T sid,rid;
    int ret1, ret2;

    mono_socket_init();
    cJSON_InitHooks(NULL);

    ret1 = PTHREAD_CREATE (&sid, NULL, (void *)mono_http_send, NULL);	
    ret2 = PTHREAD_CREATE (&rid, NULL, (void *)mono_http_recv, NULL);
    
    if ((ret1 != 0) || (ret2 != 0)) {		
        mono_info("create thread failed!\n");     
        EXIT(1);    
    }
    
    mono_info("Mono thread create OK!\n");

    PTHREAD_JOIN(sid, NULL);
    PTHREAD_JOIN(rid, NULL);
    
    return 0;
}

