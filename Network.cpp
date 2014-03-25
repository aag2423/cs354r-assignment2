#include "Network.h"

using namespace std;

Network::Network(bool s, const char* h, int p) {
	if (SDLNet_Init() < 0)
	{
		printf("SDLNet_Init: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

	serve = s;
	if (serve){
		server();
	}
	else {
		client(h, p);
	}
}

Network::~Network() {
	SDLNet_TCP_Close(sd);
	SDLNet_TCP_Close(csd);
	SDLNet_Quit();
}

bool Network::receivePacket(){
	if (serve){
		if(SDLNet_TCP_Recv(csd, buffer, 512) > 0)
		{

			return true;
		}
	}
	else 
	{

	}
	return false;
}

bool Network::sendPacket(){
	if(serve)
	{

	}
	else 
	{

	}
	return false;
}

void Network::server() {
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
				printf("Host connected: %x %d\n", SDLNet_Read32(&remoteIP->host), SDLNet_Read16(&remoteIP->port));
			else
				printf("SDLNet_TCP_GetPeerAddress: %s\n", SDLNet_GetError());
			quit = 1;


 			/*
			quit2 = 0;
			while (!quit2)
			{
				if (SDLNet_TCP_Recv(csd, buffer, 512) > 0)
				{
					printf("Client say: %s\n", buffer);
 
					if(strcmp(buffer, "exit") == 0)	/* Terminate this connection 
					{
						quit2 = 1;
						printf("Terminate connection\n");
					}
					if(strcmp(buffer, "quit") == 0)	/* Quit the program 
					{
						quit2 = 1;
						quit = 1;
						printf("Quit program\n");
					}
				}
			}
			*/
		}
	}
}

void Network::client(const char* d, int p) {
	/* Resolve the host we are connecting to */
	if (SDLNet_ResolveHost(&ip, d, p) < 0)
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
	quit = 0;
	while (!quit)
	{
		printf("Write something:\n>");
		scanf("%s", buffer);
 
		len = strlen(buffer) + 1;
		if (SDLNet_TCP_Send(sd, (void *)buffer, len) < len)
		{
			printf("SDLNet_TCP_Send: %s\n", SDLNet_GetError());
			exit(EXIT_FAILURE);
		}
		quit = 1;
	}
}