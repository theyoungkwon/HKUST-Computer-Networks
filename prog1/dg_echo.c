/*
 * dg_echo.c is the main function called by UDP server program (udp_server.c)
 */

#include <sys/types.h>
#include <sys/socket.h>

#define MAXMESG 2048

dg_echo(sockfd,pcli_addr,maxclilen)
int sockfd;
struct sockaddr *pcli_addr;
int maxclilen;
{
	int n,clilen;
	char mesg[MAXMESG];

	for ( ; ; ) {
		clilen=maxclilen;
		n=recvfrom(sockfd,mesg,MAXMESG,0,pcli_addr,&clilen);
		if (n<0)
			err_dump("dg_echo: recvfrom error");
              if (sendto(sockfd,mesg,n,0,pcli_addr,clilen)!=n)
		  	err_dump("dg_echo: sendto error");
	}
}
