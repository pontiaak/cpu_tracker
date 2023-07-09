#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>	//lib for threads

void* ReaderTask();	//Task for Reader thread
void* AnalyzerTask();	//Task for Analyzer thread
void* PrinterTask();	//Task for Printer thread
