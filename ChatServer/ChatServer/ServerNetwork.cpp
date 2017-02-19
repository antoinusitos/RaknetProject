#include "ServerNetwork.h"

#include <ctime>
#include <string>

namespace UserTemplate
{

	ServerNetwork::ServerNetwork()
	{
		_nbClient = 0;
		_nbClientRegistered = 0;

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

	void ServerNetwork::ThreatMessage(const MessageData theMessage, const RakNet::SystemAddress senderAddress) const
	{
		switch (theMessage._destination)
		{
			case MessageDestination::Destination_Multicast :
			{
				printf("message to send to all client %s : %s \n", theMessage._senderName, theMessage._toSend);
				RakNet::BitStream bsOut;
				bsOut.Write((RakNet::MessageID)ID_GAME_MESSAGE_1);
				std::string s = theMessage._toSend;
				std::string toSend = theMessage._senderName;
				toSend += " : " + s;
				bsOut.Write(toSend.c_str());
				peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, peer->GetMyGUID(), true);
			}
			break;
			case MessageDestination::Destination_Client:
			{
				printf("message to send to a client %s \n", theMessage._toSend);

				std::string s = theMessage._toSend;

				std::size_t found = s.find(' ');
				if (found != std::string::npos)
				{
					std::string name = s.substr(0, found);
					for (auto i = 0; i < _nbClient; i++)
					{
						if (strcmp(_allGUID[i]._name, name.c_str()) == 0)
						{
							s = s.erase(0, found);
							std::string toSend = theMessage._senderName;
							toSend += " : " + s;
							RakNet::BitStream bsOut;
							bsOut.Write((RakNet::MessageID)ID_GAME_MESSAGE_1);
							bsOut.Write(toSend.c_str());
							peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, _allGUID[i]._address, false);
							break;
						}
					}
				}
			}
			break;
			case MessageDestination::Destination_Server:
			{
				// TODO : analyse the message
				printf("message to analyse %s \n", theMessage._toSend);
			}
			break;
		}
	}

	void ServerNetwork::HandleMessages()
	{
		std::time_t lastTime = std::time(nullptr);
		while (_isRunning)
		{
			
			if (std::time(nullptr) - lastTime >= 3)
			{
				lastTime = std::time(nullptr);
				if (_nbClientRegistered > 0 && _allGUID[0]._name != "")
				{
					std::string toSend = "";
					for (auto i = 0; i < _allGUID.size(); i++)
					{
						toSend += _allGUID[i]._name;
						toSend += ";";
					}
					RakNet::BitStream bsOut;
					bsOut.Write((RakNet::MessageID)ID_REFRESH_CLIENT_LIST);
					bsOut.Write(toSend.c_str());
					peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, peer->GetMyGUID(), true);
				}
			}

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
				case ID_NEW_INCOMING_CONNECTION: 
				{
					printf("A connection is incoming. \n");
					_allGUID.push_back(ClientAddress());
					_allGUID[_nbClient]._address = packet->guid;
					printf("connection : %s \n", _allGUID[_nbClient]._address.ToString());
					_nbClient++;
				}
				break;
				case ID_DISCONNECTION_NOTIFICATION:
					printf("A client has disconnected.\n");
					break;
				case ID_CONNECTION_LOST:
					printf("A client lost the connection.\n");
					break;
				case ID_WANT_TO_DISCONNECT:
				{
					printf("A client has just disconnect : %s.\n", packet->guid.ToString());
					int indexToRemove = -1;
					char* leaverName = "unknown";
					for (auto i = 0; i < _allGUID.size(); i++)
					{
						if (_allGUID[i]._address == packet->guid)
						{
							indexToRemove = i;
							leaverName = _allGUID[i]._name;
						}
					}

					if (indexToRemove != -1)
					{
						RakNet::BitStream bsOut;
						bsOut.Write((RakNet::MessageID)ID_WANT_TO_DISCONNECT);
						bsOut.Write(leaverName);
						peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, peer->GetMyGUID(), true);

						_allGUID.erase(_allGUID.begin() + indexToRemove);
						_nbClientRegistered--;
						_nbClient--;
					}
				}
					break;
				case ID_DESTINATION_SERVER:
				{
					printf("server message.\n");

					MessageData md;
					RakNet::BitStream bsIn(packet->data, packet->length, false);
					bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
					bsIn.Read(md);
					printf("%s\n", md._toSend);
					ThreatMessage(md, packet->systemAddress);
				}
				break;
				case ID_DESTINATION_SERVER_INIT:
				{
					printf("server init message.\n");
					RakNet::RakString rs;
					RakNet::BitStream bsIn(packet->data, packet->length, false);
					bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
					bsIn.Read(rs);

					std::string name = rs.C_String();

					for (auto i = 0; i < _nbClient; i++)
					{
						if (strcmp(_allGUID[i]._address.ToString(), packet->guid.ToString()) == 0)
						{
							strcpy(_allGUID[i]._name, name.c_str());
						}
					}

					printf("sending welcome message..\n");

					std::string sending = "Welcome ";
					sending += name;
					sending += " !";

					//sending the custom welcome message
					RakNet::BitStream bsOut;
					bsOut.Write((RakNet::MessageID)ID_GAME_MESSAGE_1);
					bsOut.Write(sending.c_str());
					peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
					printf("Done !\n");

					printf("notifiying all other client..\n");

					sending = name;
					sending += " has joined !";
					RakNet::BitStream bsOutSecond;
					bsOutSecond.Write((RakNet::MessageID)ID_GAME_MESSAGE_1);
					bsOutSecond.Write(sending.c_str());
					peer->Send(&bsOutSecond, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, true);
					printf("Done !\n");

					_nbClientRegistered++;
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