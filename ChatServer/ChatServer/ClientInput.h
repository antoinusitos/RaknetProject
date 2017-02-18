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
		ClientInput();
		~ClientInput();

		void Stop();

		bool GetReadyToSend();
		const char* GetTextToSend() const;

		bool MustClose();

		void HasSend();

		void Init();

		void PrintMessage(std::string theMessage);

	private:
		void Exit();

		void HandleInputs();
	private:
		std::thread _inputThread;

		bool _isRunning;
		bool _readyToSend;
		bool _mustClose;

		std::string _textToSend;

		std::unique_ptr<sf::RenderWindow> _windowss;

		std::vector<std::string> _textToShow;
		sf::Font _font;


	};
}