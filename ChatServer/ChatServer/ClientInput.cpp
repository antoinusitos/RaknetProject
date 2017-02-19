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

	void ClientInput::RefreshClientList(std::vector<std::string> clients)
	{
		_tempClientConnected.clear();
		for (auto i = 0; i < clients.size(); i++)
		{
			_tempClientConnected.push_back(clients[i]);
		}
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

		_windowss.get()->create(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "custom Chat");

		while (_isRunning)
		{
			_windowss->clear(sf::Color::White);

			if(_clientConnected != _tempClientConnected && _tempClientConnected.size() > 0)
				_clientConnected = _tempClientConnected;

			// the debug help at the top
			sf::RectangleShape rectangleHelp(sf::Vector2f(SCREEN_WIDTH, 50));
			rectangleHelp.setPosition(sf::Vector2f(0, 0));
			rectangleHelp.setOutlineThickness(-5);
			rectangleHelp.setOutlineColor(sf::Color::Black);
			rectangleHelp.setFillColor(sf::Color::Green);
			_windowss->draw(rectangleHelp);

			sf::Text textHelp;
			textHelp.setFont(_font);
			textHelp.setString("Enter a text and press Enter to Send (use @Name to send to a specific person)");
			textHelp.setCharacterSize(24);
			textHelp.setFillColor(sf::Color::Black);
			textHelp.setPosition(10.0f, 10.0f);
			_windowss->draw(textHelp);

			// print all the connected client
			sf::RectangleShape rectangleConnected(sf::Vector2f(200, SCREEN_HEIGHT));
			rectangleConnected.setPosition(sf::Vector2f(SCREEN_WIDTH - 200, 0));
			rectangleConnected.setFillColor(sf::Color::Cyan);
			rectangleConnected.setOutlineThickness(-5);
			rectangleConnected.setOutlineColor(sf::Color::Black);
			_windowss->draw(rectangleConnected);

			sf::Text textConnected;
			textConnected.setFont(_font);
			textConnected.setString("Connected :");
			textConnected.setCharacterSize(24);
			textConnected.setFillColor(sf::Color::Black);
			textConnected.setPosition(SCREEN_WIDTH - 190, 20);
			_windowss->draw(textConnected);

			float indexConnected = 1.0f;
			for (auto it : _clientConnected)
			{
				//if (it != "")
				//{
					sf::Text clientConnected;
					clientConnected.setFont(_font);
					clientConnected.setString(it);
					clientConnected.setCharacterSize(24);
					clientConnected.setFillColor(sf::Color::Black);
					clientConnected.setPosition(SCREEN_WIDTH - 190, 20 + (indexConnected * 20.0f));
					indexConnected += 1.0f;
					_windowss->draw(clientConnected);
				//}
			}

			// the rectangle of the input
			sf::RectangleShape rectangle(sf::Vector2f(SCREEN_WIDTH, 50));
			rectangle.setPosition(sf::Vector2f(0, SCREEN_HEIGHT - 50));
			rectangle.setFillColor(sf::Color::Green);
			rectangle.setOutlineThickness(-5);
			rectangle.setOutlineColor(sf::Color::Black);
			_windowss->draw(rectangle);

			float index = 1.0f;
			for (auto i = 0; i < _textToShow.size(); i++)
			{
				sf::Text text;
				text.setFont(_font);
				text.setString(_textToShow[i]);
				text.setCharacterSize(24);
				text.setFillColor(sf::Color::Black);
				text.setPosition(0.0f, 50 + (index * 20.0f));
				index+=1.0f;
				_windowss->draw(text);
			}

			sf::Text text;
			text.setFont(_font);
			text.setString(_textToSend);
			text.setCharacterSize(24);
			text.setFillColor(sf::Color::Black);
			text.setPosition(10.0f, SCREEN_HEIGHT - 40);
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