#pragma once
#include "User.h"
#include <memory>
#include "ServerNetwork.h"

namespace UserTemplate
{
	class Server : public User
	{
	public:
		Server();
		~Server();

	private:
		void Init();
		void Exit();

	private:
		std::unique_ptr<ServerNetwork> _network;

		bool _isRunning;
	};
}