//UNIT test, it runs the whole program like tieto_cpu_tracker.c, but during runtime it tests the correctness of modules working

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>	//lib for threads
#include <semaphore.h>	//lib for semaphores to use un producer-consumer-problem
#include <signal.h>	//lib for handling signals
#include "tieto_cpu_tracker.h"	//header file for this project
#include <assert.h>	//lib for automatic tests done with the help of asserts
#include <unistd.h>	//lib for sleep function

struct cpuStatisticsStruct cpuStatistics[maximumCpuNumber];	//array of structures defined inheader file to be used as buffer-shared memmory between threads
int cpuNumber = 0;	//number of cpus in shared memmory - cpuStatistics
pthread_mutex_t mutexBuffer;	//mutex for producer-consumer problem solution
sem_t producerSemaphore;	//creating a semaphore for reader thread (producer)
sem_t consumerSemaphore;	//creating a semaphore for analyzer thread (consumer)
unsigned long long cpuUsageSumm[maximumCpuNumber] = {0};	//array to act as buffer containing summ of multiple sets of cpu usage statistics of all processors to be later devided by cpuUsageSetsNumber during the printer thread to present avarage cpu usage for 1s
int cpuUsageSetsNumber = -1;	//number of sets of data aggregated during 1second, starts with -1 because the first measurment set is alone and thus cannot be used to calculate percantage usage, we need to compare two sets

volatile sig_atomic_t terminationRequest = 0;	//atomic flag, it is set to 1 to start closing the application gently

int main()
{	
	if (signal(SIGTERM, SigtermHandler) == SIG_ERR)
	{
        perror("Error installing SIGTERM handler");
        return EXIT_FAILURE;
	}
	pthread_t threadReader, threadAnalyzer, threadPrinter;
	int retReader, retAnalyzer, retPrinter;	//values as output of creating threads to check for success in their creation
	pthread_mutex_init(&mutexBuffer,NULL);	//initializing mutex buffer for pcp
	sem_init(&producerSemaphore, 0, 1);
	sem_init(&consumerSemaphore, 0, 0);
	

	retReader = pthread_create( &threadReader, NULL, ReaderTask, NULL);	//creating reader thread, second atribute is NULL, ReaderTask is a function this thread accomplishes
	if (retReader != 0)
	{
		perror("Problem encountered while creating Reader Thread\n"); sem_post(&loggerSemaphore); loggerMessage = 4; terminationRequest = 1;
	}
	retAnalyzer = pthread_create( &threadAnalyzer, NULL, AnalyzerTask, NULL);	//creating analyzer thread, second atribute is NULL, AnalyzerTask is a function this thread accomplishes
	if (retAnalyzer != 0)
	{
		perror("Problem encountered while creating Analyzer Thread\n"); sem_post(&loggerSemaphore); loggerMessage = 5; terminationRequest = 1;
	}
	retPrinter = pthread_create( &threadPrinter, NULL, PrinterTask, NULL);	//creatin printer thread, second atribute is NULL, PrinterTask is a function this thread accomplishes
	if (retPrinter != 0)
	{
		perror("Problem encountered while creating Printer Thread\n"); sem_post(&loggerSemaphore); loggerMessage = 6; terminationRequest = 1;
	}
	retWatchdog = pthread_create( &threadWatchdog, NULL, WatchdogTask, NULL);	//creatin Watchdog thread, second atribute is NULL, WatchdogTask is a function this thread accomplishes
	if (retWatchdog != 0)
	{
		perror("Problem encountered while creating Watchdog Thread\n"); sem_post(&loggerSemaphore); loggerMessage = 7; terminationRequest = 1;
	}
	retLogger = pthread_create( &threadLogger, NULL, LoggerTask, NULL);	//creatin Logger thread, second atribute is NULL, LoggerTask is a function this thread accomplishes
	if (retLogger != 0)
	{
		perror("Problem encountered while creating Logger Thread\n"); sem_post(&loggerSemaphore); loggerMessage = 8; terminationRequest = 1;
	}
	
	sleep(2);	//sleeping for two seconds to let the threads copleat initial data read and exchange and not to do tests on empty memory 
	int turnOfAfterIterations = 0;
	while(!terminationRequest)//loop waiting for termination signal to exit, untill then repeats to let threads work
	{
		pthread_mutex_lock(&mutexBuffer);	//locking the tests into mutex beacause they cannot check perform memory result control during the brief moment that memory is being manipulated
		
		//testing the Reader thread for if it got correct data and CPU total statistics is greater or equal than first CPU core statistics (which it should because it is a total of all cores)
		assert(
		cpuStatistics[0].user >= cpuStatistics[1].user &&
		cpuStatistics[0].nice >= cpuStatistics[1].nice &&
		cpuStatistics[0].system >= cpuStatistics[1].system &&
		cpuStatistics[0].idle >= cpuStatistics[1].idle &&
		cpuStatistics[0].iowait >= cpuStatistics[1].iowait
		);
		
		
		pthread_mutex_unlock(&mutexBuffer);	//unlocking thread from mutex
		sleep(1);
		turnOfAfterIterations +=1;
		if(turnOfAfterIterations == 5)
		{
			terminationRequest=1;
		}
	}	
	
	pthread_join( threadReader, NULL);	//terminating thread
	//printf("Reader Thread Terminated\n");	//checking for thread termination
	
	pthread_join( threadAnalyzer, NULL);	//terminating thread
	//printf("Analyzer Thread Terminated\n");	//checking for thread termination

	pthread_join( threadPrinter, NULL);	//terminating thread	
	
	//checking for thread termination
	sem_destroy(&producerSemaphore);
	sem_destroy(&consumerSemaphore);
	//system("clear");	//clearing screen for improved user interface
	printf("Gracefully exited the program by sigterm handler\n");	
	pthread_mutex_destroy(&mutexBuffer);	//destroing mutex buffer for pcp
	return EXIT_SUCCESS;
}
