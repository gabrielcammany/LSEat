
/**
 * Gabriel Cammany Ruiz - Manel Manchon Gasco
 * ls30652 - ls31343
 */
#include "include/controller.h"


int main (int argc, char *argv[])
{
    Data data;

    signal(SIGINT, control_signalHandler);

    if (argc < 1)
    {
        write(1,ERR_ARGS,strlen(ERR_ARGS));
		exit(EXIT_FAILURE);
    }

	basic_startValues(&data);

    if(basic_readDataConfig(argv[1],&data) < 0){
        write(1,ERR_FILE,strlen(ERR_FILE));
		exit(EXIT_FAILURE);
    }

    write(1, EXE, strlen(EXE));

	return control_executeData(&data);

}