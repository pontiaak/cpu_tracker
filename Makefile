#This is a makefile for cpu tracker project by Igor Elche
CC = gcc
CFLAGS = -Wall -Wextra -lpthread

final:
	$(CC) tieto_cpu_tracker.c Reader.c Analyzer.c Printer.c Watchdog.c Logger.c Sigterm.c -o cpu_tracker_app $(CFLAGS)
	
test:
	$(CC) unit_test.c Reader.c Analyzer.c Printer.c Watchdog.c Logger.c Sigterm.c -o auto_test_cpu_app $(CFLAGS)

clean:
	rm -rf cpu_tracker_app
	rm -rf auto_test_cpu_app
	rm -rf logs.txt
