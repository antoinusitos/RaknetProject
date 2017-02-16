#pragma once

#include <thread>
#include <memory>
#include <stdio.h>
#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#define ESCAPE_KEY 27
#define ENTER_KEY 13
#define BACKSPACE_KEY 8

namespace UserTemplate
{
	class ClientInput
	{
	public:
		ClientInput(sf::RenderWindow& window);
		~ClientInput();

		void Stop();

		bool GetReadyToSend();
		const char* GetTextToSend() const;

		void HasSend();

		void Init(std::reference_wrapper<sf::RenderWindow> window);

	private:
		void Exit();

		void HandleInputs(sf::RenderWindow& window);
	private:
		std::thread _inputThread;

		bool _isRunning;
		bool _readyToSend;

		std::string _textToSend;
	};
}