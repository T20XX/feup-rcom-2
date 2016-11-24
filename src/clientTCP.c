/*      (C)2000 FEUP  */

#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "clientTCP.h"

#define SERVER_PORT 6000
#define SERVER_ADDR "192.168.28.96"

int read_pasv_from_host(int connection_fd, char * ip, int * port);

int connect_to_host(char * ip,int port){

	int	sockfd;
	struct	sockaddr_in server_addr;
	//char	buf[] = "user anonymous\n";

	/*server address handling*/
	bzero((char*)&server_addr,sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(ip);	/*32 bit Internet address network byte ordered*/
	server_addr.sin_port = htons(port);		/*server TCP port must be network byte ordered */

	/*open an TCP socket*/
	if ((sockfd = socket(AF_INET,SOCK_STREAM,0)) < 0) {
    		perror("socket()");
        	exit(0);
    	}
			printf("Opened\n");
	/*connect to the server*/
    	if(connect(sockfd,
	           (struct sockaddr *)&server_addr,
		   sizeof(server_addr)) < 0){
        	perror("connect()");
		exit(0);
	}
	printf("Connected\n");
	if (port == 21) {
	char msg[100];
		read_from_host(sockfd, msg, "220");
		printf("%s\n", msg);
	}


	return sockfd;
	//close(sockfd);
	//exit(0);
}

int getIPbyname(char * hostname, char * ip){
	struct hostent * h;

        if ((h=gethostbyname(hostname)) == NULL) {
            herror("gethostbyname");
            exit(1);
        }

				strcpy(ip, inet_ntoa(*((struct in_addr *)h->h_addr)));

        return 0;
}

int send_logIn(int fd, char * user, char * pass){
	char msg[100];
	//char	userMsg[] = "user anonymous\n";
	char * userMsg = malloc(6+strlen(user));
	sprintf(userMsg,"user %s\n",user);
	printf("%s",userMsg);
	write_to_host(fd,userMsg);
	printf("ja pedi user\n");
	read_from_host(fd, msg, "331");
	printf("%s\n", msg);


	char * passMsg = malloc(6+strlen(pass));
	sprintf(passMsg,"pass %s\n",pass);
	printf("%s",passMsg);
	write_to_host(fd,passMsg);
	printf("ja pedi pass\n");
	read_from_host(fd, msg, "230");

	printf("%s\n", msg);

	return 0;
}

int send_path(int fd, char * path){
	printf("we in");
	char msg[100];
	//char	pathMsg[] = "retr /pub/robots.txt\n";
	char * pathMsg = malloc(6+strlen(path));
	sprintf(pathMsg,"retr %s\n",path);
	printf("%s",pathMsg);
	write_to_host(fd,pathMsg);
	printf("ja pedi path\n");
	read_from_host(fd, msg, "150");
	printf("%s\n", msg);

	return 0;
}


int get_pasv(int fd, char * ip, int * port){
	char msg[100];

	write_to_host(fd,"pasv\n");
	printf("ja pedi pasv");
	read_pasv_from_host(fd, ip, port);

	printf("%s:%d\n",ip,*port);
	return 0;
}


int write_to_host(int connection_fd, char * msg) {
		int	bytes;
	/*send a string to the server*/
	bytes = write(connection_fd, msg, strlen(msg));
	if(bytes == strlen(msg)) return 0;
	else return bytes;
}


int read_from_host(int connection_fd, char * msg, char * expected) {

	FILE* fp = fdopen(connection_fd, "r");
	int size = 4;

	do {
		memset(msg, 0, size);
		msg = fgets(msg, size, fp);

	} while (!('1' <= msg[0] && msg[0] <= '5'));

	if(strcmp(msg,expected) == 0)
		return 0;
	else return -1;
}

int read_pasv_from_host(int connection_fd, char * ip, int * port) {

	FILE* fp = fdopen(connection_fd, "r");
	int size = 512;
	char * msg = malloc(size * sizeof(char));

	do {
		memset(msg, 0, size);
		msg = fgets(msg, size, fp);

	} while (!('1' <= msg[0] && msg[0] <= '5'));

	int ipTmp[4];
	int port_arr[2];

		if ((sscanf(msg, "227 Entering Passive Mode (%d,%d,%d,%d,%d,%d)",
			&ipTmp[0], &ipTmp[1], &ipTmp[2], &ipTmp[3], &port_arr[0], &port_arr[1])) < 0)
			return -1;

		if (sprintf(ip, "%d.%d.%d.%d", ipTmp[0], ipTmp[1], ipTmp[2], ipTmp[3]) < 0)
			return -1;

	*port = 256 * port_arr[0] + port_arr[1];

	free(msg);
		return 0;
}

int download_to_file(int fd, char * filename){
	FILE* file;
	int bytes;

	if (!(file = fopen(filename, "w"))) {
	printf("ERROR: Cannot open file.\n");
	return -1;
}

char*buf = malloc(1024);
while ((bytes = read(fd, buf, sizeof(buf)))) {
	if (bytes < 0) {
		printf("ERROR: Nothing was received from data socket fd.\n");
		return -1;
	}

	if ((bytes = fwrite(buf, bytes, 1, file)) < 0) {
		printf("ERROR: Cannot write data in file.\n");
		return -1;
	}
}


if(file)
	fclose(file);

return 0;
}
