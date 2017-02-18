#include "ClientInput.h"

namespace UserTemplate
{

	ClientInput::ClientInput()
	{
		_readyToSend = false;
		_isRunning = false;
		_mustClose = false;

		if (!_font.loadFromFile("arial.ttf"))
		{
			printf("ERROR : cannot load font 'arial.ttf'");
		}
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

	bool ClientInput::MustClose()
	{
		return _mustClose;
	}

	void ClientInput::HasSend()
	{
		_readyToSend = false;
		_textToSend = "";
	}

	void ClientInput::Init()
	{

		_isRunning = true;

		_inputThread = std::thread( &ClientInput::HandleInputs, this );
		
	}

	void ClientInput::PrintMessage(std::string theMessage)
	{
		_textToShow.push_back(theMessage);
	}

	void ClientInput::Exit()
	{
		_inputThread.join();
	}

	void ClientInput::HandleInputs()
	{
		sf::Event theEvent;
		sf::Context context;

		_windowss = std::make_unique<sf::RenderWindow>();

		_windowss.get()->create(sf::VideoMode(800, 600), "custom Chat");

		while (_isRunning)
		{
			/*if (!_readyToSend)
			{
				std::getline(std::cin, _textToSend);
				_readyToSend = true;
			}*/


			_windowss->clear(sf::Color::Blue);

			float index = 0.0f;
			for (auto i = 0; i < _textToShow.size(); i++)
			{
				sf::Text text;
				text.setFont(_font);
				text.setString(_textToShow[i]);
				text.setCharacterSize(24);
				text.setFillColor(sf::Color::Red);
				text.setPosition(0.0f, index * 20.0f);
				index+=1.0f;
				_windowss->draw(text);
			}

			sf::Text text;
			text.setFont(_font);
			text.setString(_textToSend);
			text.setCharacterSize(24);
			text.setFillColor(sf::Color::Red);
			text.setPosition(0.0f, 550.0f);
			_windowss->draw(text);

			// do until there is no more event to execute
			while (_windowss->pollEvent(theEvent))
			{
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
						}
					}
				}
				else if (theEvent.type == sf::Event::Closed)
				{
					_isRunning = false;
				}

				_windowss->display();
			}
		}
		_mustClose = true;
		_windowss->close();
	}
}