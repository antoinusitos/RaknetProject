#include "ServerNetwork.h"

namespace UserTemplate
{
	ServerNetwork::ServerNetwork()
	{
		_nbClient = 0;

		RakNet::SocketDescriptor sd(SERVER_PORT, 0);
		peer->Startup(MAX_CLIENTS, &sd, 1);

		Init();
	}

	ServerNetwork::~ServerNetwork()
	{
		Stop();
	}

	void ServerNetwork::Stop()
	{
		_isRunning = false;
		Exit();
	}

	void ServerNetwork::Init()
	{
		printf("Starting the server.\n");
		// We need to let the server accept incoming connections from the clients
		peer->SetMaximumIncomingConnections(MAX_CLIENTS);
		_isRunning = true;
		_networkThread = std::make_unique<std::thread>(&ServerNetwork::HandleMessages, this);
	}

	void ServerNetwork::Exit()
	{
		_networkThread.get()->join();
		RakNet::RakPeerInterface::DestroyInstance(peer);
	}

	void ServerNetwork::HandleMessages()
	{
		while (_isRunning)
		{
			for (packet = peer->Receive(); packet; peer->DeallocatePacket(packet), packet = peer->Receive())
			{
				switch (packet->data[0])
				{
				case ID_REMOTE_DISCONNECTION_NOTIFICATION:
					printf("Another client has disconnected.\n");
					break;
				case ID_REMOTE_CONNECTION_LOST:
					printf("Another client has lost the connection.\n");
					break;
				case ID_REMOTE_NEW_INCOMING_CONNECTION:
					printf("Another client has connected.\n");
					break;
				break;
				case ID_NEW_INCOMING_CONNECTION:
				{
					printf("A connection is incoming. \n");
					_allGUID[_nbClient] = packet->guid;
					printf("connection : %s \n", _allGUID[_nbClient].ToString());

					_nbClient++;

					RakNet::BitStream bsOut;
					bsOut.Write((RakNet::MessageID)ID_GAME_MESSAGE_1);
					bsOut.Write("Welcome!");
					peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, peer->GetMyGUID(), true);
				}
				break;
				case ID_DISCONNECTION_NOTIFICATION:
					printf("A client has disconnected.\n");
					break;
				case ID_CONNECTION_LOST:
					printf("A client lost the connection.\n");
					break;
				case ID_DESTINATION_MULTICAST:
				{
					RakNet::RakString rs;
					RakNet::BitStream bsIn(packet->data, packet->length, false);
					bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
					bsIn.Read(rs);
					printf("%s\n", rs.C_String());
					peer->Send(&bsIn, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, true);
				}
				break;
				case ID_DESTINATION_SERVER:
				{
					RakNet::RakString rs;
					RakNet::BitStream bsIn(packet->data, packet->length, false);
					bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
					bsIn.Read(rs);
					printf("%s\n", rs.C_String());
				}
				break;
				default:
					printf("Message with identifier %i has arrived.\n", packet->data[0]);

					RakNet::RakString rs;
					RakNet::BitStream bsIn(packet->data, packet->length, false);
					bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
					bsIn.Read(rs);
					printf("%s\n", rs.C_String());
					break;
				}
			}

		}
	}
}