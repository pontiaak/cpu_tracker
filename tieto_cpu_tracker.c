//Main function for cpu tracker app

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>	//lib for threads
#include "tieto_cpu_tracker.h"	//header file for this project

struct cpuStatisticsStruct cpuStatistics[maximumCpuNumber];

int main()
{	
	pthread_t threadReader, threadAnalyzer, threadPrinter;
	int retReader, retAnalyzer, retPrinter;	//values as output of creating threads to check for success in their creation
	
//Reader Thread
	retReader = pthread_create( &threadReader, NULL, ReaderTask, NULL);	//creating reader thread, second atribute is NULL, ReaderTask is a function this thread accomplishes
	if (retReader != 0) printf("Problem encountered while creating Reader Thread\n"); /*else printf("Reader Thread Created Successfully\n");*/ 
	pthread_join( threadReader, NULL);	//terminating thread
	printf("Reader Thread Terminated\n");	//checking for thread termination


//Analyzer Thread
	retAnalyzer = pthread_create( &threadAnalyzer, NULL, AnalyzerTask, NULL);	//creating analyzer thread, second atribute is NULL, AnalyzerTask is a function this thread accomplishes
	if (retAnalyzer != 0) printf("Problem encountered while creating Analyzer Thread\n"); /*else printf("Analyzer Thread Created Successfully\n");*/
	pthread_join( threadAnalyzer, NULL);	//terminating thread
	printf("Analyzer Thread Terminated\n");	//checking for thread termination


//Printer Thread
	retPrinter = pthread_create( &threadPrinter, NULL, PrinterTask, NULL);	//creatin printer thread, second atribute is NULL, PrinterTask is a function this thread accomplishes
	if (retPrinter != 0) printf("Problem encountered while creating Printer Thread\n"); /*else printf("Printer Thread Created Successfully\n");*/ 
	pthread_join( threadPrinter, NULL);	//terminating thread
	printf("Printer Thread Terminated\n");	//checking for thread termination
	
	return 0;

}
