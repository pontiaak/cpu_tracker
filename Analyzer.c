//Task for Analyzer thread
//The second thread (Analyzer) works on raw data and produces CPU usage (in percentages) for each CPU core from /proc/stat. Then thread sends CPU usage to the third thread (Printer)

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>	//lib for threads
#include "tieto_cpu_tracker.h"	//header file for this project

void* AnalyzerTask(){
		for (int i = 0; i < 8; i++) {
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
	printf("AnalyzerTask Successfull\n");
	return 0;
}
