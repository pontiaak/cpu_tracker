//Main function for cpu tracker app

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>	//lib for threads
#include <semaphore.h>	//lib for semaphores to use un producer-consumer-problem
#include "tieto_cpu_tracker.h"	//header file for this project

struct cpuStatisticsStruct cpuStatistics[maximumCpuNumber];	//array of structures defined inheader file to be used as buffer-shared memmory between threads
int cpuNumber = 0;	//number of cpus in shared memmory - cpuStatistics
pthread_mutex_t mutexBuffer;	//mutex for producer-consumer problem solution
sem_t producerSemaphore;	//creating a semaphore for reader thread (producer)
sem_t consumerSemaphore;	//creating a semaphore for analyzer thread (consumer)
int stopFlag = 0; // Flag to control the loop condition
unsigned long long cpuUsageSumm[maximumCpuNumber] = {0};	//array to act as buffer containing summ of multiple sets of cpu usage statistics of all processors to be later devided by cpuUsageSetsNumber during the printer thread to present avarage cpu usage for 1s
int cpuUsageSetsNumber = -1;	//number of sets of data aggregated during 1second, starts with -1 because the first measurment set is alone and thus cannot be used to calculate percantage usage, we need to compare two sets

int main()
{	
	pthread_t threadReader, threadAnalyzer, threadPrinter;
	int retReader, retAnalyzer, retPrinter;	//values as output of creating threads to check for success in their creation
	pthread_mutex_init(&mutexBuffer,NULL);	//initializing mutex buffer for pcp
	sem_init(&producerSemaphore, 0, 1);
	sem_init(&consumerSemaphore, 0, 0);
	

	retReader = pthread_create( &threadReader, NULL, ReaderTask, NULL);	//creating reader thread, second atribute is NULL, ReaderTask is a function this thread accomplishes
	if (retReader != 0) printf("Problem encountered while creating Reader Thread\n"); /*else printf("Reader Thread Created Successfully\n");*/ 
	retAnalyzer = pthread_create( &threadAnalyzer, NULL, AnalyzerTask, NULL);	//creating analyzer thread, second atribute is NULL, AnalyzerTask is a function this thread accomplishes
	if (retAnalyzer != 0) printf("Problem encountered while creating Analyzer Thread\n"); /*else printf("Analyzer Thread Created Successfully\n");*/
	retPrinter = pthread_create( &threadPrinter, NULL, PrinterTask, NULL);	//creatin printer thread, second atribute is NULL, PrinterTask is a function this thread accomplishes
	if (retPrinter != 0) printf("Problem encountered while creating Printer Thread\n"); /*else printf("Printer Thread Created Successfully\n");*/ 
	
	while(1){}	//temporary loop until better way is implimented
	
	pthread_join( threadReader, NULL);	//terminating thread
	printf("Reader Thread Terminated\n");	//checking for thread termination

	
	pthread_join( threadAnalyzer, NULL);	//terminating thread
	printf("Analyzer Thread Terminated\n");	//checking for thread termination

	
	pthread_join( threadPrinter, NULL);	//terminating thread
	printf("Printer Thread Terminated\n");	//checking for thread termination
	
	pthread_exit(NULL);
	
	sem_destroy(&producerSemaphore);
	sem_destroy(&producerSemaphore);
	pthread_mutex_destroy(&mutexBuffer);	//destroing mutex buffer for pcp
	return 0;

}
