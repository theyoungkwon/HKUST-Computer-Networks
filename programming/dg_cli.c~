/*  dg_cli.c
 *  main function called by UDP client (udp_client.c)
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ddi.h>

#define MAXLINE 512

dg_cli(fp,sockfd,pserv_addr,servlen)
FILE *fp;
int sockfd;
struct sockaddr *pserv_addr;
int servlen;
{   
    int n,i;
    char sendline[MAXLINE], recvline[MAXLINE];
    for(;;) {
            if (fgets(sendline,MAXLINE,fp)!=NULL) {                                
      		n=strlen(sendline);
                  if(sendto(sockfd,sendline,n,0,pserv_addr,servlen)!=n)
		        err_dump("dg_cli: sendto error on socket");
            }
                 if (ferror(fp))
		        err_dump("dg_cli: error reading file");
        	n=recvfrom(sockfd,recvline,MAXLINE,0,pserv_addr,&servlen);
		    if (n<0)
			    err_dump("dg_cli: recvfrom error");
		recvline[n]=0;
		fputs(recvline,stdout);
    }
}
