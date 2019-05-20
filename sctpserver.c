//SCTPServer.C To compile - gcc sctpserver.c - o server - lsctp
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/sctp.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#define RECVBUFSIZE             4096
#define PPID                    1234
#define PORT 8080
int main()
{
	int sock_fd,n,flags,iMsgSize;
	socklen_t from_len;
//	char a[1024];
       struct sockaddr_in serv_addr = {0};
       struct sctp_sndrcvinfo sinfo = {0};
       struct sctp_event_subscribe event = {0};
       char pRecvBuffer[RECVBUFSIZE] = {0};
	sock_fd=socket(AF_INET,SOCK_SEQPACKET,IPPROTO_SCTP);
	if(sock_fd<0)
	{
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	
	}
	if(setsockopt(sock_fd, IPPROTO_SCTP, SCTP_EVENTS, &event,sizeof(struct sctp_event_subscribe))<0)
	{
		perror("setsockopt failed");
		exit(EXIT_FAILURE);
	}
     
       serv_addr.sin_family = AF_INET;
       serv_addr.sin_port = htons(PORT);
       serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

       //bind to specific server address and port
       if(bind(sock_fd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr_in))<0)
       {
	       perror("setsockopt failed");
                exit(EXIT_FAILURE);

       }
      if( listen(sock_fd, 1)<0)
      {
	      perror("listen failed");
	      exit(EXIT_FAILURE);
      }
      int count = 0,i;
        char a[20][10],d[20][10];
        char send_data[1024];

        strcpy(a[0],"Abraham");
        strcpy(a[1],"Buck");
        strcpy(a[2],"Casper");
        strcpy(a[3],"Dammy");
        strcpy(d[0],"1");
        strcpy(d[1],"2");
        strcpy(d[2],"3");
        strcpy(d[3],"4");
     while(1)
     {
	     flags = 0;
             memset((void *)&serv_addr, 0, sizeof(struct sockaddr_in));
             from_len = (socklen_t)sizeof(struct sockaddr_in);
             memset((void *)&sinfo, 0, sizeof(struct sctp_sndrcvinfo));
	     n = sctp_recvmsg(sock_fd, (void*)pRecvBuffer, RECVBUFSIZE,(struct sockaddr *)&serv_addr, &from_len, &sinfo, &flags);
             printf("%d\n",n);
	     if(n==-1)
	     {
		     printf("Receive failed");
		     break;
		     close(sock_fd);
	     }

	     printf("Client:%s\n",pRecvBuffer);
	     int p = 0;
                     for(i=0;i<4;i++)
                     {
                        if(strcmp(pRecvBuffer,d[i]) == 0)
                        {
                             strcpy(send_data,a[i]);p=1;
                        }
                     }
                     if(p == 0)
                      strcpy(send_data,"No one on that role.");
		     //send message to client
               printf("Sending to client: %s\n", send_data);
               sctp_sendmsg(sock_fd, (const void *)send_data, strlen(send_data), (struct sockaddr *)&serv_addr, from_len, htonl(PPID), 0, 0 /*stream 0*/, 0, 0);

               //close server when exit is received
               if (0 == strcmp(pRecvBuffer, "exit"))
               {
                       break;
               }
	    /* printf("Sending Role  to server: ");
	     gets(a);
             iMsgSize=strlen(a);
             int send;
             send=sctp_sendmsg(sock_fd, (const void *)send_data, iMsgSize, NULL, 0,0, 0, 0 , 0, 0);
		printf("%d\n",send);
             if(send<0)
             {
             	  perror("send failed");
              	 exit(EXIT_FAILURE);
       	     }*/

     }
     close(sock_fd);
     return 0;

}
