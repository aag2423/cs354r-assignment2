#ifndef __Network_h_
#define __Network_h_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SDL_net.h"

using namespace std;

class Network {
public:
	Network(bool s, const char* h, int p);
	~Network();
	void server();
	void client(const char* d, int p);
	bool receivePacket();
	bool sendPacket();
	TCPsocket sd, csd;
	IPaddress ip, *remoteIP;
	int quit, quit2, len;
	char buffer[512];
	bool serve;
};

#endif
