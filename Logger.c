//task for Logger thread
//The fifth thread (Logger) receives messages from all threads and writes them into a file. Logger is useful to write down debug prints in a synchronized way

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>	//lib for threads
#include "tieto_cpu_tracker.h"	//header file for this project
#include <unistd.h>	//lib for sleep function
#include <time.h>	//lib for getting current time in order to make logs more readible and usefull

void* LoggerTask(void*)
{

	while(!terminationRequest)
	{
		sem_wait(&loggerSemaphore);	//waiting until any of other threads posts this semaphore, thus waking it up and printing message into a file, then it will wait once again
		pthread_mutex_lock(&mutexBuffer);	//locking thread into mutex to solve pcp
		FILE* loggerFile = fopen(loggerFilePath, "a");	//opening the defined in header path to file in which the logs will be stored with atribute for appending ("a")
		time_t myTime = time(NULL);
		char * timeString = ctime(&myTime);
		timeString[strlen(timeString)-1] = '\0';
		switch(loggerMessage)
		{
			case 1: fprintf(loggerFile, "%s : application shut down gracefully\n",timeString); break;
			case 2: fprintf(loggerFile, "%s : application launched\n",timeString); break;
			case 3: fprintf(loggerFile, "%s : encountered Error installing SIGTERM handler, shutting down\n",timeString); break;
			case 4: fprintf(loggerFile, "%s : Problem encountered while creating Reader Thread, shutting down\n",timeString); break;
			case 5: fprintf(loggerFile, "%s : Problem encountered while creating Analyzer Thread, shutting down\n",timeString); break;
			case 6: fprintf(loggerFile, "%s : Problem encountered while creating Printer Thread, shutting down\n",timeString); break;
			case 7: fprintf(loggerFile, "%s : Problem encountered while creating Watchgod Thread, shutting down\n",timeString); break;
			case 8: fprintf(loggerFile, "%s : Problem encountered while creating Logger Thread, shutting down\n",timeString); break;
			case 9: fprintf(loggerFile, "%s : Reader thread failed to open /proc/stat, shutting down\n",timeString); break;
			case 10: fprintf(loggerFile, "%s : Received SIGINT\n",timeString); break;
		}
		
		pthread_mutex_unlock(&mutexBuffer);	//locking thread into mutex to solve pcp
		fclose(loggerFile);
	}
	pthread_exit(NULL);
}
