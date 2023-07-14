//Task for Analyzer thread
//The second thread (Analyzer) works on raw data and produces CPU usage (in percentages) for each CPU core from /proc/stat. Then thread sends CPU usage to the third thread (Printer)

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>	//lib for threads
#include "tieto_cpu_tracker.h"	//header file for this project
#include <unistd.h>	//lib for sleep function

	

void* AnalyzerTask(){
	
	unsigned long long idle, nonIdle, total, totald, idled, cpuPercentage;
	unsigned long long prevIdle[maximumCpuNumber], prevTotal[maximumCpuNumber];	//these are set to maximumCpuNumber and not cpuNumber not to cause problems
	
	while(!terminationRequest){	//while loop to keep thread functioning
		sem_wait(&consumerSemaphore);	//waiting until reader thread finishes reading all cpu's data, writes it to the buffer structure and thus, incriments this semaphore
		pthread_mutex_lock(&mutexBuffer);	//locking thread into mutex to solve pcp
		
		
		for (int i = 0; i < cpuNumber; i++) {
			//in case you want to see deatailed statistics for each cpu
			/*printf("CPU %d:   user: %llu   ", i-1,cpuStatistics[i].user);
			printf("User: %llu\n", cpuStatistics[i].user);
			printf("Nice: %llu\n", cpuStatistics[i].nice);
			printf("System: %llu\n", cpuStatistics[i].system);
			printf("Idle: %llu\n", cpuStatistics[i].idle);
			printf("iowait: %llu\n", cpuStatistics[i].iowait);
			printf("irq: %llu\n", cpuStatistics[i].irq);
			printf("softirq: %llu\n", cpuStatistics[i].softirq);
			printf("steal: %llu\n", cpuStatistics[i].steal);
			printf("guest: %llu\n", cpuStatistics[i].guest);
			("guest_nice: %llu\n", cpuStatistics[i].guest_nice);
			printf("\n");*/
				
			idle = cpuStatistics[i].idle + cpuStatistics[i].iowait;
			nonIdle = cpuStatistics[i].user + cpuStatistics[i].nice + cpuStatistics[i].system + cpuStatistics[i].irq + cpuStatistics[i].softirq + cpuStatistics[i].steal;
			total = idle + nonIdle;
			
			if(prevIdle[i]==0 || prevTotal[i]==0)	//this scenario means this is a first time and we have only one set of data, and thus need to skip
			{
				//we skip the first set of data because it has nothing to compare to
				prevIdle[i] = idle;
				prevTotal[i] = total;
			}else{
				totald = total - prevTotal[i];
				idled = idle - prevIdle[i];
				cpuPercentage = (double)(totald - idled) / totald*100;
				prevIdle[i] = idle;
				prevTotal[i] = total;
				cpuUsageSumm[i] += cpuPercentage;	//array to act as buffer containing summ of multiple sets of cpu usage statistics of all processors to be later devided by cpuUsageSetsNumber during the printer thread to present avarage cpu usage for 1s
			}
			

		}
		cpuUsageSetsNumber += 1;	//counts numbers of sets of data summed into cpuUsageSumm[] to later in printer devide it by this number to get avarage
		watchdogFlags[1]=1;	//flag for watchdog to know this thread is active, its inside mutex lock b.c. it's shared memory with watchdog
		pthread_mutex_unlock(&mutexBuffer);	//locking thread into mutex to solve pcp
		sem_post(&producerSemaphore);	//incrimenting semaphore, thus telling the wait() in reader thread that analyzing of this data packet is done and it can start to read next package of cpu data
		usleep(250000); //250 milliseconds - sleep function to being able to gather sets of cpu stats with interval
	}
	pthread_exit(NULL);
}
