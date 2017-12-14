all: serverSocket clientSocket launchClient UnCoServerSocket BoCoServerSocket ExBoServerSocket

launchClient: launchClient.c
	gcc -o launchClient launchClient.c

serverSocket: serverSocket.c socketMng.o
	gcc -o serverSocket -g serverSocket.c socketMng.o
	
##################
## CUSTOM
#################

UnCoServerSocket: UnCoServerSocket.c socketMng.o
	gcc -o UnCoServerSocket -g UnCoServerSocket.c socketMng.o

	
	
BoCoServerSocket: BoCoServerSocket.c socketMng.o
	gcc -o BoCoServerSocket -g BoCoServerSocket.c socketMng.o
	
ExBoServerSocket: ExBoServerSocket.c socketMng.o
	gcc -o ExBoServerSocket -g ExBoServerSocket.c socketMng.o -pthread
############################

clientSocket: clientSocket.c socketMng.o
	gcc -o clientSocket -g clientSocket.c socketMng.o

socketMng.o: socketMng.c
	gcc -c -g socketMng.c

clean: 
	rm -f serverSocket clientSocket socketMng.o launchClient UnCoServerSocket BoCoServerSocket ExBoServerSocket
