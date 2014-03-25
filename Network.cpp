#include "Network.h"

using namespace std;

Network::Network(bool s, const char* h, int p) {
	
	if (SDLNet_Init() < 0)
	{
		printf("SDLNet_Init: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}
	set = SDLNet_AllocSocketSet(3);
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
	if(serve){
		SDLNet_TCP_Close(csd);
	}
	SDLNet_TCP_Close(sd);
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

bool Network::receiveOutputState(OutputState* os){
	//OutputState os;
	SDLNet_CheckSockets(set, 100);
	if(SDLNet_SocketReady(csd)){
		if(SDLNet_TCP_Recv(csd, buffer, 512) > 0)
		{
			memcpy(os, buffer, sizeof(OutputState));
			std::cout << "receiveOutputState success" << std::endl;
			return true;
		}
	}
	std::cout << "receiveOutputState fail" << std::endl;
	return false;
}

bool Network::sendOutputState(OutputState os)
{
	if(serve)
	{
		len = sizeof(OutputState) + 1;
		memcpy(buffer, &os, sizeof(OutputState));
		if (SDLNet_TCP_Send(csd, (void *)buffer, len) < len)
		{
			std::cout << "sendOutputState fail: " << SDLNet_GetError() << std::endl;
			//exit(EXIT_FAILURE);
			return false;
		}else{	
			std::cout << "sendOutputState success" << std::endl;
			return true;
		}
	}
	return false;
}

bool Network::receiveInputState(InputState* in)
{
	//OutputState os;
	SDLNet_CheckSockets(set, 100);
	if(SDLNet_SocketReady(csd)){
		if(SDLNet_TCP_Recv(csd, buffer, 512) > 0)
		{
			memcpy(in, buffer, sizeof(InputState));
			std::cout << "receiveInputState success" << std::endl;
			return true;
		}
	}
	std::cout << "receiveInputState fail" << std::endl;
	return false;
}

bool Network::sendInputState(InputState in)
{
	if(!serve)
	{
		len = sizeof(InputState) + 1;
		memcpy(buffer, &in, sizeof(InputState));
		if (SDLNet_TCP_Send(csd, (void *)buffer, len) < len)
		{
			std::cout << "sendInputState fail: " << SDLNet_GetError() << std::endl;
			return false;
		}else{	
			std::cout << "sendInputState success" << std::endl;
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
		if (SDLNet_TCP_Send(c2sd, (void *)m, len) < len)
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
	quit = 1;
	printf("DEBUG: WAITING FOR Players ========================== \n");
	int counter = 100000;
	while (quit && counter > 0)
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
			quit--;
		}
		counter--;
	}
	
	connectionSuccess = counter > 0;
	if (!connectionSuccess) return;
	
	printf("DEBUG: CONNECTION MADE ========================== \n");
	SDLNet_TCP_AddSocket(set,sd);
	SDLNet_TCP_AddSocket(set,csd);
	
}

void Network::client(const char* d, int p) {
	/* Resolve the host we are connecting to */
	//gameHost = d;
	if (SDLNet_ResolveHost(&ip, d, p) < 0)
	{
		printf("SDLNet_ResolveHost: %s\n", SDLNet_GetError());
		//exit(EXIT_FAILURE);
	}
 
	/* Open a connection with the IP provided (listen on the host's port) */
	if (!(csd = SDLNet_TCP_Open(&ip)))
	{
		printf("SDLNet_TCP_Open: %s\n", SDLNet_GetError());
		//exit(EXIT_FAILURE);
	}
	connectionSuccess = !!csd;
	if (!connectionSuccess) return;
	printf("DEBUG: CLIENT CONNECTED TO SERVER ========================== \n");
	/* Send messages */
	SDLNet_TCP_AddSocket(set,csd);
}
