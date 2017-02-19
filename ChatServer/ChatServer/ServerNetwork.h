#pragma once

#include <stdio.h>
#include <string.h>
#include <iostream>
#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include "BitStream.h"
#include "RakNetTypes.h"  // MessageID
#include <thread>
#include <memory>
#include "Data.h"
#include <vector>

#define MAX_CLIENTS 10
#define SERVER_PORT 60000

namespace UserTemplate
{
	class ServerNetwork
	{

	enum GameMessages
	{
		ID_GAME_MESSAGE_1 = ID_USER_PACKET_ENUM + 1
	};

	public:
		ServerNetwork();
		~ServerNetwork();

		void Stop();

	private:
		void Init();

		void Exit();

		void HandleMessages();

		void ThreatMessage(const MessageData theMessage, const RakNet::SystemAddress senderAddress) const;

	private:
		std::unique_ptr<std::thread> _networkThread;

		bool _isRunning;

		RakNet::RakPeerInterface* peer = RakNet::RakPeerInterface::GetInstance();
		RakNet::Packet* packet;
		RakNet::SystemAddress serverGUID;

		int _nbClient;
		int _nbClientRegistered;

		std::vector<ClientAddress> _allGUID;
	};
}