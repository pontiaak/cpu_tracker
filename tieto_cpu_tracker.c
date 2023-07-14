//Main function for cpu tracker app

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>	//lib for threads
#include <semaphore.h>	//lib for semaphores to use un producer-consumer-problem
#include <signal.h>	//lib for handling signals
#include "tieto_cpu_tracker.h"	//header file for this project

struct cpuStatisticsStruct cpuStatistics[maximumCpuNumber];	//array of structures defined inheader file to be used as buffer-shared memmory between threads
int cpuNumber = 0;	//number of cpus in shared memmory - cpuStatistics
pthread_mutex_t mutexBuffer;	//mutex for producer-consumer problem solution
sem_t producerSemaphore;	//creating a semaphore for reader thread (producer)
sem_t consumerSemaphore;	//creating a semaphore for analyzer thread (consumer)
unsigned long long cpuUsageSumm[maximumCpuNumber] = {0};	//array to act as buffer containing summ of multiple sets of cpu usage statistics of all processors to be later devided by cpuUsageSetsNumber during the printer thread to present avarage cpu usage for 1s
int cpuUsageSetsNumber = -1;	//number of sets of data aggregated during 1second, starts with -1 because the first measurment set is alone and thus cannot be used to calculate percantage usage, we need to compare two sets
int watchdogFlags[4] = {0};	//the threads will need to set the corresponding [i] flag to 1 every 2 seconds to show the watchdog that they are active
volatile sig_atomic_t terminationRequest = 0;	//atomic flag, it is set to 1 to start closing the application gently

int main()
{	
	if (signal(SIGTERM, SigtermHandler) == SIG_ERR)
	{
        perror("Error installing SIGTERM handler");
        return EXIT_FAILURE;
	}
	if (signal(SIGINT, SigtermHandler) == SIG_ERR)
	{
        perror("Error installing SIGTERM handler");
        return EXIT_FAILURE;
	}
	pthread_t threadReader, threadAnalyzer, threadPrinter, threadWatchdog;
	int retReader, retAnalyzer, retPrinter, retWatchdog;	//values as output of creating threads to check for success in their creation
	pthread_mutex_init(&mutexBuffer,NULL);	//initializing mutex buffer for pcp
	sem_init(&producerSemaphore, 0, 1);
	sem_init(&consumerSemaphore, 0, 0);
	

	retReader = pthread_create( &threadReader, NULL, ReaderTask, NULL);	//creating reader thread, second atribute is NULL, ReaderTask is a function this thread accomplishes
	if (retReader != 0){printf("Problem encountered while creating Reader Thread\n"); return EXIT_FAILURE;}; /*else printf("Reader Thread Created Successfully\n");*/ 
	retAnalyzer = pthread_create( &threadAnalyzer, NULL, AnalyzerTask, NULL);	//creating analyzer thread, second atribute is NULL, AnalyzerTask is a function this thread accomplishes
	if (retAnalyzer != 0){printf("Problem encountered while creating Analyzer Thread\n"); return EXIT_FAILURE;}; /*else printf("Analyzer Thread Created Successfully\n");*/
	retPrinter = pthread_create( &threadPrinter, NULL, PrinterTask, NULL);	//creatin printer thread, second atribute is NULL, PrinterTask is a function this thread accomplishes
	if (retPrinter != 0){printf("Problem encountered while creating Printer Thread\n"); return EXIT_FAILURE;}; /*else printf("Printer Thread Created Successfully\n");*/
	retWatchdog = pthread_create( &threadWatchdog, NULL, WatchdogTask, NULL);	//creatin Watchdog thread, second atribute is NULL, WatchdogTask is a function this thread accomplishes
	if (retWatchdog != 0){printf("Problem encountered while creating Watchdog Thread\n"); return EXIT_FAILURE;}; /*else printf("Printer Thread Created Successfully\n");*/
	
	while(!terminationRequest){}	//loop waiting for termination signal to exit, untill then repeats to let threads work
	
	pthread_join( threadReader, NULL);	//terminating thread
	pthread_join( threadAnalyzer, NULL);	//terminating thread
	pthread_join( threadPrinter, NULL);	//terminating thread
	pthread_join( threadWatchdog, NULL);	//terminating thread
	
	//checking for thread termination
	sem_destroy(&producerSemaphore);
	sem_destroy(&consumerSemaphore);
	printf("\nGracefully exited the program by sigterm handler\n");	
	pthread_mutex_destroy(&mutexBuffer);	//destroing mutex buffer for pcp
	return EXIT_SUCCESS;
}
