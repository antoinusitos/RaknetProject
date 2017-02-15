#pragma once

#include <thread>
#include <memory>
#include <stdio.h>
#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>

#define ESCAPE_KEY 27
#define ENTER_KEY 13
#define BACKSPACE_KEY 8

namespace UserTemplate
{
	class ClientInput
	{
	public:
		ClientInput(const sf::RenderWindow& window);
		~ClientInput();

		void Stop();

		bool GetReadyToSend();
		const char* GetTextToSend() const;

		void HasSend();

	private:
		void Init(const sf::RenderWindow& window);

		void Exit();

		void HandleInputs(const sf::RenderWindow& window);
	private:
		std::unique_ptr<std::thread> _inputThread;

		bool _isRunning;
		bool _readyToSend;

		std::string _textToSend;
	};
}