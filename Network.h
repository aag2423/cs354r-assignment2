#ifndef __Network_h_
#define __Network_h_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <cstring.h>
#include "SDL_net.h"
#include "Game.h"

using namespace std;

class Network {
public:
	bool connectionSuccess;
	Network(bool s, const char* h);
	~Network();
	void server();
	void client(const char* d);
	bool receivePacket();
	bool sendPacket();
	bool receiveOutputState(OutputState* os);
	bool sendOutputState(OutputState os);
	bool receiveInputState(InputState* in);
	bool sendInputState(InputState in);
	SDLNet_SocketSet set;
	TCPsocket sd, csd, c2sd;
	IPaddress ip, *remoteIP;
	int quit, quit2, len;
	char buffer[512];
	bool serve;
	char* gameHost;
};

#endif
