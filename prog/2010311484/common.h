//-----------------------------------------------------------
// 
// SSE2030: Introduction to Computer Systems (Fall 2013)
//
// Skeleton code for PA#3
//
// October 6, 2013.
// Jinyong Ha
// Sungkyunkwan University
//
//-----------------------------------------------------------


#include <unistd.h>

#define ERR_NO_FILE		(-1)
#define ERR_NOT_ALLOWED  	(-2)
#define ERR_INVALID_URL   	(-3)
#define ERR_CONNECTION_FAIL	(-4)
#define ERR_INVALID_DOMAIN 	(-5)

static void print_error_code(int error_code)
{
	char * msg;
	int str_cnt;
	
	switch(error_code)
	{
		case ERR_NO_FILE:
			msg = "FILE NOT FOUND\n";
			str_cnt = 15;
			break;

		case ERR_NOT_ALLOWED:
			msg = "NOT ALLOWED\n";
			str_cnt = 12;
			break;

		case ERR_INVALID_URL:
			msg = "INVALID URL\n";
			str_cnt = 12;
			break;

		case ERR_INVALID_DOMAIN:
			msg = "INVALID DOMAIN\n";
			str_cnt = 15;
			break;

		case ERR_CONNECTION_FAIL:
			msg = "CONNECTION FAIL\n";
			str_cnt = 16;
			break;

		default:
			msg = "WRONG ERROR CODE\n";
			str_cnt = 17;
			break;
	}
	
	write(1, msg, str_cnt);
}

static void print_msg(const char *file_name, int file_size)
{
	int i = 0;
	char buffer[16];

	write(1, "OK:", 3);

	while(file_name[i++]);

	write(1, file_name, i-1);
	write(1, ":", 1);

	i = 1;
	while (file_size) {
		buffer[i++] = file_size % 10;
		file_size /= 10;
	}
	while (--i) {
		buffer[i] += '0';
		write(1, buffer+i, 1);
	}
	write(1, "\n", 1);
}


