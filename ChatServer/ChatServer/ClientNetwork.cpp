#include "ClientNetwork.h"
#include <sstream>

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

		_messageReceived = false;
		_mustRefreshClientList = false;
	}

	ClientNetwork::~ClientNetwork()
	{

	}

	void ClientNetwork::Stop()
	{
		_isRunning = false;
		Exit();
	}

	void ClientNetwork::ClientSendMessage(const char* text, MessageDestination destination)
	{
		RakNet::BitStream bsOut;
		
		std::string toSend = "";
		toSend += text;

		MessageData message = MessageData();
		message._destination = destination;
		strcpy(message._toSend, toSend.c_str());
		message._senderAddress = _peer->GetMyGUID();
		strcpy(message._senderName, _name);

		switch (destination)
		{
			case MessageDestination::Destination_Client:
			{
				bsOut.Write((RakNet::MessageID)ID_DESTINATION_SERVER);
				bsOut.Write(message);
				_peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, _serverGUID, false);
				break;
			}
			case MessageDestination::Destination_Multicast:
			{
				bsOut.Write((RakNet::MessageID)ID_DESTINATION_SERVER);
				bsOut.Write(message);
				_peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, _serverGUID, false);
				break;
			}
			case MessageDestination::Destination_Server:
			{
				bsOut.Write((RakNet::MessageID)ID_DESTINATION_SERVER);
				bsOut.Write(message);
				_peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, _serverGUID, false);
				break;
			}
		}
	}

	void ClientNetwork::Init()
	{
		_isRunning = true;

		_networkThread = std::thread(&ClientNetwork::HandleMessages, this);
	}

	bool ClientNetwork::HasNewMessage()
	{
		return _messageReceived;
	}

	const char * ClientNetwork::GetNewMessage()
	{
		_messageReceived = false;
		return _theMessage;
	}

	bool ClientNetwork::MustRefreshClientList()
	{
		return _mustRefreshClientList;
	}

	std::vector<std::string> ClientNetwork::GetAllClientsConnected()
	{
		_mustRefreshClientList = false;
		return _clientConnected;
	}

	void ClientNetwork::Exit()
	{
		_networkThread.join();
		if(_peer != nullptr && _peer->IsActive())
			RakNet::RakPeerInterface::DestroyInstance(_peer);
	}

	void ClientNetwork::HandleMessages()
	{
		while (_isRunning)
		{
			for (_packet = _peer->Receive(); _packet; _peer->DeallocatePacket(_packet), _packet = _peer->Receive())
			{
				if (_packet != NULL)
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
						printf("Game Message : %s\n", rs.C_String());
						_messageReceived = true;
						_theMessage = rs.C_String();
					}
					break;
					case ID_REFRESH_CLIENT_LIST:
					{
						RakNet::RakString rs;
						RakNet::BitStream bsIn(_packet->data, _packet->length, false);
						bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
						bsIn.Read(rs);
						_clientConnected.clear();

						const char* temp = rs.C_String();
						std::string l(temp);
						std::string delimiter = ";";

						size_t pos = 0;
						std::string token;
						while ((pos = l.find(delimiter)) != std::string::npos) {
							token = l.substr(0, pos);
							_clientConnected.push_back(token);
							l.erase(0, pos + delimiter.length());
						}
						_mustRefreshClientList = true;
					}
					break;
					/*default:
						RakNet::RakString rs;
						RakNet::BitStream bsIn(_packet->data, _packet->length, false);
						bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
						bsIn.Read(rs);
						printf("%s\n", rs.C_String());
						break;*/
					}
				}
			}
		}
	}
}