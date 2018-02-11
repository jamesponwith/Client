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
void sensorInfo(int fd, char *selection);

int main() {
    printf("%s\n", "WELCOME TO COMP 375 SENSOR NETWORK :)");
    printf("\n\n");
    int fd; 
    fd = connectToHost("comp375.sandiego.edu", "47789");
    mainLoop(fd);
    close(fd);
    return 0;
}  

void mainLoop(int fd) {
    while(1) {
        long selection = prompt();
        char *selec;
        switch(selection) {
            case 1:
                selec = "AIR TEMPERATURE";
                sensorInfo(fd, selec);
                close(fd);
                fd = connectToHost("comp375.sandiego.edu", "47789");
                break;
            case 2:
                selec = "RELATIVE HUMIDITY";
                sensorInfo(fd, selec);
                close(fd);
                fd = connectToHost("comp375.sandiego.edu", "47789");
                break;
            case 3:
                selec = "WIND SPEED";
                sensorInfo(fd, selec);
                close(fd);
                fd = connectToHost("comp375.sandiego.edu", "47789"); 
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
    freeaddrinfo(servinfo);
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

/**
 * Handles selection option to communicate with the sensor server
 * to retrieve desired information
 *
 * @param fd The socket file descriptor to communicate with the server
 * @param *selection Depending on the menu option the necessary server message will be passed in the switch menu (ex. "AIR TEMPERATURE")
 */
void sensorInfo(int fd, char *selection) {

    char buff[BUFF_SIZE];
    char *ret_buff[BUFF_SIZE];

    send_or_exit(fd,"AUTH password123\n",17);
    recv_or_exit(fd,buff,BUFF_SIZE);

    parseArguments(buff, ret_buff); // Parse server response

    memset(buff,0,BUFF_SIZE); // Reset buffer for next message
    
    fd = connectToHost(ret_buff[1], ret_buff[2]); // Connect to sensor server and sensor port
    
    send_or_exit(fd,"AUTH sensorpass321\n",21); // Send password to sensor
    recv_or_exit(fd,buff,BUFF_SIZE); // Recieve server message

    memset(buff,0,BUFF_SIZE); // Reset buffer for next message

    send_or_exit(fd, selection, 17); // Request specific sensor information with selection 
    recv_or_exit(fd,buff,BUFF_SIZE); // Recieve data

    memset(ret_buff,0,BUFF_SIZE); // Reset buffer for next message
    
    parseArguments(buff,ret_buff); // Parse the data for display

    time_t time; // Create time object to convert EPOCH time to readable time 
    time = strtoul(ret_buff[0],NULL,0); // Convert String to Int

    // Print and format data
    printf("\n%s%s%s","The last ",selection," reading was ");
    printf("%s%s%s",ret_buff[1],ret_buff[2],", taken at ");
    printf("%s\n",ctime(&time)); 

    memset(buff,0,BUFF_SIZE); // Reset buffer

    send_or_exit(fd,"CLOSE\n",BUFF_SIZE); // Send CLOSE message
    recv_or_exit(fd,buff,BUFF_SIZE);

    memset(buff,0,BUFF_SIZE); // Reset buffer
    memset(ret_buff,0,BUFF_SIZE);
}
