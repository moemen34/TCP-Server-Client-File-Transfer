/*
 * tcp_server.c - A simple TCP echo server
 *
 * Compile: gcc tcp_server.c -o tcp_server
 *
 * usage: tcp_server <port>
 */


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFSIZE 1024


/*
 * error - wrapper for perror
 */
void error(char *msg) {
  perror(msg);
  exit(1);
}

int main(int argc, char **argv) {
  int parentfd; /* parent socket */
  int childfd; /* child socket */
  int portno; /* port to listen on */
  socklen_t clientlen; /* byte size of client's address */
  struct sockaddr_in serveraddr; /* server's addr */
  struct sockaddr_in clientaddr; /* client addr */
  struct hostent *hostp; /* client host info */
  char buf[BUFSIZE]; /* message buffer */
  char *hostaddrp; /* dotted decimal host addr string */
  int optval; /* flag value for setsockopt */
  int n; /* message byte size */

  /*
   * check command line arguments
   */
  if (argc != 2) {
    fprintf(stderr, "usage: %s <port>\n", argv[0]);
    exit(1);
  }
  portno = atoi(argv[1]);

  /*
   * socket: create the parent socket
   */
  parentfd = socket(AF_INET, SOCK_STREAM, 0);
  if (parentfd < 0)
    error("ERROR opening socket");

  /* setsockopt: Handy debugging trick that lets
   * us rerun the server immediately after we kill it;
   * otherwise we have to wait about 20 secs.
   * Eliminates "ERROR on binding: Address already in use" error.
   */
  optval = 1;
  setsockopt(parentfd, SOL_SOCKET, SO_REUSEADDR,
             (const void *)&optval , sizeof(int));

  /*
   * build the server's Internet address
   */
  bzero((char *) &serveraddr, sizeof(serveraddr));

  /* this is an Internet address */
  serveraddr.sin_family = AF_INET;

  /* let the system figure out our IP address */
  serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

  /* this is the port we will listen on */
  serveraddr.sin_port = htons((unsigned short)portno);

  /*
   * bind: associate the parent socket with a port
   */
  if (bind(parentfd, (struct sockaddr *) &serveraddr,
           sizeof(serveraddr)) < 0)
    error("ERROR on binding");
  /*
   * Needed for TCP
   * listen: make this socket ready to accept connection requests
   */
  if (listen(parentfd, 5) < 0) /* allow 5 requests to queue up */
    error("ERROR on listen");

  /*
   * main loop: wait for a connection request, echo input line,
   * then close connection.
   */
  clientlen = sizeof(clientaddr);
  while (1) {

    /*
     * accept: wait for a connection request
     */
    childfd = accept(parentfd, (struct sockaddr *) &clientaddr, &clientlen);
    if (childfd < 0)
      error("ERROR on accept");

    /*
     * gethostbyaddr: determine who sent the message
     */
    hostp = gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr,
                          sizeof(clientaddr.sin_addr.s_addr), AF_INET);
    if (hostp == NULL)
      error("ERROR on gethostbyaddr");
    hostaddrp = inet_ntoa(clientaddr.sin_addr);
    if (hostaddrp == NULL)
      error("ERROR on inet_ntoa\n");
    printf("server established connection with %s (%s)\n",
           hostp->h_name, hostaddrp);



    char buff[BUFSIZE];
    int n;
    bzero(buff, BUFSIZE);

    // read the message from client and copy it in buffer


    //printf("kkkkkkkkk\n");
    if(read(childfd, buff, sizeof(buff)) < 0){
      printf("didn't receive path");
      return 1;
    }
    //buff[strlen(buff)-1] = '\0';
    printf("From client: %s, %ld\n", buff, strlen(buff));

    //"/home/moemen/ICSI516/test.txt"

    n = 0;

    FILE *textfile;
    char ch;
    char bufff[BUFSIZE];

    textfile = fopen(buff, "r");
    bzero(buff, BUFSIZE);
    //check if the path received is valid
    //if not terminate
    if(textfile == NULL){
      //printf("PATH: %s", buff, strlen)
      write(childfd, "No such path", 12);
      return 1;
    }


    int k=0;
    int line = 0;
    while(1){
      int k=0;
      // loop over each line char by char until the
      //end of the line is reached
      while((ch = fgetc(textfile))!= '\n') {
        if(ch == EOF){
            break; //break once the end of the file is reached
        }
        bufff[k] = ch;
        k++;
      }
      bufff[k]='\n';
      bufff[k+1]='\0';
      k=0;

      //send the line to the client
      write(childfd, bufff, strlen(bufff));
      line++;

      //if 4 lines have been sent, wait for a confirmation
      //and print it
      if(line%4 == 0){
        read(childfd, buff, BUFSIZE);
        printf("From client: %s\n", buff);
        bzero(buff, BUFSIZE);
      }

      //cler bufff
      bzero(bufff, BUFSIZE);
      //break once the end of the file is reached
      if(ch == EOF){
        break;
      }
    }

    write(childfd, "#", 1);
    printf("\nend\n");

    fclose(textfile);

    close(childfd);
  }
  close(parentfd);
}
