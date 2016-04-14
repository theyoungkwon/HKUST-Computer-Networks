binary_file: udp_server udp_client

OBJ_FILEUC= udp_client.o dg_cli.o my_err.o

OBJ_FILEUS= udp_server.o dg_echo.o my_err.o

udp_client: $(OBJ_FILEUC) $(OBJ_FILEUS) makefile
	cc -o $@ $(OBJ_FILEUC) -lsocket -lnsl -lm 

udp_server: $(OBJ_FILEUC) $(OBJ_FILEUS) makefile
	cc -o $@ $(OBJ_FILEUS) -lsocket -lnsl -lm 

clean:
	rm -f udp_client udp_server *.o

udp_client.o: udp_client.c dg_cli.o inet.h my_err.o
udp_server.o: udp_client.c dg_echo.o inet.h my_err.o
dg_cli.o: dg_cli.c my_err.o
dg_echo.o: dg_echo.c my_err.o
my_err.o: my_err.c




