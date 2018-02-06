/*
 * James Ponwith
 * Project 01
 * Network Application
 * Using Wireshark, this program was
 * reverse engineered.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>


void menu();

int main() {
    printf("%s\n", "WELCOME TO COMP 375 SENSOR NETWORK :)");
	printf("\n\n");
    //int fd = connectToHost("comp375.sandiego.edu", "44144");
	while(1) {
		menu();	
		printf("%s", "Selection: ");
		int selection = 0;
		scanf("%d", &selection);
		switch(selection) {
			case 1:
				printf("\n%s\n\n", "Temp is ....");
				break;
			case 2:
				printf("\n%s\n\n", "Wind is ....");
				break;
			case 3:
				printf("\n%s\n\n","Something is....");
				break;
			case 4:
				printf("\n%s\n\n","GOODBYE!!");
			    // close(fd);
                exit(0);
                break;
            default:
				printf("\n\n%s\n\n","***INVALID OPTION");
		}
	}
	return 0;
}

void menu() {
	printf("%s\n\n", "Which sensor would you like to access: ");
	printf("%s\n", "\t(1)	Temperature.");
	printf("%s\n", "\t(2)	Relative Humidity.");
	printf("%s\n", "\t(3)	Wind Speed.");
	printf("%s\n\n", "\t(4)	Quit Program.");
}

int connectToHost(char *hostname, char *port) {
    int status;
    struct addrinfo hints;
    struct addrinfo *servinfo;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if((status = getaddrinfo(hostname, port, &hints, &servinfo))!= 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        exit(1);
    }

    int fd = socket(servinfo -> ai_family, servinfo -> ai_socktype, servinfo ->
                ai_protocol);
    
    if(fd == -1) {
        perror("socket");
        exit(1);
    }

    if(connect(fd, servinfo -> ai_addr, servinfo -> ai_addrlen) != 0) {
        perror("connect");
        exit(1);
    }
    return fd;
}
