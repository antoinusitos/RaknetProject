#pragma once
#include <string>
#include "RakPeerInterface.h"

enum MessageDestination
{
	Destination_Client,
	Destination_Server,
	Destination_Multicast,
};

struct MessageData
{
	MessageDestination _destination;
	char _toSend[500];
	char _senderName[500];
	RakNet::AddressOrGUID _senderAddress;
};

struct ClientAddress
{
	RakNet::RakNetGUID _address;
	char _name[500];
};