#pragma once

#include <thread>
#include <memory>
#include <stdio.h>
#include <string>
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
		const char* GetTextToSend() const;

		void HasSend();

	private:
		void Init();

		void Exit();

		void HandleInputs();
	private:
		std::unique_ptr<std::thread> _inputThread;

		bool _isRunning;
		bool _readyToSend;

		std::string _textToSend;
	};
}