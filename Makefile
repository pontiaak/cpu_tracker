#This is a makefile for cpu tracker project by Igor Elche

final:
	gcc tieto_cpu_tracker.c Reader.c Analyzer.c Printer.c -o cpu_tracker_app -Wall -Wextra -lpthread
	#gcc tieto_cpu_tracker.c -o cpu_tracker_app -Wall -Wextra
