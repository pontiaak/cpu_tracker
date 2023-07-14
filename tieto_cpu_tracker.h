#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>	//lib for threads
#include <string.h>
#include <signal.h>	//lib for handling signals
#include <semaphore.h>	//lib for semaphores to use un producer-consumer-problem

void* ReaderTask();	//Task for Reader thread - is a "producer"
void* AnalyzerTask();	//Task for Analyzer thread - is a "consumer"
void* PrinterTask();	//Task for Printer thread
void* WatchdogTask();	//Task for Watchdog thread
void* LoggerTask();	//Task for Logger thread
void SigtermHandler(int signum);

extern pthread_mutex_t mutexBuffer;	//forward-declare of mutex in tieto_cpu_tracker.c for producer-consumer problem solution
extern sem_t producerSemaphore;	//forward-declare of a semaphore for reader thread (producer)
extern sem_t consumerSemaphore;	//forward-declare of a semaphore for analyzer thread (consumer)
extern sem_t loggerSemaphore;	//forward-declare of a semaphore used to wake up the logger and pass the message to it

extern volatile sig_atomic_t terminationRequest;	//atomic flag, it is set to 1 to start closing the application gently
extern int cpuNumber;	//number of cpus in shared memmory - cpuStatistics

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
#define loggerFilePath "logs.txt"	//the text file to which the logger will write down logs

extern struct cpuStatisticsStruct cpuStatistics[maximumCpuNumber];	//forward-declare creation of an array of structures to store all cpu's statistics during Reader thread to later pass to Analyzer thread
extern unsigned long long cpuUsageSumm[maximumCpuNumber];	//array to act as buffer containing summ of multiple sets of cpu usage statistics of all processors to be later devided by cpuUsageSetsNumber during the printer thread to present avarage cpu usage for 1s
extern int cpuUsageSetsNumber;	//number of sets of data aggregated during 1second
extern int watchdogFlags[4];	//the threads will need to set the corresponding [i] flag to 1 every 2 seconds to show the watchdog that they are active
extern int loggerMessage;	//global variable to pass the type of message the logger needs to print
