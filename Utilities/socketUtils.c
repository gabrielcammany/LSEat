#include "socketUtils.h"

int readClientConfigFile(char** ip, char** port, char* fileName){

    int fd = 0;

    fd=open(fileName, O_RDONLY);

    if (fd<0){
        return 1;
    }

    if(readDynamic(ip, fd) < 0 || readDynamic(port, fd) < 0){
        return 1;
    }

    return 0;

}

int createConnectionClient(int portInput, char* ipInput){

    // comprovem la validesa del port
    uint16_t port;
    if (portInput < 1 || portInput > 65535)
    {
        fprintf (stderr, ERR_PORT, portInput);
        exit (EXIT_FAILURE);
    }
    port = portInput;
    // comprovem la validesa de l'adre￿a IP
    // i la convertim a format binari
    struct in_addr ip_addr;
    if (inet_aton (ipInput, &ip_addr) == 0)
    {
        fprintf (stderr, ERR_ATON, ipInput, strerror (errno));
        exit (EXIT_FAILURE);
    }
    // creem el socket
    int sockfd;
    sockfd = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd < 0)
    {
        perror ("socket TCP");
        exit (EXIT_FAILURE);
    }
    // especifiquem l'adre￿a del servidor
    struct sockaddr_in s_addr;
    //memset(&s_addr, '\0', sizeof(s_addr));
    bzero (&s_addr, sizeof (s_addr));
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons (port);
    s_addr.sin_addr = ip_addr;
    // i ara ja ens podem connectar
    // al cridar connect, hem de fer un cast:
    // bind espera struct sockaddr *
    // i nosaltres passem struct sockaddr_in *
    if (connect (sockfd, (void *) &s_addr, sizeof (s_addr)) < 0)
    {
        perror ("connect");
        exit (EXIT_FAILURE);
    }
    return sockfd;
}

int createConnectionServer(int portInput){

    // comprovem la validesa del port
    uint16_t port;
    if (portInput < 1 || portInput > 65535)
    {
        fprintf (stderr, ERR_PORT, portInput);
        exit (EXIT_FAILURE);
    }
    port = portInput;

    // creem el socket
    int sockfd;
    sockfd = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd < 0)
    {
        perror ("socket TCP");
        exit (EXIT_FAILURE);
    }
    // especifiquem l'adre￿a que volem vincular al nostre socket
    // admetrem connexions dirigides a qualsevol IP de la nostra m￿quina
    // al port especificat per l￿nia de comandes
    struct sockaddr_in s_addr;
    bzero (&s_addr, sizeof (s_addr));
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons (port);
    s_addr.sin_addr.s_addr = INADDR_ANY;
    // al cridar bind, hem de fer un cast:
    // bind espera struct sockaddr *
    // i nosaltres passem struct sockaddr_in *
    if (bind (sockfd, (void *) &s_addr, sizeof (s_addr)) < 0)
    {
        perror ("bind");
        exit (EXIT_FAILURE);
    }
    // ara obrim el port, 5 ￿s un valor t￿pic
    listen (sockfd, 5);
    return sockfd;
}

int readServerConfigFile(char** port, char* fileName){

    int fd = 0;

    fd=open(fileName, O_RDONLY);

    if (fd<0){
        return 1;
    }

    if(readDynamic(port, fd) < 0){
        return 1;
    }

    return 0;
}


void serialHandler(int port, void *(*handler) (void *),void *arg, int *sockfd){
    char* buff = NULL;
    *sockfd = createConnectionServer(port);

    while (1)
    {
        struct sockaddr_in c_addr;
        socklen_t c_len = sizeof (c_addr);

        int newsock = accept (*sockfd, (void *) &c_addr, &c_len);
        if (newsock < 0)
        {
            perror ("accept");
            exit (EXIT_FAILURE);
        }
        ClientInfo client;
        client.c_addr = c_addr;
        client.socket = newsock;
        client.arg = (void*) &arg;

        buff = (char*)malloc(sizeof(char)*strlen(MSG_NEW)+25);
        sprintf(buff,MSG_NEW,inet_ntoa (client.c_addr.sin_addr),ntohs (client.c_addr.sin_port));
        write(1,buff,strlen(buff));


        handler((void*) &client);
    }
}

void parallelHandler(int port, void *(*handler) (void *),void *arg, int *sockfd){
    char* buff = NULL;
    *sockfd = createConnectionServer(port);
    pthread_t thread_id;

    while (1)
    {
        struct sockaddr_in c_addr;
        socklen_t c_len = sizeof (c_addr);

        int newsock = accept (*sockfd, (void *) &c_addr, &c_len);
        if (newsock < 0)
        {
            perror ("accept");
            exit (EXIT_FAILURE);
        }

        ClientInfo client;
        client.c_addr = c_addr;
        client.socket = newsock;
        client.arg = (void*) &arg;

        buff = (char*)malloc(sizeof(char)*strlen(MSG_NEW)+25);
        sprintf(buff,MSG_NEW,inet_ntoa (client.c_addr.sin_addr),ntohs (client.c_addr.sin_port));
        write(1,buff,strlen(buff));


        if( pthread_create( &thread_id , NULL ,  handler , (void*) &client) < 0)
        {
            perror("could not create thread");
            close (newsock);
        }
        /*if( pthread_create( &thread_id , NULL ,  handler , (void*) &newsock) < 0)
        {
            return 1;
        }*/
    }
}
