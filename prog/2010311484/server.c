//-----------------------------------------------------------
// 2010311484 Kwon Young Dae
// SSE2030: Introduction to Computer Systems (Fall 2013)
//
// Skeleton code for PA#3
//
// October 6, 2013.
// Jinyong Ha
// Sungkyunkwan University
//
//-----------------------------------------------------------

#include "common.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>  

#include <stdlib.h>
#include <strings.h>
#include <arpa/inet.h>

#define MAXLINE 80
#define MAXDATA 2048
int mystrlen(char *a){		// ex) when "123", return value is 3
	int i;
	for(i=0; i<2048; i++){
		if( *(a+i) == NULL)
			break;
	}
	return i;
}

char *mystrcat(char *a, const char *b){	//return (arrA+arrB)
	int i, j;
	for(i=0; i<1024; i++){
		if( *(a+i) == 0)
			break;
	}
	for(j=0; j<1024; j++){
		*(a+i+j) = *(b+j);
		if( *(b+j) == 0)
			break;
	}
	return a;
}

char *mystrcpy(char *a, const char *b){	//return (arrB)
	int j;
	for(j=0; j<1024; j++){
		*(a+j) = *(b+j);
		if( *(b+j) == 0)
			break;
	}
	return a;
}
char *mystrncpy(char *a, const char *b, size_t n){	//return (arrB) with length n
	int j;
	for(j=0; j<n; j++){
		*(a+j) = *(b+j);
		if( *(b+j) == 0)
			break;
	}
	*(a+j) = NULL;
	return a;
}

void arrclear(char *a){	//make all element NULL
	int i;
	for(i =0; i<2048; i++){
		*(a+i) = 0;
	}
}

int mystrncmp(const char* a, const char* b, size_t n){ //if arrA and arrB are identical
	int i;															//for length n, return 0		
	for( i=0; i<n ; i++){
		if( *(a+i) > *(b+i) )
			return 1;
		if( *(a+i) < *(b+i) )
			return -1;
	}
	return 0;	
}
//find one char position from startpoint n
int myfindchar(const char* a, int n, const char b){	//if you can find 'b' in (arrA)
	int i;															//from n, return point i;
	for( i=(n+1); i<MAXDATA ; i++){							//if fail, return MAXDATA or
		if( *(a+i) == b)										//sizeof(arrA)
			return i;
		if(*(a+i) == NULL)
			return i;
	}
	return i;
}
int myfindchar2(const char* a, int n, const char b){	//if you can find 'b' reversely
	int i;										//(back to front) from n in (arrA), return point
	for( i=(n-1); i>0; i--){				//if fail, return 0;
		if( *(a+i) == b)
			return i;
	}
	return i;
}

int main(int argc, char ** argv)
{
	int n, nbytes,  listenfd, connfd, caddrlen, out_fd;
	struct hostent *h;
	struct sockaddr_in saddr, caddr;
	char buf_line[MAXLINE], file_name[MAXLINE], buf_data[MAXDATA];
	int port=1234, status_code = 0, file_size=0, buff=0;
	char *envir_name;
	struct stat st;
	if( argv[1] != NULL){
		port = atoi(argv[1]);
	}	
	if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) <0 ){
		exit(1);
	}
	bzero((char *)&saddr, sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);
	saddr.sin_port = htons(port);
	if( bind(listenfd, (struct sockaddr *)&saddr, sizeof(saddr)) <0){
		exit(2);
	}
	if(listen(listenfd, 5) <0){
		exit(3);
	}
	while(1){
		caddrlen = sizeof(caddr);
		if((connfd = accept(listenfd, (struct sockaddr *)&caddr, &caddrlen)) <0){
			continue;
		}
		
		h= gethostbyaddr((const char *)&caddr.sin_addr.s_addr, sizeof(caddr.sin_addr.s_addr), AF_INET);
		arrclear(file_name);
		if( (envir_name = getenv("HOME_DIR")) != NULL ){  		//if there is envir_name
			mystrcpy(file_name, (envir_name) );
		}
		
		//get pathname and make file_name
		if( (n = read(connfd, buf_line, MAXLINE)) >0){
			*(buf_line+n-1) = NULL;
			mystrcat(file_name, buf_line);
		}
		//open file with file_name
		if( (out_fd = open(file_name, O_RDONLY) ) < 0){
			if(errno == EACCES){
				status_code = (ERR_NOT_ALLOWED);
			}
			if(errno == ENOENT){
				status_code = (ERR_NO_FILE);
			}
			file_size=0;
			buff = htonl(status_code);
			write(connfd, &buff, 4);	//send status_code and file_size
			buff = htonl(file_size);
			write(connfd, &buff, 4);
		}
		//check file_size and make it to network version 
		else{
			stat(file_name, &st);
			file_size = st.st_size;
			buff = htonl(status_code);
			write(connfd, &buff, 4);	//send status_code and file_size
			buff = htonl(file_size);
			write(connfd, &buff, 4);
			while( (nbytes = read(out_fd, buf_data, MAXDATA)) >0){
				write(connfd, buf_data, nbytes);
			}
		}
		close(out_fd);
		close(connfd);
		status_code = 0, file_size=0, buff=0;
	}
	
	return 0;			
}
