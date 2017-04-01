#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

#define BUFSIZE 1024
#define ARGSIZE 40
#define SUBJECTSIZE 100
int sockfd, portno, n;
struct sockaddr_in serveraddr;
struct hostent *server;
char *hostname;
char buf[BUFSIZE];  /* for buffer */
char buf2[BUFSIZE];     /* buffer for message */
char buf3[BUFSIZE];     /* buffer for storing original string in sub prompt */
char orig_str[BUFSIZE],orig_str2[BUFSIZE];  /* to store original input commands */
int i, fnlist;  /* loop variable and function list */
char arg1[ARGSIZE], arg2[ARGSIZE], arg3[ARGSIZE], arg4[ARGSIZE];    /* store commands and 2nd arguments (1,2 for interface and 3,4 for user function) */
char subject[SUBJECTSIZE];  /* to store subject of the message */
/*all functions*/

void client_interface(/*arguments*/){
    /*defination*/
        /* create socket */
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
        bcopy((char *)server->h_addr_list[0], (char *)&serveraddr.sin_addr.s_addr, server->h_length);     
        /* create server address */
        serveraddr.sin_family = AF_INET;
        serveraddr.sin_port = htons(portno);

        /* connect: create a connection with the server */
        if (connect(sockfd, &serveraddr, sizeof(serveraddr)) < 0) 
          error("ERROR connecting"); 
    while(1){
        
        /* main input prompt */
        printf("Main-Prompt > ");
        bzero(orig_str,BUFSIZE);
        fgets(orig_str, BUFSIZE, stdin);
        bzero(buf,BUFSIZE);
        strcpy(buf, orig_str);
        char *cmd = strtok(buf," ");
        int i=0;
        /* store command and followed string */
        while (cmd != NULL){
            if(i == 0)
                strcpy(arg1,cmd);
            else if(i == 1)
                strcpy(arg2,cmd);
            cmd = strtok(NULL, " ");
            i++;
        }
        
        /* remove next line character from the end of the string */
        if (arg1!=NULL){
            if (arg1[strlen(arg1)-1] == '\n')
                arg1[strlen(arg1)-1] = NULL;
        }
        if (arg2!=NULL){
            if (arg2[strlen(arg2)-1] == '\n')
                arg2[strlen(arg2)-1] = NULL;
        }

        i = 0;
        int operatorlen = strlen(arg1);
        /*convert operator string to lowercase, so that operater is no longer case sensitive*/
        while (i< operatorlen){
            if(arg1[i] >= 65 && arg1[i]<=90)
                arg1[i] = arg1[i] + 32;
            i++;
        }

        /* assign function number to functions for switch */
        if(!strcmp(arg1,"listusers"))
            fnlist = 1;
        else if(!strcmp(arg1,"adduser"))
            fnlist = 2;
        else if(!strcmp(arg1,"setuser"))
            fnlist = 3;
        else if(!strcmp(arg1,"quit"))
            fnlist = 4;
        else fnlist = 0;
        switch(fnlist){
            case 1  :   Listusers();break;
            case 2  :   Adduser();break;
            case 3  :   SetUser();break;
            case 4  :   Quit();break;
            default :   printf("Command not found: %s",orig_str);break;
        }
        if (fnlist == 4)
            break;
    }
}

/* function to list available users */
void Listusers(){
    bzero(buf,BUFSIZE);
    strcpy(buf,"LSTU");

    /* sending to server */
    n = write(sockfd, buf, strlen(buf));
    if (n < 0) 
      error("ERROR writing to socket");

    bzero(buf, BUFSIZE);
    /* receiving from server */
    n = read(sockfd, buf, BUFSIZE);
    if (n < 0) 
      error("ERROR reading from socket");
    printf("%s\n", buf);
}

/* function to add users */
void Adduser(){
    /* adduser user only if username is having any value */
    if (strlen(arg2) <= 0)
        printf("Invalid username.\n");
    else
    {
        bzero(buf,BUFSIZE);
        strcpy(buf, "ADDU ");
        strcat(buf, arg2);
        bzero(arg2,ARGSIZE);
        n = write(sockfd, buf, strlen(buf));
        if (n < 0) 
          error("ERROR writing to socket");

        bzero(buf, BUFSIZE);
        n = read(sockfd, buf, BUFSIZE);
        if (n < 0) 
          error("ERROR reading from socket");
        printf("%s\n", buf);
    }
}

/* function to set user and give sub prompt */
void SetUser(){
    /* set user only if it is having any value */
    if (strlen(arg2) <= 0)
        printf("Please provide a username\n");
    else
    {   bzero(buf,BUFSIZE);
        strcpy(buf, "USER ");
        strcat(buf, arg2);
        /* send username to server */
        n = write(sockfd, buf, strlen(buf));
        if (n < 0) 
          error("ERROR writing to socket");
        
        /* read if user is available in server */
        bzero(buf, BUFSIZE);
        n = read(sockfd, buf, BUFSIZE);
        if (n < 0) 
          error("ERROR reading from socket");
        printf("%s\n", buf);
        /* check first letter of received string */
        if (!strcmp(strtok(buf," "),"User"))
        {
            while(1){
                /* setting sub prompt for available user */   
                printf("Sub-Prompt-%s > ", arg2);
                bzero(orig_str2,BUFSIZE);
                fgets(orig_str2, BUFSIZE, stdin);
                bzero(buf3,BUFSIZE);
                strcpy(buf3, orig_str2);
                char *cmd = strtok(buf3," ");
                int i=0;
                /* storing sub prompt command and followed string */
                while (cmd != NULL){
                    if(i == 0)
                        strcpy(arg3,cmd);
                    else if(i == 1)
                        strcpy(arg4,cmd);
                    cmd = strtok(NULL, " ");
                    i++;
                }
                /* removed next line character from the end of the command and followed string */
                if (arg3!=NULL){
                    if (arg3[strlen(arg3)-1] == '\n')
                        arg3[strlen(arg3)-1] = NULL;
                }
                if (arg4!=NULL){
                    if (arg4[strlen(arg4)-1] == '\n')
                        arg4[strlen(arg4)-1] = NULL;
                }

                i = 0;
                int operatorlen = strlen(arg3);
                /*convert operator string to lowercase, so that operater is no longer case sensitive*/
                while (i< operatorlen){
                    if(arg3[i] >= 65 && arg3[i]<=90)
                        arg3[i] = arg3[i] + 32;
                    i++;
                }

                /* assigning function list for switching */
                if(!strcmp(arg3,"read"))
                    fnlist = 5;
                else if(!strcmp(arg3,"send"))
                    {   if(arg4 == NULL)    /* check if send <user> is having any value*/
                            continue;
                        else
                            fnlist = 6;
                    }
                else if(!strcmp(arg3,"delete"))
                    fnlist = 7;
                else if(!strcmp(arg3,"done"))
                    fnlist = 8;
                else fnlist = 0;
                /* switch for sub prompt */
                switch(fnlist){
                    case 5  :   Read();break;
                    case 6  :   Send();break;
                    case 7  :   Delete();break;
                    case 8  :   Done();break;
                    default :   printf("Command not found: %s",orig_str2);break;
                }
                if (fnlist == 8)
                    break;
            }
        }
    }
    bzero(arg2,ARGSIZE);
}

/* function to read from server */
void Read(){
    bzero(buf,BUFSIZE);
    strcpy(buf,"READM");
    n = write(sockfd, buf, strlen(buf));
    if (n < 0) 
    error("ERROR writing to socket");

    bzero(buf, BUFSIZE);
    n = read(sockfd, buf, BUFSIZE);
    if (n < 0) 
      error("ERROR reading from socket");
    printf("%s", buf);
}

/* function to delete message */
void Delete(){
    bzero(buf,BUFSIZE);
    strcpy(buf,"DELM");
    n = write(sockfd, buf, strlen(buf));
    if (n < 0) 
    error("ERROR writing to socket");

    bzero(buf, BUFSIZE);
    n = read(sockfd, buf, BUFSIZE);
    if (n < 0) 
      error("ERROR reading from socket");
    printf("%s\n", buf);
}

/* function to send message to another available user */
void Send(){
    if(strlen(arg4) > 0)
    {

        bzero(buf2, BUFSIZE);
        strcpy(buf2, "SEND ");
        strcat(buf2, arg4);
        /* send username to server */
        n = write(sockfd, buf2, strlen(buf2));
        if (n < 0) 
        error("ERROR writing to socket");
        
        char ACK[50];
        bzero(ACK, 50);
        /* receive inforfation, if username is available in server */
        n = read(sockfd, ACK, 50);
        if (n < 0) 
          error("ERROR reading from socket");
        if (! strcmp(ACK, "INVALIDUSER"))
        {   
            printf("%s\n", ACK);
            bzero(ACK, 20);
            return;
        }
        else
        {   /* take message subject and content from user */
            printf("Subject: ");
            bzero(subject, SUBJECTSIZE);
            fgets(subject, SUBJECTSIZE, stdin);
            bzero(buf, BUFSIZE);
            n = write(sockfd, subject, strlen(subject));
            if (n < 0) 
              error("ERROR reading from socket");

            printf("Type Message: ");
            bzero(buf,BUFSIZE);
            char previous_line[BUFSIZE];
            bzero(previous_line, BUFSIZE);
            char rep = 'y';
            /* input content continuously till ### is not entered or buffer get full */
            int k = 0;
            while (rep == 'y')
            {
                int i=0;
                bzero(buf2,BUFSIZE);
                fgets(buf2, BUFSIZE, stdin);            
                while(buf2[i] != NULL)
                {   
                    if (buf2[i] == '#')
                    {   if (buf2[i+1] == '#')
                            if (buf2[i+2] == '#')
                            {   
                                rep = 'n';
                                int j=i+3;
                                while(buf2[j+1] != NULL)
                                {
                                    buf2[j] = NULL;
                                    j++;
                                }
                                buf2[i+3] = '\n';
                                break;
                            }
                    }
                    i++;
                    k++;
                }
                
                /* if buffer is full then make last three character ### */
                if(strlen(buf)+strlen(buf2)+strlen(subject) > BUFSIZE-4)
                {   
                    if ( previous_line[strlen(previous_line) - 1] == '\n')
                        previous_line[strlen(previous_line) - 1] = NULL;
                    printf("Buffer is full.\nSending message till\n\"%s\"\n", previous_line);
                    buf[strlen(buf)] = '#';
                    buf[strlen(buf)] = '#';
                    buf[strlen(buf)] = '#';
                    buf[strlen(buf)] = '\n';
                    break;
                }
                else
                {
                    strcat(buf,buf2);
                    bzero(previous_line, BUFSIZE);
                    strcpy(previous_line, buf2);
                }
            }
            /* write message to server */
            bzero(buf2,BUFSIZE);
            n = write(sockfd, buf, strlen(buf));
            if (n < 0) 
            error("ERROR writing to socket");
            
            /* server status about writing message */
            bzero(buf, BUFSIZE);
            n = read(sockfd, buf, BUFSIZE);
            if (n < 0) 
              error("ERROR reading from socket");
            printf("%s\n", buf);
        }
    }
    else
        printf("send username is empty\n");
}

void Done(){
    bzero(buf,BUFSIZE);
    strcpy(buf,"DONEU");
    n = write(sockfd, buf, strlen(buf));
    if (n < 0) 
      error("ERROR writing to socket");

    bzero(buf, BUFSIZE);
    n = read(sockfd, buf, BUFSIZE);
    if (n < 0) 
      error("ERROR reading from socket");
    printf("%s\n", buf);
}

void Quit(){
    bzero(buf,BUFSIZE);
    strcpy(buf, "QUIT");
    n = write(sockfd, buf, strlen(buf));
    if (n < 0) 
      error("ERROR writing to socket");

    bzero(buf, BUFSIZE);
    n = read(sockfd, buf, BUFSIZE);
    if (n < 0) 
      error("ERROR reading from socket");
    printf("%s\n", buf);
    /* close socket */
    close(sockfd);
}

void error(char *msg) {
    perror(msg);
    exit(0);
}

int main(int argc, char **argv) {
    /* check command line arguments */
    switch(argc){
        case 1: /*strcpy(hostname,"127.0.0.1");*/
                hostname = "127.0.0.1";
                portno = 4567;
                printf("Client running on localhost:4567 by default\n");
                break;
        case 2: hostname = "127.0.0.1";
                portno = atoi(argv[1]);
                printf("Client running of localhost by default.\n");
                break;
        case 3: /* if localhost is typed then changing it to ip address */
                if(!(strcmp(argv[1],"localhost")))
                    hostname = "127.0.0.1";
                else
                    hostname = argv[1];
                portno = atoi(argv[2]);
                break;
        default:if (argc != 3) {
                fprintf(stderr,"usage: %s <hostname> <port>\n", argv[0]);
                exit(0);
        }
    }
    
    /* calling interface function */
    client_interface();
    return 0;
}