
CONTENTS OF THIS FILE
---------------------

* Introduction
* Installation 
* HOW TO USE	
* Trouble Shooting
* Programming Methods
* FAQ
* Creators


INTRODUCTION 
---------------------

	The shopping program makes a server which holds a set of commands
replicating an online store which the client connects to via tcp/ip. The client
can then enter commands that the server will execute.



INSTALLATION
---------------------

* Download the tar file, "pa3.tar" from sakai
* Once downloaded proceed to the terminal and enter the following command
	tar -xvf pa3.tar
* The file should contain the following:
	- server.c (the server program)
	- client.c (the client program)
	- server.h 
	- client.h
	- functions.h
	- session.c (functions for the server)
	- catalog.txt (catalog file)
	- Makefile (make file)
	- README.txt (Readme file)
	- testcases.txt
	- testplan.txt
	- shopping.pdf (The instructions for the hw)
	
* After this conduct your test


HOW TO USE
---------------------
(WARNING WAIT TO BE PROMPTED TO USE COMMANDS OR ELSE BAD THINGS WILL HAPPEN)

1. Go to terminal
2. Use make function
3. Computer may ask for permission accept it
4. For the server
	-   ./server to run the program
5. For the client
	-   ./client <host name>
6. The program should run on both sides
7. Follow the prompts


Trouble Shooting
---------------------

	The catalog function does not print to the user neither does the placarder function.
	(WARNING WAIT TO BE PROMPTED TO USE COMMANDS OR ELSE BAD THINGS WILL HAPPEN)


	If the program does not contain all the files, does not compile, or not perform 
as expected, feel free to take points off!

Programming Methods
---------------------
	The main server thread is initialized to open the first socket for communication on the input port. Then it enter a loop to constantly check for new client connections. When a new client connection is found, it creates another socket for that connection for use with that client instance. After this socket is made, the program creates a new thread to service the client. This function works in two phases. First it waits for a message from the client with recv() and then it sends back a response with write() after processing. Since recv() is a blocking call, the particular thread waits until the client either disconnects or sends a message. 
	The server handles the function process for account creation, adding/removing items , catalog, places order, done, and withdrawing. This logic occurs right after one of the server threads recieves a message from the client. The create method makes a new account in a list of accounts. This method is mutex protected so that multiple threads do not make an account at the same time which would mess up the linked list data. This ensures that the information is correct. The function will be run, this can either be an error if the process could not be completed or it could be a completion message. Then the server sends that message back to client. If the client disconnects from the server, the thread is closed and all resources are dealt with.
	The server shuts down when the sigint handler occurs when ctrl-c is inputted in the console. This function closes all threads by using a global linked list of tids. After each thread is closed, the function closes the main socket which is using the port and terminates the program. 


FAQ
---------------------

* How hard was it to code this program?
	This was an exponential increase in difficulty for me compared to the last 
two assignments. This was the hardest programming homework for me till this time.
I never have written so many lines of code. The debugging was a pain so with testing.

* How long did it take you to complete?
	It took me from spring break till today.

Creators
----------------------

* Nicolas Rubert 
	- RUID: 172007365
	- NetID: nar126


