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

#define BUFF_SIZE 1024

void menu();
int connectToHost(char *hostname, char *port);
void send_or_exit(int fd, char *buff, size_t buff_len);
void recv_or_exit(int fd, char *buff, size_t max_len);

int main() {
    printf("%s\n", "WELCOME TO COMP 375 SENSOR NETWORK :)");
	printf("\n\n");
    int fd; // = connectToHost("comp375.sandiego.edu", "44144");
	
    char buff[BUFF_SIZE];
    memset(buff,0,BUFF_SIZE);
  
/*

    send_or_exit(fd,"  ",4);

    recv_or_exit(fd,buff,BUFF_SIZE);

  //  char tmp_buff1[BUFF_SIZE];
  //  sscanf(buff,"ONLINE - %d",(int*)BUFF_SIZE);

  //  printf("The server says: %s\n", tmp_buff1);
  //  memset(buff,0,BUFF_SIZE);

*/


    while(1) {
		menu();	
		printf("%s", "Selection: ");
		int selection = 0;
		scanf("%d", &selection);
		switch(selection) {
			case 1:
				// printf("\n%s\n\n", "Temp is ....");
                
                fd = connectToHost("comp375.sandiego.edu", "58983");
                send_or_exit(fd,"AUTH password123",4);
                recv_or_exit(fd,buff,BUFF_SIZE);

                printf("%s\n", buff);
              //  char tmp_buff1[BUFF_SIZE];
              //  sscanf(buff,"ONLINE - %d",(int*)BUFF_SIZE);

              //  printf("The server says: %s\n", tmp_buff1);
                memset(buff,0,BUFF_SIZE);

                send_or_exit(fd,"AIR TEMPERATURE",4);
				recv_or_exit(fd,buff,BUFF_SIZE);

                 memset(buff,0,BUFF_SIZE);
                 

                 
                 close(fd);

                break;

			case 2:
				printf("\n%s\n\n", "Wind is ....");
			
                send_or_exit(fd,"RELATIVE HUMIDITY",4);

                recv_or_exit(fd,buff,BUFF_SIZE);

                char tmp_buff2[BUFF_SIZE];
                sscanf(buff,"ONLINE - %d",(int*)BUFF_SIZE);

                printf("The server says: %s\n", tmp_buff2);
                memset(buff,0,BUFF_SIZE);

                break;

			case 3:
				printf("\n%s\n\n","Something is....");
				
                send_or_exit(fd,"WIND SPEED",4);

                recv_or_exit(fd,buff,BUFF_SIZE);

                char tmp_buff3[BUFF_SIZE];
                sscanf(buff,"ONLINE - %d",(int*)BUFF_SIZE);

                printf("The server says: %s\n", tmp_buff3);
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
	return 0;
}

void menu() {
	printf("%s\n\n", "Which sensor would you like to access: ");
	printf("%s\n", "\t(1)	Temperature.");
	printf("%s\n", "\t(2)	Relative Humidity.");
	printf("%s\n", "\t(3)	Wind Speed.");
	printf("%s\n\n", "\t(4)	Quit Program.");
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

