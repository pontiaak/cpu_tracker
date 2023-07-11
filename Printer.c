//Task for Printer thread
//The third thread (Printer) prints to the console (in format way, the format is up to you) average CPU usage in every one second. (You cannot print in per received data, you need to aggregate data and print only avg from 1s)

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>	//lib for threads
#include "tieto_cpu_tracker.h"	//header file for this project

void* PrinterTask(){
	pthread_mutex_lock(&mutexBuffer);	//locking thread into mutex to solve pcp
	
	pthread_mutex_unlock(&mutexBuffer);	//locking thread into mutex to solve pcp
	printf("PrinterTask Successfull\n");
	return 0;
}
