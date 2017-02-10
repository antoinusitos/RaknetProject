#include "Client.h"

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

		m.unlock();
		_input = std::make_unique<ClientInput>();

		while (_isRunning)
		{
			if (_input->GetReadyToSend())
			{

				char* text = _input->GetTextToSend();

				if (text[0] == 'm')
				{
					text[0] = ' ';
					_network->ClientSendMessage(text, UserTemplate::ClientNetwork::MessageDestination::Destination_Multicast);
				}
				else if (text[0] == 's')
				{
					text[0] = ' ';
					_network->ClientSendMessage(text, UserTemplate::ClientNetwork::MessageDestination::Destination_Server);
				}
				else
				{
					printf("can't parse the text in the client !\n");
				}

				_input->HasSend();
			}
		}

		Exit();
	}

	void Client::Exit()
	{
		_network.get()->Stop();
		_input.get()->Stop();
	}
}