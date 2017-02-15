#include "ClientInput.h"

namespace UserTemplate
{

	ClientInput::ClientInput(sf::RenderWindow& window)
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

	void ClientInput::Init(sf::RenderWindow& window)
	{
		_isRunning = true;
		_inputThread = std::make_unique<std::thread>(&ClientInput::HandleInputs, this);

		/*std::thread t([this, &window]()
		{
			while (this->_isRunning)
			{
				sf::Event theEvent;
				//while (window.pollEvent(theEvent))
				while (window.pollEvent(theEvent))
				{
					// if we press a key on the keyboard
					if (!_readyToSend && theEvent.type == sf::Event::TextEntered)
					{
						if (theEvent.text.unicode < 128)
						{
							// escape key
							if (theEvent.key.code == ESCAPE_KEY)
							{
								fprintf(stderr, "Stopping application... \n");
							}
							// backscape key
							else if (theEvent.key.code == BACKSPACE_KEY)
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
					}
				}
			}
		});
		t.join();*/
	}

	void ClientInput::Exit()
	{
		_inputThread.get()->join();
	}

	void ClientInput::HandleInputs(/*sf::RenderWindow& window*/)
	{
		while (_isRunning)
		{
			if (!_readyToSend)
			{
				std::getline(std::cin, _textToSend);
				_readyToSend = true;
			}
			// do until there is no more event to execute
			/*sf::Event event;
			while (window.pollEvent(event))
			{
				// if we press a key on the keyboard
				if (!_readyToSend && event.type == sf::Event::TextEntered)
				{
					if (event.text.unicode < 128)
					{
						// escape key
						if (event.key.code == ESCAPE_KEY)
						{
							fprintf(stderr, "Stopping application... \n");
						}
						// backscape key
						else if (event.key.code == BACKSPACE_KEY)
						{
							if (_textToSend.size() > 0)
								_textToSend = _textToSend.substr(0, _textToSend.size() - 1);
						}
						// enter key
						else if (event.key.code == ENTER_KEY)
						{
							_readyToSend = true;
						}
						else
						{
							_textToSend += static_cast<char>(event.text.unicode);
							//_text += static_cast<char>(event.text.unicode);
						}
					}
				}
			}*/
		}
	}
}