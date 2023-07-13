#This is a makefile for cpu tracker project by Igor Elche

final:
	gcc tieto_cpu_tracker.c Reader.c Analyzer.c Printer.c Sigterm.c -o cpu_tracker_app -Wall -Wextra -lpthread
