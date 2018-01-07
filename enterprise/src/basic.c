/**
 * @Author: Manel Manchón Gascó / Gabriel Cammany Ruiz
 * @Date:   24-10-2017
 * @Email:  ls31343@salleurl.edu ls30652@salleurl.edu
 * @Project: Practica LSEat
 * @Filename: basic.h
 * @Last modified by:   Manel Manchón Gascó / Gabriel Cammany Ruiz
 * @Last modified time: 27-10-2017
 */

#include "../include/controller.h"

void BASIC_freeMemory() {

	if(enterprise.config.picard_ip != NULL)free(enterprise.config.picard_ip);
	if(enterprise.config.data_ip != NULL)free(enterprise.config.data_ip);
	if(enterprise.config.data_port != NULL)free(enterprise.config.data_port);
	if(enterprise.config.picard_port != NULL)free(enterprise.config.picard_port);

	if(enterprise.restaurant.name != NULL)free(enterprise.restaurant.name);

	MSTRUCTURE_destruct(&enterprise.restaurant.menu);

	PSTRUCTURE_destruct(&enterprise.clients);

	pthread_mutex_destroy(&mtx);

}

int BASIC_readNumber(int fd, int *extractNumber) {
    int error = 0;
    char *number = NULL;

    error = UTILS_readDynamic(&number, fd);

    if (error < 1) {
        return ERROR_CODE;
    }

    *extractNumber = (unsigned int)atoi(number);
	free(number);

    return 1;

}

int BASIC_readConfigEnterprise(char *fitxer) {
    int fd = 0, error = 0;

    fd = FILES_openFile(fitxer, 1);
    if (fd < 0) {
        return ERROR_CODE;
    }

    //We read the restaurant name
    if (UTILS_readDynamic(&enterprise.restaurant.name, fd) < 0) {
        write(1, ERR_ENTNAME, strlen(ERR_ENTNAME));
		error = ERROR_CODE;
    }

    //we read restaurant seconds to refresh data info
    if (BASIC_readNumber(fd, &(enterprise.restaurant.seconds)) == ERROR_CODE) {
        write(1, ERR_SEC, strlen(ERR_SEC));
		error = ERROR_CODE;
    }

    //we read IP to connect to data
    if (UTILS_readDynamic(&enterprise.config.data_ip, fd) < 0) {
        write(1, ERR_IP, strlen(ERR_IP));
		error = ERROR_CODE;
    }

    //We read the datas port
    if (UTILS_readDynamic(&enterprise.config.data_port, fd) == ERROR_CODE) {
        write(1, ERR_PORT, strlen(ERR_PORT));
		error = ERROR_CODE;
    }

    //same as above but from where picards will connect
    if (UTILS_readDynamic(&enterprise.config.picard_ip, fd) < 0) {
        write(1, ERR_IP, strlen(ERR_IP));
		error = ERROR_CODE;
    }

    if (UTILS_readDynamic(&enterprise.config.picard_port, fd) == ERROR_CODE) {
        write(1, ERR_PORT, strlen(ERR_PORT));
		error = ERROR_CODE;
    }

    close(fd);

    return error;
}

int BASIC_readMenu(char *menu){
	int fd = 0, units = 0, price = 0;
	char* aux_key;

	fd = FILES_openFile(menu, 1);

	if(fd < 0){

		return ERROR_CODE;
	}

	aux_key = NULL;

	while(UTILS_readDynamic(&aux_key, fd) > 0 ) {

		if(BASIC_readNumber(fd, &units) < 0){return ERROR_CODE;}

		if(BASIC_readNumber(fd, &price) < 0){return ERROR_CODE;}
		aux_key = UTILS_toLower(aux_key);
		aux_key[strlen(aux_key)-1] = '\0';
		MSTRUCTURE_insert(&enterprise.restaurant.menu,MSTRUCTURE_createBucket(aux_key,price,units));

		free(aux_key);
		aux_key = NULL;

	}

	write(1, MENU_READY, strlen(MENU_READY));

	close(fd);

	return 0;
}

void BASIC_welcomeMessage(){
	write (1, WELCOME, strlen(WELCOME));
	write (1, enterprise.restaurant.name, strlen(enterprise.restaurant.name));
	write (1, "\n\0", sizeof(char)*2);
}

void BASIC_startValues(){

	enterprise.restaurant.menu = MSTRUCTURE_createStructure(MENU_SIZE);
	enterprise.restaurant.name = NULL;
	enterprise.restaurant.seconds = 0;
	enterprise.config.data_ip = NULL;
	enterprise.config.data_port = NULL;
	enterprise.config.picard_port = NULL;
	enterprise.config.picard_ip = NULL;

	enterprise.clients = PSTRUCTURE_createTable(MAX_PICARDS);
	enterprise.thread_data = 0;

	pthread_mutex_init(&mtx, NULL);

}