/* udp_server.c
 * UDP server program 
 */

/* byte ordering routines

	ntohl	convert network-to-host, long integer
	ntohs	convert network-to-host, short integer 	
*/

/* byte operations
	
	bzero(char *dest, int nbytes); write the specified
		number of null bytes to the specified destination 
*/

#include "inet.h"

main(argc,argv)
int argc;
char *argv[];
{
	int sockfd;
	struct sockaddr_in serv_addr,cli_addr;

	pname=argv[0];

	if((sockfd=socket(AF_INET,SOCK_DGRAM,0))<0)
		err_dump("server:can't open datagram socket");

	bzero((char *) &serv_addr,sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=htonl(INADDR_ANY); /* INADDR_ANY is an old
						broadcast address */
	serv_addr.sin_port=htons(SERV_UDP_PORT);

	if(bind(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr))<0)
		err_dump("server: can't bind local address");

	/* echo routine */
	dg_echo(sockfd, (struct sockaddr *)&cli_addr,sizeof(cli_addr)); 
}

	
