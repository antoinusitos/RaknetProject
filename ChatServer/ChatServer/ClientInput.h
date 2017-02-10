#pragma once

#include <thread>
#include <memory>
#include <stdio.h>
#include <string.h>
#include <iostream>

namespace UserTemplate
{
	class ClientInput
	{
	public:
		ClientInput();
		~ClientInput();

		void Stop();

		bool GetReadyToSend();
		char* GetTextToSend();

		void HasSend();

	private:
		void Init();

		void Exit();

		void HandleInputs();
	private:
		std::unique_ptr<std::thread> _inputThread;

		bool _isRunning;
		bool _readyToSend;

		char* _textToSend;
	};
}