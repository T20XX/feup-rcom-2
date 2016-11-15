#include <stdio.h>
#include <stdlib.h>

typedef struct  ftpConnection{
char * user;      /*user name*/
char * pass;      /*password*/
char * host;      /*server host*/
char * path;      /*file path*/
char * filename;  /*name of file*/
} ftpConnection;

int main(int argc, char *argv[])
{

        if (argc != 2) {
            fprintf(stderr,"usage: download URL\n");
            exit(1);
        }

        ftpConnection client;
        if (parseURL(argv[1], client) != 0){
          exit(2);
        }

        return 0;
}

int parseURL(char * url, ftpConnection connection){
  char buf[100];
  char * ptr = url;
  //ftpConnection connection;

  if (strncmp(url, "ftp://", 6) != 0){
    return -1;
  }
  printf("ok\n");
  return 0;
}
