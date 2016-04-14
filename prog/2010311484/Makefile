#-----------------------------------------------------------
# 
# SSE2030: Introduction to Computer Systems (Fall 2013)
#
# Skeleton code for PA#3
#
# October 6, 2013.
# Jinyong Ha
# Sungkyunkwan University
#
#-----------------------------------------------------------


CC		= gcc
CFLAGS		= -g -Wall
LDFLAGS		=
RM		= rm
TAR		= tar


SVR_OBJ = server.o
CLT_OBJ = client.o

CSRCS		= server.c client.c
SUBCSRCS	= Makefile common.h
TARGET		= server
TARGETS		= server client
OBJECTS		= $(CSRCS:.c=.o)

ifeq (tar,$(firstword $(MAKECMDGOALS)))
  TAR_ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
  $(eval $(TAR_ARGS):;@:)
endif

all:	
	$(CC) $(CFLAGS)  -o server server.c
	$(CC) $(CFLAGS)  -o client client.c

tar:
	$(TAR) zcvf $(TAR_ARGS).tar.gz $(CSRCS) $(SUBCSRCS)
clean:
	$(RM) -f $(SVR_OBJ) $(CLT_OBJ) $(TARGETS) *~

