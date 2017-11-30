
/**
 * Gabriel Cammany Ruiz - Manel Manchon Gasco
 * ls30652 - ls31343
 */
#include "include/controller.h"


int main (int argc, char *argv[])
{

    signal(SIGINT, dControl_signalHandler);
	signal(SIGUSR1, dControl_signalHandler);

    if (argc != 2)
    {
        write(1,ERR_ARGS,strlen(ERR_ARGS));
		exit(EXIT_FAILURE);
    }

	basic_startValues(&data);

    if(basic_readDataConfig(argv[1],&data) == EXIT_FAILURE){
        write(1,ERR_FILE,strlen(ERR_FILE));
		exit(EXIT_FAILURE);
    }


	addEnterprise();

    write(1, EXE, strlen(EXE));

	dNetwork_executeData(data.enterprisePort, data.picardPort, data.ip);

	return EXIT_SUCCESS;

}