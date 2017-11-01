//
// Created by gabriel on 31/10/17.
//

#include "connectionManager.h"
#include "../../modules/types.h"


int connectToEnterprise(ClientLSEat *lsEat, char *configFile){
	int socket = 0, error = 0;

	error = readClientConfig(configFile,lsEat);
	if(error < 0){
		exit(EXIT_FAILURE);
	}
	socket = createConnectionClient(lsEat->config.Port,lsEat->config.IP);
	if(socket < 0){
		write(1,ERR_CONNECTION,strlen(ERR_CONNECTION));
		exit(EXIT_FAILURE);
	}
	return socket;
}