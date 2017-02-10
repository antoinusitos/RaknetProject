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

	char * ClientInput::GetTextToSend()
	{
		return _textToSend;
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
			_textToSend = new char[512];
			std::cin.getline(_textToSend, 512);
			_readyToSend = true;
		}
	}
}