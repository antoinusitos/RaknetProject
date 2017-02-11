#include "ClientNetwork.h"

namespace UserTemplate
{
	ClientNetwork::ClientNetwork()
	{
		char str[512];
		RakNet::SocketDescriptor sd;
		_peer->Startup(1, &sd, 1);

		printf("Enter server IP or hit enter for 127.0.0.1\n");
		std::cin.getline(str, 512);
		if (str[0] == 0) {
			strcpy(str, "127.0.0.1");
		}
		printf("Starting the client.\n");
		_peer->Connect(str, SERVER_PORT, 0, 0);

		_name = new char[512];

		printf("Enter a name \n");
		std::cin.getline(_name, 512);

		Init();
	}

	ClientNetwork::~ClientNetwork()
	{
		Stop();
	}

	void ClientNetwork::Stop()
	{
		_isRunning = false;
		Exit();
	}

	void ClientNetwork::ClientSendMessage(const char* text, MessageDestination destination)
	{
		RakNet::BitStream bsOut;
		
		char toSend[500];
		strcpy(toSend, _name);
		strcat(toSend, " :");
		strcat(toSend, text);

		switch (destination)
		{
			case MessageDestination::Destination_Client:
			{

				break;
			}
			case MessageDestination::Destination_Multicast:
			{
				bsOut.Write((RakNet::MessageID)ID_DESTINATION_MULTICAST);
				bsOut.Write(toSend);
				_peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, _peer->GetMyGUID(), true);
				break;
			}
			case MessageDestination::Destination_Server:
			{
				bsOut.Write((RakNet::MessageID)ID_DESTINATION_SERVER);
				bsOut.Write(toSend);
				_peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, _serverGUID, false);
				break;
			}
		}
	}

	void ClientNetwork::Init()
	{
		_isRunning = true;

		_networkThread = std::make_unique<std::thread>(&ClientNetwork::HandleMessages, this);
	}

	void ClientNetwork::Exit()
	{
		_networkThread.get()->join();
		RakNet::RakPeerInterface::DestroyInstance(_peer);
	}

	void ClientNetwork::HandleMessages()
	{
		while (_isRunning)
		{
			for (_packet = _peer->Receive(); _packet; _peer->DeallocatePacket(_packet), _packet = _peer->Receive())
			{
				switch (_packet->data[0])
				{
				case ID_CONNECTION_REQUEST_ACCEPTED:
				{
					printf("Our connection request has been accepted.\n");
					printf("Our GUID : %s \n", _peer->GetMyGUID().ToString());
					// Use a BitStream to write a custom user message
					// Bitstreams are easier to use than sending casted structures, and handle endian swapping automatically
					RakNet::BitStream bsOut;
					bsOut.Write((RakNet::MessageID)ID_DESTINATION_SERVER_INIT);
					bsOut.Write(_name);
					_peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, _packet->systemAddress, false);

					_serverGUID = _packet->systemAddress;
				}
				break;
				case ID_NO_FREE_INCOMING_CONNECTIONS:
					printf("The server is full.\n");
					break;
				case ID_DISCONNECTION_NOTIFICATION:
					printf("We have been disconnected.\n");
					break;
				case ID_CONNECTION_LOST:
					printf("Connection lost.\n");
					break;
				case ID_GAME_MESSAGE_1:
				{
					RakNet::RakString rs;
					RakNet::BitStream bsIn(_packet->data, _packet->length, false);
					bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
					bsIn.Read(rs);
					printf("%s\n", rs.C_String());
				}
				break;
				default:
					RakNet::RakString rs;
					RakNet::BitStream bsIn(_packet->data, _packet->length, false);
					bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
					bsIn.Read(rs);
					printf("%s\n", rs.C_String());
					break;
				}
			}

		}
	}
}