//SIGTERM handler to close the application gently.
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "tieto_cpu_tracker.h"	//header file for this project

void SigtermHandler(int signum)
{
	printf("Received SIGINT with signal number %d\n", signum);
 	terminationRequest = 1;
}
