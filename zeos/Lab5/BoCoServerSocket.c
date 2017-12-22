#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
// #include "socketMng.h"
#include <sys/wait.h>

const int MAX_CONS = 10;
int current_cons = 0;

//Declarations
int
createServerSocket (int port);

int
acceptNewConnections (int socket_fd);

int
clientConnection (char *host_name, int port);

int deleteSocket (int socket_fd);


void doService(int fd) {
    int i = 0;
    char buff[80];
    char buff2[80];
    int ret;
    int socket_fd = (int) fd;

	ret = read(socket_fd, buff, sizeof(buff));
	while(ret > 0) {
		buff[ret]='\0';
		sprintf(buff2, "Server [%d] received: %s\n", getpid(), buff);
		write(1, buff2, strlen(buff2));
		ret = write(fd, "caracola ", 8);
		if (ret < 0) {
			perror ("Error writing to socket");
			exit(1);
		}
		ret = read(socket_fd, buff, sizeof(buff));
	}
	if (ret < 0) {
			perror ("Error reading from socket");

	}
	sprintf(buff2, "Server [%d] ends service\n", getpid());
	write(1, buff2, strlen(buff2));

}

void doServiceFork(int fd) {
    if (current_cons >= MAX_CONS) {
        int st;
        wait(&st);
        --current_cons;
    }
    int pid = fork();
    if (pid == 0) {
        //FILL
        doService(fd);
        exit(0);
    }
    else if (pid > 0) {
        //PARE
        ++current_cons;
    }
    else if (pid < 0) {
        perror ("Unable to fork");
    }
}


int main (int argc, char *argv[])
{
  int socketFD;
  int connectionFD;
  char buffer[80];
  int ret;
  int port;


  if (argc != 2)
    {
      strcpy (buffer, "Usage: ServerSocket PortNumber\n");
      write (2, buffer, strlen (buffer));
      exit (1);
    }

  port = atoi(argv[1]);
  socketFD = createServerSocket (port);
  if (socketFD < 0)
    {
      perror ("Error creating socket\n");
      exit (1);
    }

  while (1) {
	  connectionFD = acceptNewConnections (socketFD);
	  if (connectionFD < 0)
	  {
		  perror ("Error establishing connection \n");
		  deleteSocket(socketFD);
		  exit (1);
	  }

	  doServiceFork(connectionFD);
  }

}