#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define DEST_IP   "127.0.0.1"
#define BUFFER_SIZE 512

int main(int argc, char ** argv)
{
    int sockfd, port,k;
    char sent_buf[BUFFER_SIZE];
	int sent_len;
	char recv_buf[BUFFER_SIZE];
	int recv_len;
    int count_timeout=0;
    struct sockaddr_in dest_addr;   // will hold the destination addr
    fd_set read_fds; // temp file descriptor list for select()
    fd_set master; 
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 500000;
    FD_ZERO(&read_fds);
    FD_ZERO(&master);
    int read_fds_rv=0;
    //int recv_fds_rv =0;
    
	if( argv[1] != NULL){
		port = atoi(argv[1]);
	}
    sockfd = socket(PF_INET, SOCK_STREAM, 0); // do some error checking!

    dest_addr.sin_family = AF_INET;          // host byte order
    dest_addr.sin_port = htons(port);   // short, network byte order
    dest_addr.sin_addr.s_addr = inet_addr(DEST_IP);
    memset(&(dest_addr.sin_zero), '\0', 8);  // zero the rest of the struct

    // don't forget to error check the connect()!
    if( connect(sockfd, (struct sockaddr *)&dest_addr, sizeof(struct sockaddr)) <0){
    	printf("ERR_CONNECTION\n");
    	return 0;
    }
	
	printf("PROGRAM START\n");
	FD_SET(0, &read_fds); // add standard input into read_fds
	FD_SET(sockfd, &read_fds);	// add socket descriptor into read_fds
	FD_SET(0, &master);
	FD_SET(sockfd, &master);
    while(1){
    	read_fds = master;
    	read_fds_rv = select(sockfd+1, &read_fds, NULL, NULL, &tv);    	
    	if( read_fds_rv < 0){
    		perror("select"); // error occurred in select()
    	}
    	else if ( read_fds_rv == 0 ){
    		sleep(1);
    		count_timeout += 1;
    		//printf("count_timeout %d\n", count_timeout);
    		if ( count_timeout == 30 ){
    			printf("client wants to disconnect \n");
    			send(sockfd, NULL, 1, 0);
    			break;
    		}
    	}
    	else {
    		if ( FD_ISSET(0, &read_fds)) {
    			count_timeout =0;        		
        		sent_len = read(0, sent_buf, BUFFER_SIZE);        		
        		send(sockfd, sent_buf, sent_len-1, 0);
   			}
   			if( FD_ISSET(sockfd, &read_fds) ){
   				//printf("client will receive some data\n");
   				recv_len = recv( sockfd, recv_buf, BUFFER_SIZE, 0);
        		for(k=0; k<recv_len; k++)
                	printf("%c", recv_buf[k]);
        		printf("\n");
   			}
    	}
    	
	}
	close(sockfd);
	return 0;
}
