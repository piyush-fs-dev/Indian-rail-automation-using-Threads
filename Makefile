CC=gcc
CFLAGS=-Wall -Wno-unused-value

calltrain: calltrain_run_file.c calltrain.c calltrain_base_files.h
	$(CC) -c calltrain.c -o calltrain.o
	$(CC) -c calltrain_run_file.c -o calltrain_run_file.o
	$(CC) $(CFLAGS) calltrain.o calltrain_run_file.o -o run -lpthread
