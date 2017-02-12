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
#include <time.h>

#define BUFSIZE 1024
#define ARGSIZE 40
#define EMAILNO 1000    /* no of email a user can have */
char subject[BUFSIZE]; message[BUFSIZE]; /* subject and message buffer */
char buf2[BUFSIZE], messagesend[BUFSIZE], email[EMAILNO][BUFSIZE]; /* 2nd buffer, message to send to client, store emails */
char arg1[ARGSIZE], arg2[ARGSIZE], username[ARGSIZE],sendtouser[ARGSIZE];   /* command and followed string, username and send user */
int fnlist, totalemail;   /* function list and total no. of message counter */
int count=0;    /* counter for messages to be updated while delete */
int readcount=0;  /* reading message counter */


/* all functions */
void server_interface(int childfd){
  /* make buffer zero for command and followed string */
  char *cmd = strtok(buf2," ");
  int i = 0;
  /* store command and followed string */
  while (cmd != NULL){
      if(i == 0)
      { bzero(arg1,ARGSIZE);  
        strcpy(arg1,cmd);
      }
      else if(i == 1)
      {
          bzero(arg2,ARGSIZE);
          strcpy(arg2,cmd);
      }
      cmd = strtok(NULL, " ");
      i++;
  }
  /* assign function list */
  if(!strcmp(arg1,"LSTU"))
      fnlist = 1;
  else if(!strcmp(arg1,"ADDU"))
      {   if(arg2 != NULL);
              fnlist = 2;
      }
  else if(!strcmp(arg1,"USER"))
      {   if(arg2 != NULL);
          {
              fnlist = 3;
              bzero(username,ARGSIZE);
              strcpy(username,arg2);
          }
      }
  else if(!strcmp(arg1,"READM"))
      fnlist = 4;
  else if(!strcmp(arg1,"DELM"))
      fnlist = 5;
  else if(!strcmp(arg1,"SEND"))
      {
        fnlist = 6;
        bzero(sendtouser,ARGSIZE);
        strcpy(sendtouser,arg2);
      }
  else if(!strcmp(arg1,"DONEU"))
      fnlist = 7;
  else if(!strcmp(arg1,"QUIT"))
      fnlist = 8;
  else fnlist = 0;
  switch(fnlist){
      case 1  :   LSTU();break;
      case 2  :   ADDU();break;
      case 3  :   USER();bzero(arg2,ARGSIZE);break;
      case 4  :   READM();break;
      case 5  :   DELM();break;
      case 6  :   SEND(childfd);break;
      case 7  :   DONEU();break;
      case 8  :   QUIT();break;
      default :   break;
  }

}

void LSTU(){
  /* read files from the directory */
  DIR *d;
  struct dirent *dir;
  d = opendir("users");
  if (d)
    {   bzero(messagesend,BUFSIZE);
        strcpy(messagesend, "Userlist:\n");
        while ((dir = readdir(d)) != NULL)
        {  if ( !(!(strcmp(dir->d_name, ".")) || !(strcmp(dir->d_name,".."))))
            { /* append each file name to the reply message */
              strcat(messagesend, dir->d_name);
              strcat(messagesend, " ");
            }
        }
        /* reply with this if no user available */
        if (strlen(messagesend) == 10)
          {
            bzero(messagesend,BUFSIZE);
            strcpy(messagesend,"Userlist is empty");
          }
        closedir(d);
    }
  else
  { /* reply with this if directory not available */
    bzero(messagesend,BUFSIZE);
    strcpy(messagesend,"\'users\' directory not found.");
  }
}

/* add a user by creating a user spool file */
void ADDU(){
  FILE *fp, *fp2;
  char fname[50];
  bzero(fname, 50);
  strcpy(fname, "users/");
  strcat(fname, arg2);
  fp = fopen(fname, "r");
  if (fp == NULL)
  { 
    fp2 = fopen(fname, "wr");
    if (fp2 == NULL)
      strcpy(messagesend,"Error occurred, while adding user.");
    else
    { 
      bzero(messagesend, BUFSIZE);
      strcpy(messagesend, "User has been added successfully.");
    }
  }
  else {
    bzero(messagesend, BUFSIZE);
    strcpy(messagesend, "User already exist.");
    fclose(fp);
  }
  bzero(arg2,ARGSIZE);
}

void USER(){
  FILE *fp, *fp2;
  char fname[50];
  bzero(fname, 50);
  strcpy(fname, "users/");
  strcat(fname, arg2);
  fp = fopen(fname, "r");
  if (fp == NULL)
  { 
    bzero(messagesend, BUFSIZE);
    strcpy(messagesend, "INVALIDUSER");
  }
  else 
  {
    int k=0;
    for(k = 0;k<EMAILNO;k++)
      bzero(email[k], BUFSIZE);
    char buf5[BUFSIZE],buf6[BUFSIZE];
    bzero(buf5, BUFSIZE);
    count = 0;
    k = 0;
    char rep = 'y';
        bzero(buf6, BUFSIZE);
        while (fgets(buf5, BUFSIZE, fp) != NULL)
        {
            int i=0;
            strcat(email[count],buf5);
            while(buf5[i] != NULL)
            {   
                if (buf5[i] == '#')
                {   if (buf5[i+1] == '#')
                        if (buf5[i+2] == '#')
                        {   
                          count++;                          
                        }
                }
                i++;
            }
            bzero(buf5, BUFSIZE);
        }
    bzero(messagesend, BUFSIZE);
    sprintf(messagesend, "User exists and has %d messages.", count);
    fclose(fp);
  }
}

void READM(/*parameters*/){
  /*function defination*/
  bzero(messagesend,BUFSIZE);
    strcat(messagesend,email[readcount]);
    if(strlen(messagesend) > 0)
      readcount++;
    else
    { /* once user reaches the end of the mail, then after giving him INVALID reply, we will again showing the messages from start */
      strcpy(messagesend, "INVALID: No More Mail\n");
      readcount = 0;
    }
}

void DELM(/*parameters*/){
  /*function defination*/
  int i;
  /* check if user is having any messages */
  if(count > 0)
  {
    i = readcount;
    /* copy each emails to previous one for filling the deleted value */
    while (strlen(email[i]) > 0 && i < count)
    { 
      bzero(email[i-1], BUFSIZE);
      strcpy(email[i-1], email[i]);
      i++;
    }
    bzero(email[i-1],BUFSIZE);
    if(readcount > 0)
      readcount--;
    
    FILE *fp;
    char fname[50];
    bzero(fname, 50);
    strcpy(fname, "users/");
    strcat(fname, username);
    /* open file and append rest of the remaining emails into it */
    fp = fopen(fname, "w");
    if (fp == NULL)
    { 
      bzero(messagesend, BUFSIZE);
      strcpy(messagesend, "Cannot Delete.");
    }
    else 
    {
      fputs(email[0],fp);
      fclose(fp);
    
      fp = fopen(fname, "a");
      if (fp == NULL)
      { 
        bzero(messagesend, BUFSIZE);
        strcpy(messagesend, "Error occurred while deleting.");
      }
      else 
      {
        i = 1;
        /* write mails into the user file */
        while(strlen(email[i]) > 0)
        {
          fputs(email[i],fp);
          i++;
        }
        fclose(fp);
      }
      strcpy(messagesend, "successfully deleted.");
      count--;
    }
  }
  else
  {
    bzero(messagesend,BUFSIZE);
    strcpy(messagesend,"Nothing to Delete");
  }
}

void SEND(int childfd){
  bzero(messagesend,BUFSIZE);
  strcpy(messagesend,"SEND is called.");
  int n;
  FILE *fp;
  char fname[50];
  char ACK[20];
  bzero(fname, 50);
  strcpy(fname, "users/");
  strcat(fname, sendtouser);
  fp = fopen(fname, "r");
  if (fp == NULL)
  { 
    bzero(messagesend,BUFSIZE);
    strcpy(messagesend,"INVALIDUSER");
    return;
  }
  else 
  {
    bzero(ACK, 20);
    strcpy(ACK, "AVAILABLEUSER");
    n = write(childfd, ACK, strlen(ACK));
    if (n < 0) 
      error("ERROR writing to socket");
    bzero(ACK, 20);
    fclose(fp);
    
    /* store time with abbreviated timezone */
    char mytime[70];                                                                
    time_t temp;                                                                  
    struct tm *timeptr;                                                           
                                                                                  
    temp = time(NULL);                                                            
    timeptr = localtime(&temp);                                                   
    strftime(mytime,sizeof(mytime)-1,"%a %b %d %T %Z %Y\n", timeptr);                            
  
    bzero(subject,BUFSIZE);
    n = read(childfd, subject, BUFSIZE);
    if (n < 0) 
      error("ERROR reading from socket");
  
    bzero(message,BUFSIZE);
    n = read(childfd, message, BUFSIZE);
    if (n < 0) 
      error("ERROR reading from socket");
    if (strlen(subject)>0 || strlen(message)>0)
    {
      char createemail[BUFSIZE+strlen(subject)];
      bzero(createemail,BUFSIZE+strlen(subject));
      if(strlen(message) == 0)
        strcpy(message,"###\n");
      sprintf(createemail,"From: %s\nTo: %s\nDate: %sSubject: %sMessage: %s", username, sendtouser, mytime, subject, message);
      bzero(subject,BUFSIZE);
      bzero(message,BUFSIZE);
      fp = fopen(fname, "a");
      if (fp == NULL)
          perror("ERROR file opening");
      fputs(createemail, fp);
      fclose(fp);
      bzero(messagesend,BUFSIZE);
      strcpy(messagesend,"Send Successfully");
    }
    else
    {
      bzero(messagesend,BUFSIZE);
      strcpy(messagesend,"Nothing to send (empty message).");
    }
  }
}

void DONEU(){
  readcount = 0;
  bzero(messagesend,BUFSIZE);
  strcpy(messagesend,"Logged Out");
}

void QUIT(){
  bzero(messagesend,BUFSIZE);
  strcpy(messagesend,"Session over");
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
  switch(argc){
    case 1: portno = 4567;
            printf("Server running on port 4567 by default.\n");
            break;
    case 2: portno = atoi(argv[1]);
            break;
    default:fprintf(stderr, "usage: %s <port>\n", argv[0]);
            exit(1);
  }

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
  serveraddr.sin_addr.s_addr = INADDR_ANY;

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
    
    hostp = gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr, sizeof(clientaddr.sin_addr.s_addr), AF_INET);   
    /*if (hostp == NULL)
      error("ERROR on gethostbyaddr");*/

    hostaddrp = inet_ntoa(clientaddr.sin_addr);
    if (hostaddrp == NULL)
      error("ERROR on inet_ntoa\n");
    if(!(strcmp(hostaddrp,"127.0.0.1")))
      strcpy(hostaddrp,"localhost");
    if (hostp != NULL)
      printf("server established connection with %s (%s)\n", hostp->h_name, hostaddrp);
    else
      printf("server established connection with %s\n", hostaddrp);
    
    /* 
     * read: read input string from the client
     */
    readcount = 0;
    char rep='y';
    /* keep taking input  */
    while(rep=='y'){
      bzero(buf, BUFSIZE);
      n = read(childfd, buf, BUFSIZE);
      if (n < 0) 
        error("ERROR reading from socket");
      if(strlen(buf)>0)
      {
        if(!strcmp(buf,"QUIT")){
          rep='n';
        }
          bzero(buf2, BUFSIZE);
          strcpy(buf2,buf);
          server_interface(childfd);
          if(strlen(messagesend) > 0) {
            bzero(buf, BUFSIZE);
            strcpy(buf,messagesend);
          }
        
        /* 
         * write: reply message to the client 
         */
        if (strlen(messagesend)>0)
        {  n = write(childfd, buf, strlen(buf));
          if (n < 0) 
            error("ERROR writing to socket");
        }
        bzero(messagesend, BUFSIZE);
      }
      else break;
    }
    close(childfd);
  }
}