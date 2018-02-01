CC = gcc
CFLAGS = -Wall -Wextra -Werror -g

TARGET = client

all: $(TARGET)

client: client.c
	    $(CC) $(CFLAGS) -o $@ $^

clean:
	    $(RM) $(TARGET)
