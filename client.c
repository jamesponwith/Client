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
				exit(0);
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

