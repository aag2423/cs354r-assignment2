#ifndef __Network_h_
#define __Network_h_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include "SDL_net.h"

using namespace std;

class Network {
public:
	Network(bool s, const char* h);
	~Network();
	/*TCPsocket sd, csd;
	IPaddress ip, *remoteIP;
	int quit, quit2, len;
	*/
	bool serve;
	char buffer[512];
	pthread_t thread;
	void sendGameState();
};
void sig_func(int sig);
void *server(void* threadid);
void *client(void* domain);

static TCPsocket sd, csd;
static IPaddress ip, *remoteIP;
static int quit, quit2, len;
static IPaddress target;

#endif
