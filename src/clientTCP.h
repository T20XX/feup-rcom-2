#pragma once

int getIPbyname(char * hostname, char * ip);
int connect_to_host(char * ip, int port);
int read_from_host(int connection_fd, char * msg, char * expected) ;
int write_to_host(int connection_fd, char * msg) ;
int send_logIn(int fd, char * user, char * pass);
int get_pasv(int fd, char * ip, int * port);
int send_path(int fd, char * path);
int download_to_file(int fd, char * filename);
