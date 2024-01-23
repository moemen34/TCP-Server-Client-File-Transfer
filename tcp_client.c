/*
 * tcp_client.c - A simple TCP client
 *
 * Compile: gcc tcp_client.c -o tcp_client
 *
 * usage: tcp_client <host> <port>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>

#define BUFSIZE 1024

/*
 * error - wrapper for perror
 */
void error(char *msg) {
    perror(msg);
    exit(0);
}

int main(int argc, char **argv) {
    int sockfd, portno, n;
    struct sockaddr_in serveraddr;
    struct hostent *server;
    char *hostname;
    char buf[BUFSIZE];
    struct timespec start, end;
    
    /* check command line arguments */
    if (argc != 4) {
      fprintf(stderr,"usage: %s <hostname> <port> <file path>\n", argv[0]);
      exit(0);
    }
    hostname = argv[1];
    portno = atoi(argv[2]);
    char *path = argv[3];///////////////////////////////
    
    /* socket: create the socket */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
      error("ERROR opening socket");
    /* gethostbyname: get the server's DNS entry */
    server = gethostbyname(hostname);
    if (server == NULL) {
      fprintf(stderr,"ERROR, no such host as %s\n", hostname);
      exit(0);
    }
    
    /* build the server's Internet address */
    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
    (char *)&serveraddr.sin_addr.s_addr, server->h_length);
    serveraddr.sin_port = htons(portno);
    
    /* connect: create a connection with the server */
    if (connect(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) < 0)
        error("ERROR connecting");
    
    char buff[BUFSIZE];
        
    FILE *fp;
    fp = fopen("test2.txt","a");
    int lineCount = 0;
        
    bzero(buff, BUFSIZE);
    printf("PATH: %s, len: %ld\n", path, strlen(path));
    write(sockfd, path, strlen(path));
    int line = 0;
        
    while(1){
      bzero(buff, BUFSIZE);
      read(sockfd, buff, BUFSIZE);//read lines sent by server
       if ((strcmp(buff, "No such path")) == 0) {
        break;
      }
          //entire file was received
    if ((strcmp(buff, "#")) == 0) {
        printf("buff: %s\n", buff);
        printf("Entire file received, content was written to test2.txt\n");
        //break;
    }
    
    printf("From Server : %s", buff);//print the line received
    line++;
    
    int end = 0;
    for(int i = 0; i<strlen(buff); i++){
      if(buff[i] == '\n'){
        lineCount++;
        //printf("hello mf: %d\n", lineCount);
      }
      if(buff[i] == '#'){
        end++;
        buff[i] = '\0';
        break;
      }
    }
    
    fprintf(fp, "%s", buff);
    
    if(end>0){
      break;
    }
    
          //if 4 lines were received send a confirmation
    if(lineCount%4 == 0){
        char msg[BUFSIZE] = "Received messages up to #";
        char lines[6];
        sprintf(lines, "%d", lineCount);
        printf("line: %s, %ld\n", lines, strlen(lines));
        strcat(msg, lines);
        printf("msg: %s, %ld\n", msg, strlen(msg));
        write(sockfd, msg, strlen(msg));
    }
    if((strcmp(buff, "#")) == 0){
        break;
    }
        }
    //entire file was received
    if ((strncmp(buff, "#", 4)) == 0) {
      //break;
    }
        //path sent was invalid
    if ((strcmp(buff, "No such path")) == 0) {
      printf("No suck path, path passed as arg doesn't exist\n");
      fprintf(stderr,"usage: %s <hostname> <port> <file path>\n", argv[0]);
    }
        
    fclose(fp);
    close(sockfd);
    return 0;
}
