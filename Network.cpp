#include "Network.h"

using namespace std;

Network::Network(bool s, const char* h, int p) {
	
	if (SDLNet_Init() < 0)
	{
		printf("SDLNet_Init: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}
	set = SDLNet_AllocSocketSet(2);
	if(!set) {
    	printf("SDLNet_AllocSocketSet: %s\n", SDLNet_GetError());
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
	SDLNet_FreeSocketSet(set);
	SDLNet_TCP_Close(sd);
	SDLNet_TCP_Close(csd);
	SDLNet_Quit();
}

bool Network::receivePacket(){
	if (serve){
		if(SDLNet_TCP_Recv(csd, buffer, 512) > 0)
		{
			printf("Client say: %s\n", buffer);
			return true;
		}
	}
	else 
	{
		if(SDLNet_TCP_Recv(sd, buffer, 512) > 0)
		{
			printf("Server say: %s\n", buffer);
			return true;
		}
	}
	return false;
}

bool Network::sendPacket(){
	if(serve)
	{
		char* m = "Server message";
		len = strlen(m) + 1;
		if (SDLNet_TCP_Send(csd, (void *)m, len) < len)
		{
			printf("SDLNet_TCP_Send Error: %s\n", SDLNet_GetError());
			//exit(EXIT_FAILURE);
			return false;
		}
		return true;
	}
	else 
	{
		char* m = "client message";
		len = strlen(m) + 1;
		if (SDLNet_TCP_Send(sd, (void *)m, len) < len)
		{
			printf("SDLNet_TCP_Send Error: %s\n", SDLNet_GetError());
			//exit(EXIT_FAILURE);
			return false;
		}
		return true;
	}
	return false;
}

void Network::server() {
	/* Resolving the host using NULL make network interface to listen */
	if (SDLNet_ResolveHost(&ip, NULL, 2000) < 0)
	{
		printf("SDLNet_ResolveHost: %s\n", SDLNet_GetError());
	}

	/* Open a connection with the IP provided (listen on the host's port) */
	if (!(sd = SDLNet_TCP_Open(&ip)))
	{
		printf("SDLNet_TCP_Open: %s\n", SDLNet_GetError());
	}
	quit = 0;
	printf("DEBUG: WAITING FOR CONNECTION========================== \n");
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
		}
	}
	printf("DEBUG: CONNECTION MADE ========================== \n");
	SDLNet_TCP_AddSocket(set,sd);
	SDLNet_TCP_AddSocket(set,csd);
	
}

void Network::client(const char* d, int p) {
	/* Resolve the host we are connecting to */

	if (SDLNet_ResolveHost(&ip, d, p) < 0)
	{
		printf("SDLNet_ResolveHost: %s\n", SDLNet_GetError());
		//exit(EXIT_FAILURE);
	}
 
	/* Open a connection with the IP provided (listen on the host's port) */
	if (!(sd = SDLNet_TCP_Open(&ip)))
	{
		printf("SDLNet_TCP_Open: %s\n", SDLNet_GetError());
		//exit(EXIT_FAILURE);
	}
	printf("DEBUG: CLIENT CONNECTED TO SERVER ========================== \n");
	/* Send messages */
	SDLNet_TCP_AddSocket(set,sd);
}