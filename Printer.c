//Task for Printer thread
//The third thread (Printer) prints to the console (in format way, the format is up to you) average CPU usage in every one second. (You cannot print in per received data, you need to aggregate data and print only avg from 1s)

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>	//lib for threads
#include "tieto_cpu_tracker.h"	//header file for this project
#include <unistd.h>	//lib for sleep function

void* PrinterTask(){

	double cpuUsageAvarage;	
	
	while(!terminationRequest){	//while loop to keep thread functioning
		sleep(1);
		system("clear");	//clearing screen for improved user interface
		pthread_mutex_lock(&mutexBuffer);	//locking thread into mutex to solve pcp
		
		for (int i = 0; i < cpuNumber; i++)
		{
			cpuUsageAvarage = (double)cpuUsageSumm[i] / cpuUsageSetsNumber;	//calculation of avarage cpu usage by dividing all the gathered and summed cpu usage statistics from shared data by the number of those sets of statistics
			if(i == 0)
			{
				printf("\tCPU tracker app\n average CPU usage in last 1s (%dsets aggregated)\nCPU Total:\t%.2f%%\n",cpuUsageSetsNumber,cpuUsageAvarage);
			}else{
				printf("CPU Core %d:\t%.2f%%\n",i-1,cpuUsageAvarage);
			}
		}
		printf("\n exit gently by pkill or ctrl+c\n");
		cpuUsageSetsNumber=0;	//cleaning because this variable stores how many times we received a set of statistics from analyzer for each second, thus after this second we need to calculate anew for consecutive second
		for (int i = 0; i < cpuNumber; i++)	//cleaning shared memory buffer storing summed up cpu usage statistics because we need to calculate anew for consecutive second
		{
			cpuUsageSumm[i] = 0;
		}
		
		
		pthread_mutex_unlock(&mutexBuffer);	//locking thread into mutex to solve pcp
	}
	pthread_exit(NULL);
}
