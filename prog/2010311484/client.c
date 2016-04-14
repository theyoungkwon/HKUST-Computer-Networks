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
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>  

#include <stdlib.h>
#include <strings.h>

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
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////main////////////////////////////////
int main(int argc, char ** argv)
{
	int n, cfd, nbytes=0, out_fd;
	struct hostent *h;
	struct sockaddr_in saddr;
	char buf_line[MAXDATA], buf_data[MAXDATA], pathname[MAXLINE], output_path[MAXLINE];
	char input_url[MAXLINE];
	int port, input_urllen=0;
	char host[MAXLINE], port_arr[MAXLINE];
	struct in_addr addr;
	
	//check if url is appropriate
	char check_url[20] = "rfile://", end_url[20] = "QUIT\n";
	char check1[20] = "rfile://localhost";
	int check_urllen = mystrlen(check_url);
	int point_host=0, point_port=0, pathname_len, point_url1=0, point_url2=0, point_output=0;
	//from server data
	int status_code = 0, file_size=0, buff=0;
	 (nbytes = read(0, buf_line, MAXDATA));	//read URL from keyboard

	while(1){
		//separate url 
		if( (point_url2 = myfindchar(buf_line, point_url1, '\n')) ){
			mystrncpy(input_url, (buf_line+point_url1), (point_url2-point_url1+1));
			point_url1 = point_url2+1;
			if( (mystrncmp( input_url, end_url, 5 ) == 0) ){	//if QUIT\n 
				return 0;		//terminate process
			}
		}

		//URL CHECKING
		input_urllen = mystrlen(input_url);
		if( (mystrncmp( input_url, check_url, check_urllen)) == 0 ){ //if correct
			point_host = myfindchar(input_url, check_urllen, ':');
			point_port = myfindchar(input_url, check_urllen, '/');
			if( point_host>point_port){
				point_host = 0;
			}
			if( point_host == point_port){ //if case is rfile://
				point_port = 0;
			}
			if( (point_host < point_port)&&(point_host!=0) ){		//ex) localhost:1.2.3.4/result.txt
				mystrncpy(host, (input_url+check_urllen), (point_host-check_urllen) );
				mystrncpy(port_arr, (input_url+point_host+1), (point_port-point_host-1));
				port = atoi(port_arr);	
			}
			else if(point_port==0){	//if case is rfile://
				print_error_code(ERR_INVALID_URL);
				continue;
			}
			else{		//ex) sys.skku.edu/list.dat
				mystrncpy(host, (input_url+check_urllen), (point_port-check_urllen) );
				port = 1234;
			}
			pathname_len = (mystrlen(input_url) - point_port);
			mystrncpy(pathname, (input_url+point_port) , pathname_len );
		}
		else{
			print_error_code(ERR_INVALID_URL);
			continue;
		}
		//make socket
		if( (cfd = socket(AF_INET, SOCK_STREAM, 0) ) <0 ){
			print_error_code(ERR_CONNECTION_FAIL);	// return value is 1
			exit(1);
		}
		//get host from DNS server
		if( (h = gethostbyname(host)) == NULL) {	//if host is name form
			print_error_code(ERR_INVALID_DOMAIN);
			continue;
		}
		bzero((char *)&saddr, sizeof(saddr) );
		saddr.sin_family = AF_INET;
		bcopy((char *)h->h_addr, (char *)&saddr.sin_addr.s_addr, h->h_length);
		saddr.sin_port = htons(port); 
		//connect with Server 
		if(connect(cfd, (struct sockaddr *)&saddr, sizeof(saddr) ) <0){
			print_error_code(ERR_CONNECTION_FAIL);
			continue;
		}

		point_output = myfindchar2(pathname, pathname_len, '/');
		mystrcpy(output_path, (pathname+point_output+1) );	//final storage file name=output_path
		*(output_path+pathname_len-point_output-2) = NULL;
		write(cfd, pathname, pathname_len );
		if( (read(cfd, &buff, 4)) >0 ){
			status_code = ntohl(buff);
			if (status_code == -1){
				print_error_code(ERR_NO_FILE);
				read(cfd, &buff, 4);
				status_code =0, file_size = 0;
				close(cfd);	
				continue;
			}
			if (status_code == -2){
				print_error_code(ERR_NOT_ALLOWED);
				read(cfd, &buff, 4);
				status_code =0, file_size = 0;
				close(cfd);
				continue;
			}
			if (status_code == -4){
				print_error_code(ERR_CONNECTION_FAIL);
				read(cfd, &buff, 4);
				status_code =0, file_size = 0;
				close(cfd);
				continue;
			}
			read(cfd, &buff, 4);
			file_size = ntohl(buff);			
			if( (out_fd = open(output_path, O_WRONLY | O_CREAT, 0644) ) < 0){
				if(errno == EACCES){
					print_error_code(ERR_NOT_ALLOWED);
					close(cfd);
					continue;
				}
				if(errno == ENOENT){
					print_error_code(ERR_NO_FILE);
					close(cfd);
					continue;
				}
				close(cfd);
				continue;
			}
			while( (n = read(cfd, buf_data, MAXDATA)) >0 ){
				write(out_fd, buf_data, n);
			}
			print_msg(output_path, file_size);
		}
		close(out_fd);
		close(cfd);	
		
	}//end of while(1) 
	
	
	return 0;			
}
