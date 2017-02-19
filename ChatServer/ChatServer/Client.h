#pragma once
#include "User.h"
#include <memory>
#include "ClientInput.h"
#include "ClientNetwork.h"
#include <mutex>
#include <vector>

namespace UserTemplate
{
	class Client : public User
	{
	public:
		Client();
		~Client();

	private:
		void Init();
		void Exit();

	private:
		std::unique_ptr<ClientInput> _input;
		std::unique_ptr<ClientNetwork> _network;

		bool _isRunning;

		std::mutex m;
	};
}