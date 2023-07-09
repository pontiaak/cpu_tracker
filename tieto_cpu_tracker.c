//Main function for cpu tracker app

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>	//lib for threads
#include "tieto_cpu_tracker.h"	//header file for this project

int main()
{
	pthread_t threadReader, threadAnalyzer, threadPrinter;
	int retReader, retAnalyzer, retPrinter;	//values as output of creating threads to check for success in their creation
	
	//creating threads
	retReader = pthread_create( &threadReader, NULL, ReaderTask, NULL);	//creatin reader thread, second atribute is NULL, ReaderTask is a function this thread accomplishes
	retAnalyzer = pthread_create( &threadAnalyzer, NULL, AnalyzerTask, NULL);	//creatin analyzer thread, second atribute is NULL, AnalyzerTask is a function this thread accomplishes
	retPrinter = pthread_create( &threadPrinter, NULL, PrinterTask, NULL);	//creatin printer thread, second atribute is NULL, PrinterTask is a function this thread accomplishes
	
	//checking for success in creating threads
	if (retReader == 0) printf("Reader Thread Created Successfully\n"); else printf("Problem encountered while creating Reader Thread\n");
	if (retAnalyzer == 0) printf("Analyzer Thread Created Successfully\n"); else printf("Problem encountered while creating Analyzer Thread\n");
	if (retPrinter == 0) printf("Printer Thread Created Successfully\n"); else printf("Problem encountered while creating Printer Thread\n");
	
	//waiting for thread termination
	pthread_join( threadReader, NULL);
	printf("Reader Thread Terminated\n");
	pthread_join( threadAnalyzer, NULL);
	printf("Analyzer Thread Terminated\n");
	pthread_join( threadPrinter, NULL);
	printf("Printer Thread Terminated\n");
	
	return 0;

}
