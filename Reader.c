//Task for Reader thread
//The first thread (Reader) reads /proc/stat and sends the string (as raw data) or a structure (fields from file needed for calculation) to the second thread (Analyzer)

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>	//lib for threads
#include <string.h>
#include "tieto_cpu_tracker.h"	//header file for this project


void* ReaderTask(){

int cpuArrayIndex = 0;	//index for currently processed line of cpu statistics being processed into a structure


	FILE* file = fopen("/proc/stat", "r");	//opening /proc/stat with atribute for only reading ("r")
	if (file == NULL) {	//catching an issue
		perror("Failed to open /proc/stat");
		return 0;
	}

	char fileLine[maximumLineLength];
	while (fgets(fileLine, sizeof(fileLine), file))	//getting all the lines out the file
	{

		while (fgets(fileLine, sizeof(fileLine), file) && cpuArrayIndex < maximumCpuNumber) {	//iterating by line whith "cpu" and adding all the statistics to the global structure
			if (strncmp(fileLine, "cpu", 3) == 0)	//getting all the lines with cpu statistics
			{			
				sscanf(fileLine, "%*s %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu",
				&cpuStatistics[cpuArrayIndex].user,
				&cpuStatistics[cpuArrayIndex].nice,
				&cpuStatistics[cpuArrayIndex].system,
				&cpuStatistics[cpuArrayIndex].idle,
				&cpuStatistics[cpuArrayIndex].iowait,
				&cpuStatistics[cpuArrayIndex].irq,
				&cpuStatistics[cpuArrayIndex].softirq,
				&cpuStatistics[cpuArrayIndex].steal,
				&cpuStatistics[cpuArrayIndex].guest,
				&cpuStatistics[cpuArrayIndex].guest_nice);
					
				cpuArrayIndex++;	//incrimenting to continue iterating
			}
		}
	}




	fclose(file);
	printf("ReaderTask Successfull\n");
	return 0;
}
