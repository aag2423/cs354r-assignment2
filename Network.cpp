#include "Network.h"

using namespace std;

Network::Network(bool s, const char* h) {
	if (SDLNet_Init() < 0)
	{
		printf("SDLNet_Init: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}
	serve = s;
	printf("server: %d ", serve);
	signal(SIGKILL, sig_func);
	
	if (serve)
	{
		
		//start thread to be server
		int t = 1;
		while(pthread_create(&thread, NULL, server, (void*)t))
		{
			printf("server thread failed to start \n");
		}
		printf("server thread start success \n");
		
	}
	
	else
	{
		
		//start client thread
		//char* d = "quist-arcton.cs.utexas.edu";
		while(pthread_create(&thread, NULL, client, (void*)h))
		{
			printf("client thread failed to start \n");
		}
		printf("client thread start success \n");

	}
	
}

Network::~Network() {
	pthread_kill(thread, SIGKILL);
	SDLNet_Quit();
}

void Network::sendGameState()
{

}

void sig_func(int sig){
	printf("exiting thread\n");
	if(sig == SIGKILL){
		quit = 1;
		quit2 = 1;
	}
}

void *server(void* threadid) {
	/*
	int quit, quit2, len;
	TCPsocket sd, csd;
	IPaddress ip, *remoteIP;
	*/
	char buffer[512];
	/* Resolving the host using NULL make network interface to listen */
	if (SDLNet_ResolveHost(&ip, NULL, 2000) < 0)
	{
		printf("SDLNet_ResolveHost: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

	/* Open a connection with the IP provided (listen on the host's port) */
	if (!(sd = SDLNet_TCP_Open(&ip)))
	{
		printf("SDLNet_TCP_Open: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}
	quit = 0;
	while (!quit)
	{
		/* This check the sd if there is a pending connection.
		* If there is one, accept that, and open a new socket for communicating */
		if ((csd = SDLNet_TCP_Accept(sd)))
		{
			/* Now we can communicate with the client using csd socket
			* sd will remain opened waiting other connections */
 
			/* Get the remote address */
			if ((remoteIP = SDLNet_TCP_GetPeerAddress(csd)))
				/* Print the address, converting in the host format */
				printf("Client connected: %x %d\n", SDLNet_Read32(&remoteIP->host), SDLNet_Read16(&remoteIP->port));
			else
				printf("SDLNet_TCP_GetPeerAddress: %s\n", SDLNet_GetError());
 
			quit2 = 0;
			while (!quit2)
			{
				if (SDLNet_TCP_Recv(csd, buffer, 512) > 0)
				{
					printf("Client say: %s\n", buffer);
 
					if(strcmp(buffer, "exit") == 0)	/* Terminate this connection */
					{
						quit2 = 1;
						printf("Terminate connection\n");
					}
					if(strcmp(buffer, "quit") == 0)	/* Quit the program */
					{
						quit2 = 1;
						quit = 1;
						printf("Quit program\n");
					}
				}
			}
 			fprintf(stdout, "closing server sockets \n");
			/* Close the client socket */
			SDLNet_TCP_Close(csd);
		}
	}
	SDLNet_TCP_Close(sd);
	pthread_exit(NULL);
}

void *client(void* domain) {
/*
	int quit, quit2, len;
	TCPsocket sd, csd;
	IPaddress ip, *remoteIP;
*/
	char* message = "client sending message to server!!! ";
	/* Resolve the host we are connecting to */
	if (SDLNet_ResolveHost(&ip, (char *)domain, 2000) < 0)
	{
		printf("SDLNet_ResolveHost: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}
 
	/* Open a connection with the IP provided (listen on the host's port) */
	if (!(sd = SDLNet_TCP_Open(&ip)))
	{
		printf("SDLNet_TCP_Open: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}
	/* Send messages */
	
	len = strlen(message) + 1;
	if (SDLNet_TCP_Send(sd, (void *)message, len) < len)
	{
		printf("SDLNet_TCP_Send: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}
	SDLNet_TCP_Close(sd);
	pthread_exit(NULL);
}
