//task for Watchdog thread
//The fourth thread (Watchdog) tries to save the program from any deadlocks an other errors. If threads do not send any message to Watchdog in 2s, Watchdog should finish the application with the proper error message

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>	//lib for threads
#include "tieto_cpu_tracker.h"	//header file for this project
#include <unistd.h>	//lib for sleep function

void* WatchdogTask(void*)
{
	
	while(!terminationRequest)
	{
		sleep(2);	//Watchdog checks for 2 seconds, outside of lock not to stall application
		pthread_mutex_lock(&mutexBuffer);	//locking thread into mutex to solve pcp
		
		if(watchdogFlags[0] == 0)	//checking if Reader hasn't sent the message
		{
			perror("Watchdog found a deadlock on Reader thread");
			terminationRequest = 1;
		}
		if(watchdogFlags[1] == 0)	//checking if Analyzer hasn't sent the message
		{
			perror("Watchdog found a deadlock on Analyzer thread");
			terminationRequest = 1;
		}
		if(watchdogFlags[2] == 0)	//checking if Printer hasn't sent the message
		{
			perror("Watchdog found a deadlock on Printer thread");
			terminationRequest = 1;
		}
		//Reseting all the flags to start the wait once again
		watchdogFlags[0] = 0;
		watchdogFlags[1] = 0;
		watchdogFlags[2] = 0;
		pthread_mutex_unlock(&mutexBuffer);	//locking thread into mutex to solve pcp
	
	
	}
	pthread_exit(NULL);
}
