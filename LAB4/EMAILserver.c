#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <dirent.h>

#define BUFSIZE 1024
#define ARGSIZE 40
char buf2[BUFSIZE], buf3[BUFSIZE];
char arg1[ARGSIZE], arg2[ARGSIZE];
int fnlist;
/*all functions*/
void server_interface(/*parameters*/){
  /*function defination*/
  bzero(arg1,ARGSIZE);
  bzero(arg2,ARGSIZE);
  printf("server interface, read string : %s\n", buf2);
  char *cmd = strtok(buf2," ");
  int i = 0;
  while (cmd != NULL){
      if(i == 0)
          strcpy(arg1,cmd);
      else if(i == 1)
          strcpy(arg2,cmd);
      cmd = strtok(NULL, " ");
      i++;
  }

  printf("arg1 : %s, arg2 : %s\n", arg1,arg2);

  if(!strcmp(arg1,"LSTU"))
      fnlist = 1;
  else if(!strcmp(arg1,"ADDU"))
      {   if(arg2 == NULL);
              /*continue;*/
          else
              fnlist = 2;
      }
  else if(!strcmp(arg1,"USER"))
      {   if(arg2 == NULL);
              /*continue;*/
          else
              fnlist = 3;
      }
  else if(!strcmp(arg1,"READM"))
      fnlist = 4;
  else if(!strcmp(arg1,"DELM"))
      fnlist = 5;
  else if(!strcmp(arg1,"SEND"))
      fnlist = 6;
  else if(!strcmp(arg1,"DONEU"))
      fnlist = 7;
  else if(!strcmp(arg1,"QUIT"))
      fnlist = 8;
  else fnlist = 0;
  /*printf("fnlist : %d\n", fnlist);*/
  /*bzero(buf,BUFSIZE);*/
  switch(fnlist){
      case 1  :   LSTU();break;
      case 2  :   ADDU();break;
      case 3  :   USER();break;
      case 4  :   READM();break;
      case 5  :   DELM();break;
      case 6  :   SEND();break;
      case 7  :   DONEU();break;
      case 8  :   QUIT();break;
      default :   break;
  }

}

void LSTU(/*parameters*/){
  /*function defination*/
  printf("LSTU is called.\n");
  DIR *d;
  struct dirent *dir;
  d = opendir("users");
  if (d)
    {   bzero(buf3,BUFSIZE);
        while ((dir = readdir(d)) != NULL)
        {  if ( !(!(strcmp(dir->d_name, ".")) || !(strcmp(dir->d_name,".."))))
            {
              /*printf("%s\n", dir->d_name);
              printf("filename length %d \n", strlen(dir->d_name));*/
              strcat(buf3, dir->d_name);
              strcat(buf3, " ");}
        }
        closedir(d);
    }
  printf("\tbuf3 : %s\n", buf3);
}

void ADDU(/*parameters*/){
  /*function defination*/
  printf("ADDU is called.\n");
  printf("arg2 : %s , length : %d\n", arg2, strlen(arg2));
  FILE *fp, *fp2;
  char fname[50];
  bzero(fname, 50);
  strcpy(fname, "users/");
  strcat(fname, arg2);
  printf("fname : %s\n", fname);
  fp = fopen(fname, "r");
  if (fp == NULL)
  { 
    fp2 = fopen(fname, "wr");
    if (fp2 == NULL)
      printf("Error file creating\n");
    else
    { 
      bzero(buf3, BUFSIZE);
      strcpy(buf3, "User has been added successfully.");
      printf("%s\n", buf3);
    }
  }
  else {
    printf("User already exist.\n");
    bzero(buf3, BUFSIZE);
    strcpy(buf3, "User already exist.");
    fclose(fp);
  }
}

void USER(/*parameters*/){
  /*function defination*/
  printf("USER is called.\n");
}

void READM(/*parameters*/){
  /*function defination*/
  printf("READM is called.\n");
}

void DELM(/*parameters*/){
  /*function defination*/
  printf("DELM is called.\n");
}

void SEND(/*parameters*/){
  /*function defination*/
  printf("SEND is called.\n");
}

void DONEU(/*parameters*/){
  /*function defination*/
  printf("DONEU is called.\n");
}

void QUIT(/*parameters*/){
  /*function defination*/
  printf("QUIT is called.\n");
}

void error(char *msg) {
  perror(msg);
  exit(1);
}

int main(int argc, char **argv) {
  int parentfd; /* parent socket */
  int childfd; /* child socket */
  int portno; /* port to listen on */
  int clientlen; /* byte size of client's address */
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
    
    /* 
     * read: read input string from the client
     */
    char rep='y';
    while(rep=='y'){
      bzero(buf, BUFSIZE);
      n = read(childfd, buf, BUFSIZE);
      if (n < 0) 
        error("ERROR reading from socket");
      if(!strcmp(buf,"QUIT") || n == 0){
        rep='n';
      }
      else 
      {
        bzero(buf2, BUFSIZE);
        strcpy(buf2,buf);
        server_interface();
        if(strlen(buf3) > 0) {
          bzero(buf, BUFSIZE);
          strcpy(buf,buf3);
        }
      }
      printf("server received %d bytes: %s\n", n, buf);
      /* 
       * write: echo the input string back to the client 
       */
      n = write(childfd, buf, strlen(buf));
      if (n < 0) 
        error("ERROR writing to socket");
      bzero(buf3, BUFSIZE);
    }
    close(childfd);
  }
}