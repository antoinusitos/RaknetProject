#include "Server.h"

namespace UserTemplate
{
	Server::Server()
	{
		_isRunning = true;
		Init();
	}

	Server::~Server()
	{
	}

	void Server::Init()
	{
		_network = std::make_unique<ServerNetwork>();
		while (_isRunning)
		{

		}

		Exit();
	}

	void Server::Exit()
	{
		_network.get()->Stop();
	}
}