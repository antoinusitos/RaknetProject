#include "Client.h"

#include "Data.h"
#include <SFML/Graphics.hpp>

namespace UserTemplate
{
	Client::Client()
	{
		_isRunning = true;
		Init();
	}

	Client::~Client()
	{
	}

	void Client::Init()
	{
		m.lock();
		_network = std::make_unique<ClientNetwork>();
		_network->Init();
		m.unlock();

		_input = std::make_unique<ClientInput>();
		_input->Init();

		while (_isRunning)
		{
			if (_network->MustRefreshClientList())
			{
				_input->RefreshClientList(_network->GetAllClientsConnected());
			}

			if (_input->GetReadyToSend())
			{

				std::string text = _input->GetTextToSend();

				/*if (text[0] == 'm')
				{
					text.erase(0, 1);
					_network->ClientSendMessage(text.c_str(), MessageDestination::Destination_Multicast);
				}
				else if (text[0] == 's')
				{
					text.erase(0, 1);
					_network->ClientSendMessage(text.c_str(), MessageDestination::Destination_Server);
				}
				else if (text[0] == '@')
				{
					text.erase(0, 1);
					_network->ClientSendMessage(text.c_str(), MessageDestination::Destination_Client);
				}
				else
				{
					printf("can't parse the text in the client !\n");
				}*/

				if (text[0] == '@')
				{
					text.erase(0, 1);
					_network->ClientSendMessage(text.c_str(), MessageDestination::Destination_Client);
				}
				else
				{
					_network->ClientSendMessage(text.c_str(), MessageDestination::Destination_Multicast);
				}

				_input->HasSend();
			}
			if (_input->MustClose())
			{
				_isRunning = false;
			}

			if (_network->HasNewMessage())
			{
				std::string m (_network->GetNewMessage());
				_input->PrintMessage(m);
			}
		}
		
		Exit();

	}

	void Client::Exit()
	{
		_network->Stop();
		_input->Stop();
	}
}