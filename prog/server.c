//-----------------------------------------------------------
// 2010311484
// KWON YOUNGDAE
// SSE2030: Introduction to Computer Systems (Fall 2013)
//
// Skeleton code for PA#4
//
// October 18, 2013.
// Author : Dong Hyun, Song (songdh418@gmail.com)
// Sungkyunkwan University
//
//-----------------------------------------------------------

#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

#include <sys/stat.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

//#include "http.h"

#define MAXLINE (80)
#define MAXDATA 2048

int main(int argc, char ** argv)
{
	struct hostent;
	struct sockaddr_in saddr, caddr;
 
	int len = -1;
	int send = 0;
	int listenfd, connfd, caddrlen, n;
	int port = 1234;
	char default_msg[] = "loading fail";
	char buffer[BUF_SIZE] = {0,};
	char* http_header; 
	char* home_dir = getenv("HOME_DIR");
	//my new variable
	pid_t pid;
	int point_url1=0, point_url2=0, pathname_len=0, pathname_type_point1=0,pathname_type_point2=0;
	int status_code=0,content_type=0, content_length=0, type_check=0;
	int out_fd;
	struct stat st;
	char input_url[MAXLINE], pathname[MAXLINE];
	char pathname_type[5], default_path[]="/", default_pathname[]="index.html";
	signal (SIGCHLD, handler2);
	
	if (!home_dir)
	{
		home_dir = getenv("PWD");
	}

	if (argc == 2 && argv[1] != NULL)
	{
		port = atoi(argv[1]);
	}

	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		write(1, "socket fail\n", 12);
		return 1;
	}

	bzero((char*)&saddr, sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);
	saddr.sin_port = htons(port);

	if (bind(listenfd, (struct sockaddr*)&saddr, sizeof(saddr)) < 0)
	{
		write(1, "bind fail\n", 10);
		return 2;
	}
	
	if (listen(listenfd, 5) < 0)
	{
		write(1, "listen fail\n", 12);
		return 3;
	}
	while(1){
		caddrlen = sizeof(caddr);
		if ((connfd = accept(listenfd, (struct sockaddr*)&caddr, (socklen_t *)&caddrlen)) < 0)
		{
			write(1, "accept fail\n", 12);
			return 4;
		}
		if( (pid=fork() ) == 0){		//child process
			close(listenfd);	
			// do nothing:
			// ignore client's request
			// read client request into buffer until there is '\r\n\r\n'
			
			
			close(connfd);
			exit(0);
		}
		//parent process
		close(connfd);		//disconnect
	}
	return 0;
}
