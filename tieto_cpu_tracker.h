#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>	//lib for threads
#include <string.h>

void* ReaderTask();	//Task for Reader thread
void* AnalyzerTask();	//Task for Analyzer thread
void* PrinterTask();	//Task for Printer thread

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

extern struct cpuStatisticsStruct cpuStatistics[maximumCpuNumber];	//creating an array of structures to store all cpu's statistics during Reader thread to later pass to Analyzer thread
