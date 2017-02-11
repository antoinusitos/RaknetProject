#include "ClientInput.h"

namespace UserTemplate
{
	ClientInput::ClientInput()
	{
		_readyToSend = false;
		_isRunning = false;
		Init();
	}

	ClientInput::~ClientInput()
	{
	}

	void ClientInput::Stop()
	{
		_isRunning = false;
		Exit();
	}

	bool ClientInput::GetReadyToSend()
	{
		return _readyToSend;
	}

	const char* ClientInput::GetTextToSend() const
	{
		return _textToSend.c_str();
	}

	void ClientInput::HasSend()
	{
		_readyToSend = false;
		_textToSend = "";
	}

	void ClientInput::Init()
	{
		_isRunning = true;
		_inputThread = std::make_unique<std::thread>(&ClientInput::HandleInputs, this);
	}

	void ClientInput::Exit()
	{
	}

	void ClientInput::HandleInputs()
	{
		while (_isRunning)
		{
			if (!_readyToSend)
			{
				std::getline(std::cin, _textToSend);
				_readyToSend = true;
			}
		}
	}
}