/**
 * @Author: Manel Manchón Gascó / Gabriel Cammany Ruiz
 * @Date:   24-10-2017
 * @Email:  ls31343@salleurl.edu ls30652@salleurl.edu
 * @Project: Practica LSEat
 * @Filename: basic.h
 * @Last modified by:   Manel Manchón Gascó / Gabriel Cammany Ruiz
 * @Last modified time: 27-10-2017
 */

#include "../include/basic.h"
#include "../../lib/include/network.h"

void BASIC_freeMemory() {
	int i;

	if(enterprise.config.picard_ip != NULL)free(enterprise.config.picard_ip);
	if(enterprise.config.data_ip != NULL)free(enterprise.config.data_ip);
	if(enterprise.config.data_port != NULL)free(enterprise.config.data_port);
	if(enterprise.config.picard_port != NULL)free(enterprise.config.picard_port);
	if(enterprise.restaurant.name != NULL)free(enterprise.restaurant.name);

	if(enterprise.restaurant.menu != NULL){

		if(enterprise.restaurant.num_menu > 0){

			for(i = 0; i< enterprise.restaurant.num_menu; i++){

				free(enterprise.restaurant.menu[i].name);

			}

			free(enterprise.restaurant.menu);

		}else{

			write(1,ERR_MEMORY,strlen(ERR_MEMORY));
			free(enterprise.restaurant.menu);

		}
	}

}

int BASIC_readNumber(int fd, int *extractNumber) {
    int error = 0;
    char *number = NULL;

    error = UTILS_readDynamic(&number, fd);

    if (error < 1) {
        return ERROR_CODE;
    }

    *extractNumber = (unsigned int)atoi(number);
    return 1;

}

int BASIC_readConfigEnterprise(char *fitxer, Enterprise *enterprise) {
    int fd = 0;

    fd = FILES_openFile(fitxer, 1);
    if (fd < 0) {
        return ERROR_CODE;
    }
    //We read the restaurant name
    if (UTILS_readDynamic(&enterprise->restaurant.name, fd) < 0) {
        write(1, ERR_ENTNAME, strlen(ERR_ENTNAME));
        close(fd);
        return ERROR_CODE;
    }

    //we read restaurant seconds to refresh data info
    if (BASIC_readNumber(fd, &(enterprise->restaurant.seconds)) == ERROR_CODE) {
        write(1, ERR_SEC, strlen(ERR_SEC));
        close(fd);
        return ERROR_CODE;
    }

    //we read IP to connect to data
    if (UTILS_readDynamic(&enterprise->config.data_ip, fd) < 0) {
        write(1, ERR_IP, strlen(ERR_IP));
        close(fd);
        return ERROR_CODE;
    }

    //We read the datas port
    if (UTILS_readDynamic(&enterprise->config.data_port, fd) == ERROR_CODE) {
        write(1, ERR_PORT, strlen(ERR_PORT));
        close(fd);
        return ERROR_CODE;
    }

    //same as above but from where picards will connect
    if (UTILS_readDynamic(&enterprise->config.picard_ip, fd) < 0) {
        write(1, ERR_IP, strlen(ERR_IP));
        close(fd);
        return ERROR_CODE;
    }

    if (UTILS_readDynamic(&enterprise->config.picard_port, fd) == ERROR_CODE) {
        write(1, ERR_PORT, strlen(ERR_PORT));
        close(fd);
        return ERROR_CODE;
    }

    close(fd);

    return 1;
}

int BASIC_readMenu(char *menu, Enterprise *enterprise){
	int fd = 0, i = 0;

	fd = FILES_openFile(menu, 1);

	if(fd < 0){

		return ERROR_CODE;
	}

	enterprise->restaurant.menu = (Dish*) malloc (sizeof(Dish));

	//because this functions returns the number of bytes we read
	//we can know when its end of file
	while(UTILS_readDynamic(&enterprise->restaurant.menu[i].name, fd) > 0 ) {

		if(BASIC_readNumber(fd, &enterprise->restaurant.menu[i].units) < 0){return ERROR_CODE;}

		if(BASIC_readNumber(fd, &enterprise->restaurant.menu[i].price) < 0){return ERROR_CODE;}

		i++;
		enterprise->restaurant.menu = (Dish*) realloc (enterprise->restaurant.menu, sizeof(Dish)*(i+1));
	}

	write(1, MENU_READY, strlen(MENU_READY));
	enterprise->restaurant.num_menu = i;
	close(fd);

	return 0;
}

void BASIC_welcomeMessage(Enterprise enterprise){
	write (1, WELCOME, strlen(WELCOME));
	write (1, enterprise.restaurant.name, strlen(enterprise.restaurant.name));
	write (1, "\n\0", sizeof(char)*2);
}

void BASIC_startValues(){

	enterprise.restaurant.menu = NULL;
	enterprise.restaurant.num_menu = 0;
	enterprise.restaurant.name = NULL;
	enterprise.restaurant.seconds = 0;

	enterprise.config.data_ip = NULL;
	enterprise.config.data_port = NULL;
	enterprise.config.picard_port = NULL;
	enterprise.config.picard_ip = NULL;

	enterprise.clients = NULL;
	enterprise.num_clients = 0;


}