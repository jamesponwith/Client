/*
 * James Ponwith
 * Project 01
 * Network Application
 * Using Wireshark, this program was
 * reverse engineered.
 *
 */

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
#include "parse_args.h"

#define _XOPEN_SOURCE 600

#define BUFF_SIZE 1024

void menu();
long prompt();
int connectToHost(char *hostname, char *port);
void mainLoop(int fd);
void send_or_exit(int fd, char *buff, size_t buff_len);
void recv_or_exit(int fd, char *buff, size_t max_len);

int main() {
    printf("%s\n", "WELCOME TO COMP 375 SENSOR NETWORK :)");
	printf("\n\n");
    int fd; // = connectToHost("comp375.sandiego.edu", "44144");
    fd = connectToHost("comp375.sandiego.edu", "47789");
    mainLoop(fd);
    char buff[BUFF_SIZE];
    memset(buff,0,BUFF_SIZE);
    close(fd);
    return 0;
}  
/*

    send_or_exit(fd,"  ",4);

    recv_or_exit(fd,buff,BUFF_SIZE);

  //  char tmp_buff1[BUFF_SIZE];
  //  sscanf(buff,"ONLINE - %d",(int*)BUFF_SIZE);

  //  printf("The server says: %s\n", tmp_buff1);
  //  memset(buff,0,BUFF_SIZE);

*/

void mainLoop(int fd) {
    while(1) {
		long selection = prompt();
        char buff[BUFF_SIZE];
        memset(buff,0,BUFF_SIZE);
        char *ret_buff[BUFF_SIZE];
        memset(ret_buff,0,BUFF_SIZE);
        switch(selection) {
			case 1:
				// printf("\n%s\n\n", "Temp is ....");
                
                send_or_exit(fd,"AUTH password123\n",17);
                recv_or_exit(fd,buff,BUFF_SIZE);

                printf("%s\n", buff);
                parseArguments(buff, ret_buff);
                
                            
                unsigned int j = 0;
                while(ret_buff[j] != NULL) {
                    printf("%s\n", ret_buff[j]);
                    j++;
                }

                memset(buff,0,BUFF_SIZE);

                fd = connectToHost(ret_buff[1], ret_buff[2]);

                send_or_exit(fd,"AUTH sensorpass321\n",21);
				recv_or_exit(fd,buff,BUFF_SIZE);
                
                printf("%s\n", buff);

                memset(buff,0,BUFF_SIZE);

                //fd = connectToHost(ret_buff[1], ret_buff[2]);

                send_or_exit(fd, "AIR TEMPERATURE\n", 17);
                recv_or_exit(fd,buff,BUFF_SIZE);
               
                printf("%s\n",buff);
                
                memset(ret_buff,0,BUFF_SIZE);
                parseArguments(buff,ret_buff);

                printf("%s%s%s\n",ret_buff[0],ret_buff[1], ret_buff[2]);
                time_t time;
                time = strtoul(ret_buff[0],NULL,0);

                printf("%s","The AIR TEMPERATURE as of: ");
                printf("%s%s",ctime(&time)," is "); 
                printf("%s%s\n",ret_buff[1],ret_buff[2]);

                memset(buff,0,BUFF_SIZE);
                
                send_or_exit(fd,"CLOSE\n",BUFF_SIZE);
                recv_or_exit(fd,buff,BUFF_SIZE);

                printf("%s\n",buff);

                memset(buff,0,BUFF_SIZE);

                 close(fd);

                break;

			case 2:
				printf("\n%s\n\n", "Wind is ....");
			
                send_or_exit(fd,"RELATIVE HUMIDITY",4);

                recv_or_exit(fd,buff,BUFF_SIZE);

                memset(buff,0,BUFF_SIZE);

                break;

			case 3:
				printf("\n%s\n\n","Something is....");
				
                send_or_exit(fd,"WIND SPEED",4);

                recv_or_exit(fd,buff,BUFF_SIZE);

                memset(buff,0,BUFF_SIZE);
                
                break;
			
            case 4:
				printf("\n%s\n\n","GOODBYE!!");
			    close(fd);
                exit(0);
                break;
            
            default:
				printf("\n\n%s\n\n","***INVALID OPTION");
		}
	}
}

/**
 * Displays the menu
 */
void menu() {
	printf("%s\n\n", "Which sensor would you like to access: ");
	printf("%s\n", "\t(1)	Air Temperature.");
	printf("%s\n", "\t(2)	Relative Humidity.");
	printf("%s\n", "\t(3)	Wind Speed.");
	printf("%s\n\n", "\t(4)	Quit Program.");
}

/**
 * Print command prompt to user and obtain user input
 *
 * @return The user's desired selection, or -1 if invalid selection.
 */
long prompt() {
    // Print menu options
    menu();
    printf("%s","Selection: ");
    // Read in value from standard input
    char input[10];
    memset(input, 0, 10); // set all values of input[] to '\0'
    char *read_str = fgets(input, 10, stdin);
    if(read_str == NULL) {
        if (feof(stdin)) {
            exit(0);
        }
        else if(ferror(stdin)) {
            perror("fgets");
            exit(1);
        }
    }
    
    // get rid of newline, if there is one
    char *new_line = strchr(input, '\n');
    if(new_line != NULL) new_line[0] = '\0';
    
    // convert string to a long int
    char *end;
    long selection = strtol(input, &end, 10);

    if(end == input || *end != '\0') {
        selection = -1;
    }
    return selection;
}

/*
 * Uses Socket interface to connect to a specified host at a particular port.
 *
 * @param hostname The name of the host to connect to (e.g. "foo.sandiego.edu")
 * @param port The port number to connect to
 * @result File descriptor of the socket we are now using 
 */
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

void send_or_exit(int fd, char *buff, size_t buff_len) {
    int sent = send(fd, buff, buff_len, 0);
    if(sent == 0) {
        printf("Server connection closed unexpectedly. Good Bye. \n");
        exit(1);
    }
    else if(sent == -1) {
        perror("Send");
        exit(1);
    }
    
    // TODO: if sent < buff_len, do another send to finish sending data
    if(sent < (int) buff_len) {
        send(fd, buff, buff_len, 0);
    }

}

void recv_or_exit(int fd, char *buff, size_t max_len) {
    int recvd = recv(fd, buff, max_len,0);
    if(recvd == 0) {
        printf("Server connection closed unexpectedly. Good Bye. \n");
        exit(1);
    }
    else if(recvd == -1) {
        perror("Recv");
        exit(1);
    }
}


