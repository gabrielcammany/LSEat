#include "socketUtils.h"

int sockfd;

void signalHandler(int signum) {
    switch (signum) {
		case SIGINT:
            close(sockfd);
            write(1,MSG_EXIT,strlen(MSG_EXIT));
            exit(0);
		break;
        default:
            write (1, ERR_INT, strlen(ERR_INT));
        break;
    }
}


void readMenu(int socket){
    char *buff = NULL;
    readDynamic(&buff,socket);
    write(1,buff,strlen(buff));
    write(1,"\n",1);
    readDynamic(&buff,socket);
    write(1,buff,strlen(buff));
    write(1,"\n",1);
}



int main (int argc, char *argv[])
{
    char* ip = NULL, *port = NULL, *buff = NULL;
    signal(SIGINT, signalHandler);

    if (argc < 2)
    {
        fprintf (stderr, "Error: falta especificar l'arxiu de configuració\n");
        exit (EXIT_FAILURE);
    }

    if(readClientConfigFile(&ip,&port,argv[1])){
        write(1,ERR_FILE,strlen(ERR_FILE));
        exit (EXIT_FAILURE);
    }

    sockfd = createConnectionClient(atoi(port),ip);

    readMenu(sockfd);

    readDynamic(&buff,0);
    while(strcmp(buff,"Sortir")!=0){
        write(sockfd,buff,strlen(buff));
        write(sockfd,"\n",1);

        write(1,"Has enviat\n - ",strlen("Has enviat\n - "));
        write(1,buff,strlen(buff));
        write(1," - \n",strlen(" - \n"));

        readDynamic(&buff,0);
    }

    close(sockfd);
    return EXIT_SUCCESS;
}
