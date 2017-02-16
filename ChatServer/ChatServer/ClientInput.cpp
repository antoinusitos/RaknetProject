#include "ClientInput.h"

namespace UserTemplate
{

	ClientInput::ClientInput(sf::RenderWindow& window)
	{
		_readyToSend = false;
		_isRunning = false;
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

	void ClientInput::Init(std::reference_wrapper<sf::RenderWindow> window)
	{
		_isRunning = true;

		_inputThread = std::thread{ &ClientInput::HandleInputs, this, window };
		
	}

	void ClientInput::Exit()
	{
		_inputThread.join();
	}

	void ClientInput::HandleInputs(sf::RenderWindow& window)
	{
		sf::Event theEvent;
		while (_isRunning)
		{
			/*if (!_readyToSend)
			{
				std::getline(std::cin, _textToSend);
				_readyToSend = true;
			}*/

			// do until there is no more event to execute
			if (!window.hasFocus())
			{
				printf("p");
			}
			if(window.pollEvent(theEvent))
			{
				/*if (!window.isOpen()) return;
				// if we press a key on the keyboard
				if (!_readyToSend && theEvent.type == sf::Event::TextEntered)
				{
					if (theEvent.text.unicode < 128)
					{
						// backscape key
						if (theEvent.key.code == BACKSPACE_KEY)
						{
							if (_textToSend.size() > 0)
								_textToSend = _textToSend.substr(0, _textToSend.size() - 1);
						}
						// enter key
						else if (theEvent.key.code == ENTER_KEY)
						{
							_readyToSend = true;
						}
						else
						{
							_textToSend += static_cast<char>(theEvent.text.unicode);
							//_text += static_cast<char>(event.text.unicode);
						}
					}
				}*/
			}
		}
	}
}