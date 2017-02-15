#include <stdio.h>
#include <string.h>
#include <iostream>
#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include "BitStream.h"
#include "RakNetTypes.h"  // MessageID
#include <thread>

#include "Client.h"
#include "Server.h"

#include <SFML/Graphics.hpp>

#define MAX_CLIENTS 10
#define SERVER_PORT 60000

int main(void)
{
	char str[512];

	RakNet::RakPeerInterface *peer = RakNet::RakPeerInterface::GetInstance();
	RakNet::SystemAddress serverGUID;

	printf("(C) or (S)erver?\n");
	std::cin.getline(str, 512);
	if ((str[0] == 'c') || (str[0] == 'C'))
	{
		UserTemplate::Client* c = new UserTemplate::Client();
		delete c;
	}
	else 
	{
		UserTemplate::Server* s = new UserTemplate::Server();
		delete s;
	}

	return 0;
}