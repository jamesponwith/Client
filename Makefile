CC = gcc
CFLAGS = -Wall -Wextra -Werror -g

TARGET = client

all: $(TARGET)

client: client.c parse_args.c parse_args.h
	    $(CC) $(CFLAGS) -o client client.c parse_args.c -lm 

clean:
	    $(RM) $(TARGET)
