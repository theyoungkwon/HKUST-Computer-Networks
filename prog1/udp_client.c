/* udp_client.c
 * UDP client program
 */

#include <netdb.h>
#include "inet.h"

main(argc,argv)
int argc;
char *argv[];
{
        struct in_addr *ptr;
        struct hostent *hostptr;
	int sockfd;
	struct sockaddr_in cli_addr,serv_addr;

	pname=argv[0];
	if(argc<2)
	{
		fprintf(stderr,"Usage: %s hostname\n", argv[0]);
		exit(1);

	}

        hostptr=gethostbyname(argv[1]);
        ptr = (struct in_addr *) *(hostptr->h_addr_list);
        
	printf("Server's IP address = %s\n",inet_ntoa(*ptr));

	bzero((char *) &serv_addr,sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=ptr->s_addr;
	serv_addr.sin_port=htons(SERV_UDP_PORT);

	if((sockfd=socket(AF_INET,SOCK_DGRAM,0))<0)
		err_dump("client: can't open datagram socket");

	bzero((char *) &cli_addr,sizeof(cli_addr));
	cli_addr.sin_family=AF_INET;
	cli_addr.sin_addr.s_addr=htonl(INADDR_ANY);	/* INADDR_ANY is an
							old broadcast address */
	cli_addr.sin_port=htons(0);

	if(bind(sockfd,(struct sockaddr *) &cli_addr,sizeof(cli_addr))<0)
		err_dump("client: can't bind local address");
	dg_cli(stdin,sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr));

	close(sockfd);
	exit(0);
}


