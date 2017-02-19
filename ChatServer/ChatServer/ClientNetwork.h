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
#include <vector>
#include <mutex>

#include "Data.h"

#define SERVER_PORT 60000

namespace UserTemplate
{

	class ClientNetwork
	{

	enum GameMessages
	{
		ID_GAME_MESSAGE_1 = ID_USER_PACKET_ENUM + 1
	};

	public:

		ClientNetwork();
		~ClientNetwork();

		void Stop();

		void ClientSendMessage(const char* text, MessageDestination destination);

		void Init();

		bool HasNewMessage();
		std::string GetNewMessage();

		bool MustRefreshClientList();

		std::vector<std::string> GetAllClientsConnected();

	private:

		void Exit();

		void HandleMessages();

	private:
		std::thread _networkThread;

		bool _isRunning;

		RakNet::RakPeerInterface* _peer = RakNet::RakPeerInterface::GetInstance();
		RakNet::Packet* _packet;
		RakNet::SystemAddress _serverGUID;

		char* _name;

		bool _messageReceived;
		//const char* _theMessage;
		std::string _theMessage;

		bool _mustRefreshClientList;

		std::vector<std::string> _clientConnected;

		std::mutex m;
	};
}