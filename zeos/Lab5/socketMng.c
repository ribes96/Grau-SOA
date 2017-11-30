#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

//custom 30/11/2017
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <unistd.h>


// Create a socket and initialize it to be able to accept 
// connections.
// It returns the virtual device associated to the socket that should be used 
// in the accept system call, to get the virtual device associated to 
// the connection
//




int
createServerSocket (int port)
{
    
    //custom 30/11/2017
//     int FAMILY = AF_INET;
//     int CONNECT_TYPE = SOCK_STREAM;
    
    int error;
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        perror ("Error creating socket\n");
        exit (1);
    }
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(port);
    saddr.sin_addr.s_addr = INADDR_ANY;
    memset(&saddr.sin_zero, 0, 8);

//     error = bind(fd, &saddr, 14);
    error = bind(fd, (struct sockaddr *)&saddr, sizeof(saddr));
    if (error < 0) {
        perror("Error binding\n");
        exit(1);
    }
    //acceptem 10 peticions simultanies
    error = listen(fd, 10);
    if (error < 0) {
        perror("Error listening\n");
        exit(1);
    }
    return fd;
}


// Returns the file descriptor associated to the connection.
// accept system call will fill the socketAddr parameter
// with the address of the socket for the client which is requesting the
// connection, and the addrSize parameter with the size of that address.

//custom 30/11/2017
int
acceptNewConnections (int socket_fd)
{
//     struct sockaddr saddr;
    int client_fd = accept(socket_fd,0, 0);
    return client_fd;
}

// Returns the socket virtual device that the client should use to access 
// the socket, if the connection is successfully established 
// and -1 in case of error
//
// Connect system call requires the address of the 
// server socket to request the connection and the size of that address.
//

int
clientConnection (char *host_name, int port)
{

  struct sockaddr_in serv_addr;
  struct hostent * hent;
  int socket_fd;
  int ret;

  //creates the virtual device for accessing the socket
  socket_fd = socket (AF_INET, SOCK_STREAM, 0);
  if (socket_fd < 0)
    return socket_fd;

  memset((char *) &serv_addr, 0, sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;
  hent = gethostbyname(host_name);
  if (hent == NULL) {
        close (socket_fd);
	return -1;

  }
  memcpy((char *)&serv_addr.sin_addr.s_addr, (char *) hent->h_addr, hent->h_length);
  serv_addr.sin_port = htons(port);
  serv_addr.sin_family = PF_INET; 

  ret = connect (socket_fd, (struct sockaddr *) &serv_addr, sizeof (serv_addr));
  if (ret < 0)
  {
	  close (socket_fd);
	  return (ret);
  } 

  return socket_fd;

}


int deleteSocket (int socket_fd)
{
  close (socket_fd);
}

