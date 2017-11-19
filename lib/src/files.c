/**
 * @Author: Manel Manchón Gascó / Gabriel Cammany Ruiz
 * @Date:   24-10-2017
 * @Email:  ls31343@salleurl.edu ls30652@salleurl.edu
 * @Project: Práctica LSEat
 * @Filename: client.c
 * @Last modified by:   Manel Manchón Gascó / Gabriel Cammany Ruiz
 * @Last modified time: 26-10-2017
 */

#include "../include/files.h"

int openFile(char *name, int status) {
	int fd = 0;

	switch (status) {
		case 1:
			fd = open(name, O_RDONLY);
			break;
		case 2:
			fd = open(name, O_RDWR);
			break;
		case 3:
			fd = open(name, O_RDWR | O_APPEND | O_CREAT, 0666);
			break;
		case 4:
			fd = open(name, O_RDWR | O_APPEND);
			break;
		default:
			return ERROR_CODE;
	}

	if (fd < 0) {
		return ERROR_CODE;
	}
	return fd;
}

int moveToStart(int fd) {
	return (int) lseek(fd, 0, SEEK_SET);
}

int checkEmpty(int fd) {
	return (int) lseek(fd, 1, SEEK_CUR);
}
