#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>	//lib for threads
#include <string.h>
#include <semaphore.h>	//lib for semaphores to use un producer-consumer-problem

void* ReaderTask();	//Task for Reader thread - is a "producer"
void* AnalyzerTask();	//Task for Analyzer thread - is a "consumer"
void* PrinterTask();	//Task for Printer thread

extern pthread_mutex_t mutexBuffer;	//forward-declare of mutex in tieto_cpu_tracker.c for producer-consumer problem solution
extern sem_t producerSemaphore;	//forward-declare of a semaphore for reader thread (producer)
extern sem_t consumerSemaphore;	//forward-declare of a semaphore for analyzer thread (consumer)

struct cpuStatisticsStruct
{
	unsigned long long user;
	unsigned long long nice;
	unsigned long long system;
	unsigned long long idle;
	unsigned long long iowait;
	unsigned long long irq;
	unsigned long long softirq;
	unsigned long long steal;
	unsigned long long guest;
	unsigned long long guest_nice;
};

#define maximumLineLength 256	//defining maximum line length
#define maximumCpuNumber 256	//defining maximum number of cpus a system can have for an array of structures cpuStatistics

extern struct cpuStatisticsStruct cpuStatistics[maximumCpuNumber];	//forward-declare creation of an array of structures to store all cpu's statistics during Reader thread to later pass to Analyzer thread

