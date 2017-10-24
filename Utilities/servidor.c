#include "socketUtils.h"
#define MSG_WELCOME "Benvingut al servidor!\nEscriu el que vulguis, que t'ho retornare! :)\n"

int sockfd;

void signalHandler(int signum) {
    switch (signum) {
		case SIGINT:
            close(sockfd);
            write(1,MSG_EXIT_SRV,strlen(MSG_EXIT_SRV));
            exit(0);
		break;
        default:
            write (1, ERR_INT, strlen(ERR_INT));
        break;
    }
}

void* connectionHandler(void* arg){

    ClientInfo client = *(ClientInfo*)arg;
    char *buff = NULL;

    write(client.socket , MSG_WELCOME , strlen(MSG_WELCOME));

    while(readDynamic(&buff,client.socket) > 0){
        write(1,"M'has dit...\n - ",strlen("M'has dit...\n - "));
        write(1,buff,strlen(buff));
        write(1," - \n",strlen(" - \n"));
        free(buff);
        buff = NULL;
    }
    buff = (char*)malloc(sizeof(char)*strlen(MSG_EXIT_CLIENT)+25);
    sprintf(buff,MSG_EXIT_CLIENT,inet_ntoa (client.c_addr.sin_addr),ntohs (client.c_addr.sin_port));
    write(1,buff,strlen(buff));

    close (client.socket);
    return NULL;
}

int main (int argc, char *argv[])
{
    char* port = NULL;
    int sockfd;

    signal(SIGINT, signalHandler);

    if (argc < 2)
    {
        fprintf (stderr, "Error: falta especificar l'arxiu de configuració\n");
        exit (EXIT_FAILURE);
    }

    if(readServerConfigFile(&port , argv[1])){
        write(1,ERR_FILE,strlen(ERR_FILE));
        exit (EXIT_FAILURE);
    }

    parallelHandler(atoi(port),connectionHandler,NULL, &sockfd);

    return EXIT_SUCCESS;
}
