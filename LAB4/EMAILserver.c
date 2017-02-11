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
#define EMAILNO 50
char subject[BUFSIZE]; message[BUFSIZE]; /* message buffer */
char buf2[BUFSIZE], buf3[BUFSIZE], email[EMAILNO][BUFSIZE];
char arg1[ARGSIZE], arg2[ARGSIZE], username[ARGSIZE],sendtouser[ARGSIZE];
int fnlist, emailnumber, totalemail;
int count=0;
int readcount=0;


/*all functions*/
void server_interface(int childfd/*parameters*/){
  /*function defination*/
  bzero(arg1,ARGSIZE);
  printf("server interface, read string : %s\n", buf2);
  char *cmd = strtok(buf2," ");
  int i = 0;
  while (cmd != NULL){
      if(i == 0)
      {   
        strcpy(arg1,cmd);
        if(!strcmp(arg1,"ADDU") || !strcmp(arg1,"ADDU"))
        { bzero(arg2,BUFSIZE); printf("arg2 is made zero\n"); }
      }
      else if(i == 1)
          strcpy(arg2,cmd);
      cmd = strtok(NULL, " ");
      i++;
  }

  printf("arg1 : %s, arg2 : %s\n", arg1,arg2);

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
  /*printf("fnlist : %d\n", fnlist);*/
  /*bzero(buf,BUFSIZE);*/
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
  bzero(arg2,ARGSIZE);
}

void USER(/*parameters*/){
  /*function defination*/
  printf("USER is called.\n");
  FILE *fp, *fp2;
  char fname[50];
  bzero(fname, 50);
  strcpy(fname, "users/");
  strcat(fname, arg2);
  printf("fname : %s\n", fname);
  fp = fopen(fname, "r");
  if (fp == NULL)
  { 
    bzero(buf3, BUFSIZE);
    strcpy(buf3, "User does not exist.");
    printf("%s\n", buf3);
  }
  else {
    /*printf("User already exist.\n");*/
    
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
            /*if (buf5[i] == NULL)
              rep = 'n';*/
            bzero(buf5, BUFSIZE);
        }
        /*if (strlen(buf6) > 0)
            {
              strcpy(email[k],buf6);
              k++;
            }*/
    bzero(buf3, BUFSIZE);
    sprintf(buf3, "ACK : User exists and has %d messages.", count);
    /*printf("buf3 : %s\n", buf3);
    for (k=0;k<count;k++)
      printf("%s\n", email[k]);*/
    /*if(write(childfd, buf3, strlen(buf3)) < 0)
        error("ERROR writing to socket");*/
    fclose(fp);
  }
}

void READM(/*parameters*/){
  /*function defination*/
  printf("READM is called.\n");
  bzero(buf3,BUFSIZE);
  /*for( i = 0; i<count;i++) */
    printf("email[%d] : %s\n",readcount,email[readcount]);
    strcat(buf3,email[readcount]);
    if(strlen(buf3) > 0)
      readcount++;
    else
      strcpy(buf3, "INVALID : No More Mail");
}

void DELM(/*parameters*/){
  /*function defination*/
  printf("DELM is called.\n");
  emailnumber = atoi(arg2);
  printf("emailnumber: %d\n", strlen(arg2));
  int i;
  if(count > 0)
  {
    i = readcount;
    /*if( i == 0 )
    {
      while(strlen(email[i]) > 0)
      {
        bzero(email[i], BUFSIZE);
        if(strlen(email[i+1]) > 0)
          strcpy(email[i],email[i+1]);
        i++;
      }
    }  
    else*/
    {
      while (strlen(email[i]) > 0 && i < count)
      { 
        printf("email [%d-1] : %s , email [%d] : %s\n",i,email[i-1],i,email[i]);
        bzero(email[i-1], BUFSIZE);
        strcpy(email[i-1], email[i]);
        i++;
      }
      bzero(email[i-1],BUFSIZE);
      if(readcount > 0)
        readcount--;
    }
    FILE *fp;
    char fname[50];
    bzero(fname, 50);
    strcpy(fname, "users/");
    printf("arg2 : %s\n", username);
    strcat(fname, username);
    printf("fname : %s\n", fname);
    fp = fopen(fname, "w");
    if (fp == NULL)
    { 
      bzero(buf3, BUFSIZE);
      strcpy(buf3, "Cannot Delete.");
      printf("%s\n", buf3);
    }
    else 
    {
      fputs(email[0],fp);
      fclose(fp);
    }
    fp = fopen(fname, "a");
    if (fp == NULL)
    { 
      bzero(buf3, BUFSIZE);
      strcpy(buf3, "Error occurred while deleting.");
      printf("%s\n", buf3);
    }
    else 
    {
      i = 1;
      while(strlen(email[i]) > 0)
      {
        fputs(email[i],fp);
        i++;
      }
      fclose(fp);
    }
    strcpy(buf3, "successfully deleted.");
    printf("buf3 : %s\n", buf3);
  }
}

void SEND(int childfd/*parameters*/){
  /*function defination*/
  bzero(buf3,BUFSIZE);
  strcpy(buf3,"SEND is called.");
  printf("SEND is called.\n");
  printf("From :%s, To:%s\n", username, sendtouser);
  int n;
  FILE *fp;
  char fname[50];
  char ACK[20];
  bzero(fname, 50);
  strcpy(fname, "users/");
  /*printf("sendtouser : %s\n", sendtouser);*/
  strcat(fname, sendtouser);
  printf("fname : %s\n", fname);
  fp = fopen(fname, "r");
  if (fp == NULL)
  { 
    /*bzero(ACK, 20);
    strcpy(ACK, "INVALIDUSER");
    printf("%s\n", ACK);
    n = write(childfd, ACK, strlen(ACK));
    if (n < 0) 
      error("ERROR writing to socket");
    bzero(ACK, 20);*/
    bzero(buf3,BUFSIZE);
    strcpy(buf3,"INVALIDUSER");
    return;
  }
  else 
  {
    bzero(ACK, 20);
    strcpy(ACK, "AVAILABLEUSER");
    printf("%s\n", ACK);
    n = write(childfd, ACK, strlen(ACK));
    if (n < 0) 
      error("ERROR writing to socket");
    bzero(ACK, 20);
    fclose(fp);
    
    time_t mytime;
    mytime = time(NULL);
    printf("time :%s", ctime(&mytime));

    bzero(subject,BUFSIZE);
    n = read(childfd, subject, BUFSIZE);
    if (n < 0) 
      error("ERROR reading from socket");
    printf("subject :%s", subject);

    bzero(message,BUFSIZE);
    n = read(childfd, message, BUFSIZE);
    if (n < 0) 
      error("ERROR reading from socket");
    printf("Message :%s", message);
    printf("subject length: %d,   message length:%d\n",strlen(subject),strlen(message));
    
      char createemail[BUFSIZE];
      bzero(createemail,BUFSIZE);
      sprintf(createemail,"From: %s\nTo: %s\nDate: %sSubject: %sMessage: %s", username, sendtouser, ctime(&mytime), subject, message);
      bzero(subject,BUFSIZE);
      bzero(message,BUFSIZE);
      printf("created email\n%s", createemail);
      fp = fopen(fname, "a");
      if (fp == NULL)
          perror("ERROR file opening");
      fputs(createemail, fp);
      fclose(fp);
      strcpy(buf3,"Send Successfully");    
  }
}

void DONEU(/*parameters*/){
  /*function defination*/
  printf("DONEU is called.\n");
  readcount = 0;
  strcpy(buf3,"Logged Out");
}

void QUIT(/*parameters*/){
  /*function defination*/
  printf("QUIT is called.\n");
  strcpy(buf3,"Session over");
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
    readcount = 0;
    char rep='y';
    while(rep=='y'){
      bzero(buf, BUFSIZE);
      n = read(childfd, buf, BUFSIZE);
      if (n < 0) 
        error("ERROR reading from socket");
      printf("buf length:%d\n",strlen(buf));
      if(strlen(buf)>0)
      {
        if(!strcmp(buf,"QUIT")){
          rep='n';
        }
        
        
          bzero(buf2, BUFSIZE);
          strcpy(buf2,buf);
          server_interface(childfd);
          if(strlen(buf3) > 0) {
            bzero(buf, BUFSIZE);
            strcpy(buf,buf3);
          }
          else
            printf("buf3 length in zero.\n");
        
        printf("server received %d bytes: %s\n", n, buf);
        /* 
         * write: echo the input string back to the client 
         */
        if (strlen(buf3)>0)
        {  n = write(childfd, buf, strlen(buf));
          if (n < 0) 
            error("ERROR writing to socket");
        }
        bzero(buf3, BUFSIZE);
      }
      else break;
    }
    close(childfd);
  }
}