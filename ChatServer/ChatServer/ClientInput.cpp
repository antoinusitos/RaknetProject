#include "ClientInput.h"

namespace UserTemplate
{
	ClientInput::ClientInput(const sf::RenderWindow& window)
	{
		_readyToSend = false;
		_isRunning = false;
		Init(window);
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

	void ClientInput::Init(const sf::RenderWindow& window)
	{
		_isRunning = true;
		_inputThread = std::make_unique<std::thread>(&ClientInput::HandleInputs, window, this);
	}

	void ClientInput::Exit()
	{
		_inputThread.get()->join();
	}

	void ClientInput::HandleInputs(const sf::RenderWindow& window)
	{
		while (_isRunning)
		{
			/*if (!_readyToSend)
			{
				std::getline(std::cin, _textToSend);
				_readyToSend = true;
			}*/
			// do until there is no more event to execute
			while (window.pollEvent(event))
			{
				// if we press a key on the keyboard
				if (event.type == sf::Event::TextEntered)
				{
					if (event.text.unicode < 128)
					{
						// escape key
						if (event.key.code == ESCAPE_KEY)
						{
							fprintf(stderr, "Stopping application... \n");
							_owner->Stop();
						}
						// backscape key
						else if (event.key.code == BACKSPACE_KEY)
						{
							if (_text.size() > 0)
								_text = _text.substr(0, _text.size() - 1);
						}
						// enter key
						else if (event.key.code == ENTER_KEY)
						{
							_owner->Send(_text);
							_text = "";
						}
						else
						{
							_textToSend += static_cast<char>(event.text.unicode);
							//_text += static_cast<char>(event.text.unicode);
						}
					}
				}
			}
		}
	}
}