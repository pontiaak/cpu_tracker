//Task for Analyzer thread
//The second thread (Analyzer) works on raw data and produces CPU usage (in percentages) for each CPU core from /proc/stat. Then thread sends CPU usage to the third thread (Printer)

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>	//lib for threads
#include "tieto_cpu_tracker.h"	//header file for this project

void* AnalyzerTask(){
	while(!stopFlag){	//while loop to keep thread functioning
		sem_wait(&consumerSemaphore);	//waiting until reader thread finishes reading all cpu's data, writes it to the buffer structure and thus, incriments this semaphore
		pthread_mutex_lock(&mutexBuffer);	//locking thread into mutex to solve pcp
			for (int i = 0; i < cpuNumber; i++) {
		printf("CPU %d:\n", i);
		printf("User: %llu\n", cpuStatistics[i].user);
		printf("Nice: %llu\n", cpuStatistics[i].nice);
		printf("System: %llu\n", cpuStatistics[i].system);
		printf("Idle: %llu\n", cpuStatistics[i].idle);
		printf("iowait: %llu\n", cpuStatistics[i].iowait);
		printf("irq: %llu\n", cpuStatistics[i].irq);
		printf("softirq: %llu\n", cpuStatistics[i].softirq);
		printf("steal: %llu\n", cpuStatistics[i].steal);
		printf("guest: %llu\n", cpuStatistics[i].guest);
		printf("guest_nice: %llu\n", cpuStatistics[i].guest_nice);
		printf("\n");
		}
		pthread_mutex_unlock(&mutexBuffer);	//locking thread into mutex to solve pcp
		sem_post(&producerSemaphore);	//incrimenting semaphore, thus telling the wait() in reader thread that analyzing of this data packet is done and it can start to read next package of cpu data
		printf("AnalyzerTask Successfull\n");
	}
	return 0;
}
