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
char buf[BUFSIZE];
char buf2[BUFSIZE];
char buf3[BUFSIZE];
char orig_str[BUFSIZE],orig_str2[BUFSIZE];
int i, fnlist;
char arg1[ARGSIZE], arg2[ARGSIZE], arg3[ARGSIZE], arg4[ARGSIZE];
char subject[SUBJECTSIZE];
/*all functions*/

void input_interface(/*arguments*/){
    /*defination*/
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
        bcopy((char *)server->h_addr_list[0], 
          (char *)&serveraddr.sin_addr.s_addr, server->h_length);
        serveraddr.sin_port = htons(portno);

        /* connect: create a connection with the server */
        if (connect(sockfd, &serveraddr, sizeof(serveraddr)) < 0) 
          error("ERROR connecting"); 
    while(1){
           
        printf("Main-Prompt > ");
        bzero(orig_str,BUFSIZE);
        fgets(orig_str, BUFSIZE, stdin);
        bzero(buf,BUFSIZE);
        strcpy(buf, orig_str);
        char *cmd = strtok(buf," ");
        int i=0;
        /*printf("Received:");*/
        while (cmd != NULL){
            /*printf(" %s", cmd );*/
            if(i == 0)
                strcpy(arg1,cmd);
            else if(i == 1)
                strcpy(arg2,cmd);
            cmd = strtok(NULL, " ");
            i++;
        }
        /*printf("arg1 : %s,  arg2 : %s,  arg3 : %s \n",arg1,arg2,arg3);
        printf("strlen of arg2 : %d \n", arg2!=NULL?strlen(arg2):0);*/
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

        if(!strcmp(arg1,"listusers"))
            fnlist = 1;
        else if(!strcmp(arg1,"adduser"))
            fnlist = 2;
        else if(!strcmp(arg1,"setuser"))
            fnlist = 3;
        else if(!strcmp(arg1,"quit"))
            fnlist = 4;
        else fnlist = 0;
        /*printf("fnlist : %d\n", fnlist);*/
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

void Listusers(/*arguments*/){
    /*defination*/
    printf("listuser is called.\n");
    bzero(buf,BUFSIZE);
    strcpy(buf,"LSTU");
    printf("buf : %s\n", buf);
    n = write(sockfd, buf, strlen(buf));
    if (n < 0) 
      error("ERROR writing to socket");

    bzero(buf, BUFSIZE);
    n = read(sockfd, buf, BUFSIZE);
    if (n < 0) 
      error("ERROR reading from socket");
    printf("Echo from server: %s\n", buf);
}

void Adduser(/*arguments*/){
    /*defination*/
    if (strlen(arg2) <= 0)
        printf("Invalid username.\n");
    else
    {
        printf("adduser is called. id : %s\n", arg2);
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
        printf("Echo from server: %s\n", buf);
    }
}

void SetUser(/*arguments*/){
    /*defination*/
    if (strlen(arg2) <= 0);
    else
    {   printf("setuser is called. id : %s\n", arg2);
        bzero(buf,BUFSIZE);
        strcpy(buf, "USER ");
        strcat(buf, arg2);
        n = write(sockfd, buf, strlen(buf));
        if (n < 0) 
          error("ERROR writing to socket");

        bzero(buf, BUFSIZE);
        n = read(sockfd, buf, BUFSIZE);
        if (n < 0) 
          error("ERROR reading from socket");
        printf("Echo from server: %s\n", buf);
        if (!strcmp(strtok(buf," "),"ACK"))
        {
            while(1){
                   
                printf("Sub-Prompt-%s > ", arg2);
                bzero(orig_str2,BUFSIZE);
                fgets(orig_str2, BUFSIZE, stdin);
                bzero(buf3,BUFSIZE);
                strcpy(buf3, orig_str2);
                char *cmd = strtok(buf3," ");
                int i=0;
                /*printf("Received:");*/
                while (cmd != NULL){
                    /*printf(" %s", cmd );*/
                    if(i == 0)
                        strcpy(arg3,cmd);
                    else if(i == 1)
                        strcpy(arg4,cmd);
                    cmd = strtok(NULL, " ");
                    i++;
                }
                /*printf("arg3 : %s,  arg4 : %s \n",arg3,arg4);
                printf("strlen of arg4 : %d \n", arg4!=NULL?strlen(arg4):0);*/
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

                if(!strcmp(arg3,"read"))
                    fnlist = 5;
                else if(!strcmp(arg3,"send"))
                    {   if(arg4 == NULL)
                            continue;
                        else
                            fnlist = 6;
                    }
                else if(!strcmp(arg3,"delete"))
                    fnlist = 7;
                else if(!strcmp(arg3,"done"))
                    fnlist = 8;
                else fnlist = 0;
                /*printf("fnlist : %d\n", fnlist);*/
                /*bzero(buf,BUFSIZE);*/
                switch(fnlist){
                    case 5  :   Read();break;
                    case 6  :   Send();break;
                    case 7  :   Delete();break;
                    case 8  :   Done();break;
                    default :   printf("Command not found: %s",orig_str2);break;
                }
                /*n = write(sockfd, buf, strlen(buf));
                if (n < 0) 
                  error("ERROR writing to socket");

                bzero(buf, BUFSIZE);
                n = read(sockfd, buf, BUFSIZE);
                if (n < 0) 
                  error("ERROR reading from socket");
                printf("Echo from server: %s\n", buf);*/
                if (fnlist == 8)
                    break;
            }
        }
    }
    bzero(arg2,ARGSIZE);
}

void Read(/*arguments*/){
    /*defination*/
    printf("Read is called.\n");
    bzero(buf,BUFSIZE);
    strcpy(buf,"READM");
    n = write(sockfd, buf, strlen(buf));
    if (n < 0) 
    error("ERROR writing to socket");

    bzero(buf, BUFSIZE);
    n = read(sockfd, buf, BUFSIZE);
    if (n < 0) 
      error("ERROR reading from socket");
    printf("Echo from server: %s\n", buf);
}

void Delete(/*arguments*/){
    /*defination*/
    printf("Delete is called.\n");
    bzero(buf,BUFSIZE);
    strcpy(buf,"DELM");
    n = write(sockfd, buf, strlen(buf));
    if (n < 0) 
    error("ERROR writing to socket");

    bzero(buf, BUFSIZE);
    n = read(sockfd, buf, BUFSIZE);
    if (n < 0) 
      error("ERROR reading from socket");
    printf("Echo from server: %s\n", buf);
}

void Send(/*arguments*/){
    /*defination*/
    if (strlen(arg4) <= 0);
    else
    {    
        printf("Send is called.\n");
        printf("subject: ");
        bzero(subject, SUBJECTSIZE);
        fgets(subject, SUBJECTSIZE, stdin);
        printf("Type Message: ");
        bzero(buf,BUFSIZE);
        /*fgets(buf,BUFSIZE,stdin);*/
        char rep = 'y';
        while (rep == 'y')
        {
            int i=0;
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
            }
            strcat(buf,buf2);
        }

        n = write(sockfd, subject, strlen(subject));
        if (n < 0) 
        error("ERROR writing to socket");

        bzero(subject, SUBJECTSIZE);
        n = read(sockfd, subject, SUBJECTSIZE);
        if (n < 0) 
          error("ERROR reading from socket");
        printf("Echo from server: %s", subject);

        bzero(buf2, BUFSIZE);
        strcpy(buf2, "SEND ");
        strcat(buf2, arg4);
        n = write(sockfd, buf2, strlen(buf2));
        if (n < 0) 
        error("ERROR writing to socket");

        bzero(buf2, BUFSIZE);
        n = read(sockfd, buf2, BUFSIZE);
        if (n < 0) 
          error("ERROR reading from socket");
        /*printf("Echo from server: %s\n", buf2);*/

        n = write(sockfd, buf, strlen(buf));
        if (n < 0) 
        error("ERROR writing to socket");

        bzero(buf, BUFSIZE);
        n = read(sockfd, buf, BUFSIZE);
        if (n < 0) 
          error("ERROR reading from socket");
        printf("Echo from server: %s", buf);
    }
}

void Done(/*arguments*/){
    /*defination*/
    printf("Done is called.\n");
    bzero(buf,BUFSIZE);
    strcpy(buf,"DONEU");
    n = write(sockfd, buf, strlen(buf));
    if (n < 0) 
    error("ERROR writing to socket");

    bzero(buf, BUFSIZE);
    n = read(sockfd, buf, BUFSIZE);
    if (n < 0) 
      error("ERROR reading from socket");
    printf("Echo from server: %s\n", buf);
}

void Quit(/*arguments*/){
    /*defination*/
    printf("quit is called.\n");
    bzero(buf,BUFSIZE);
    strcpy(buf, "QUIT");
    n = write(sockfd, buf, strlen(buf));
    if (n < 0) 
      error("ERROR writing to socket");

    bzero(buf, BUFSIZE);
    n = read(sockfd, buf, BUFSIZE);
    if (n < 0) 
      error("ERROR reading from socket");
    printf("Echo from server: %s\n", buf);
    close(sockfd);
}

void error(char *msg) {
    perror(msg);
    exit(0);
}

int main(int argc, char **argv) {
    /*int sockfd, portno, n;
    struct sockaddr_in serveraddr;
    struct hostent *server;
    char *hostname;
    char buf[BUFSIZE];
    char buf2[BUFSIZE];*/
    /*char buf3[BUFSIZE];*/

    /* check command line arguments */
    if (argc != 3) {
       fprintf(stderr,"usage: %s <hostname> <port>\n", argv[0]);
       exit(0);
    }
    hostname = argv[1];
    portno = atoi(argv[2]);

    /* socket: create the socket */
    /*sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");*/

    /* gethostbyname: get the server's DNS entry */
    /*server = gethostbyname(hostname);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host as %s\n", hostname);
        exit(0);
    }*/

    /* build the server's Internet address */
    /*bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    bcopy((char *)server->h_addr_list[0], 
      (char *)&serveraddr.sin_addr.s_addr, server->h_length);
    serveraddr.sin_port = htons(portno);*/

    /* connect: create a connection with the server */
    /*if (connect(sockfd, &serveraddr, sizeof(serveraddr)) < 0) 
      error("ERROR connecting");*/

    input_interface();

    /*printf("Main-Prompt > ");
    bzero(buf, BUFSIZE);
    bzero(buf2, BUFSIZE);*/
    /*bzero(buf3, BUFSIZE);*/
    /* get message line from the user */
    /*fgets(buf3, BUFSIZE, stdin);*/
    /*printf("Main-Prompt > ");*/
    /*char rep = 'y';
    while (rep == 'y')
    {
        int i=0;
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
        }
        strcat(buf,buf2);
    }*/
    /* send the message line to the server */
    /*n = write(sockfd, buf, strlen(buf));
    if (n < 0) 
      error("ERROR writing to socket");
    char *arg1, *arg2, *arg3;
    char *cmd = strtok(buf," ");
    int i=0;*/
    /*printf("Received:");*/
    /*while (cmd != NULL){
        printf(" %s", cmd );
        if(i == 0)
            arg1 = cmd;
        else if(i == 1)
            arg2 = cmd;
        else if(i == 2)
            arg3 = cmd;
        cmd = strtok(NULL, " ");
        i++;
    }
    printf("arg1 : %s,  arg2 : %s,  arg3 : %s \n",arg1,arg2,arg3);*/
    /*i = 0;
    int operatorlen = strlen(operator);*/
    /*convert operator string to lowercase, so that operater is no longer case sensitive*/
    /*while (i< operatorlen){
        if(operator[i] >= 65 && operator[i]<=90)
            operator[i] = operator[i] + 32;
        i++;
    }*/

    /* print the server's reply */
    /*bzero(buf, BUFSIZE);
    n = read(sockfd, buf, BUFSIZE);
    if (n < 0) 
      error("ERROR reading from socket");
    printf("Echo from server: %s", buf);*/
    /*close(sockfd);*/
    return 0;
}