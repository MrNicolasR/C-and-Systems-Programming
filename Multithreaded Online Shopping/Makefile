CC = gcc
CFLAGS = -Wall -g

all: shopping

shopping: server.o client.o
	$(CC) $(CCFLAGS) -o server server.o  -lpthread
	$(CC) $(CCFLAGS) -o client client.o -lpthread


#session.o: session.c
#	$(CC) $(CCFLAGS) -c session.c

server.o: server.c server.h
	$(CC) $(CCFLAGS) -c server.c

client.o: client.c client.h
	$(CC) $(CCFLAGS) -c client.c

clean:
	@rm -rf *.o server client
