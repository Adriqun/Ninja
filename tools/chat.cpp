#include "chat.h"
#include "logconsole.h"
#include "user.h"
#include "loading.h"
#include "boost/lexical_cast.hpp"

FPS Chat::fps;
std::vector <cmm::Text*> Chat::writtens;
std::vector <std::string> Chat::writtenStrs;

Chat::Chat()
{
	free();
}

Chat::~Chat()
{
	free();
}

void Chat::free()
{
	setStyleBlackish();
	screen_w = screen_h = 0;
	scale_x = scale_y = 0;
	max_text_length = 0xFF;

	globalYScale = 0;
	globalYLine = 2;

	active = false;
	used = false;

	if (!writtens.empty())
	{
		for (auto &it : writtens)
		{
			delete it;
			it = nullptr;
		}

		writtens.clear();
	}

	if (!writtenStrs.empty())
		writtenStrs.clear();
	
	line = 0.5;
	counter = 0;
}

void Chat::reset()
{
	globalYScale = 0;

	//active = false;
	//used = false;
	//fps.reset();
	//setWritten();

	for (auto &it : writtens)
		it->setAlpha(0);

	background.setAlpha(0);
	username.setAlpha(0);
	arrow.setAlpha(0);
}



void Chat::load(const float &screen_w, const float &screen_h)
{
	free();

	this->screen_w = screen_w;
	this->screen_h = screen_h;

	float fontSize = screen_h / 36;
	scale_x = screen_w / 2560;
	scale_y = screen_h / 1440;

	// Set writtens.
	for (int i = 0; i < 6; i++)
	{
		writtens.push_back(new cmm::Text);
		Loading::add(writtens[i]->setFont(cmm::JAPOKKI_FONT_PATH));
		if (Loading::isError())	return;
		writtens[i]->setText("");
		writtens[i]->setSize(fontSize);
		writtens[i]->setAlpha(0);
		writtenStrs.push_back("");
	}

	fps.load(screen_w, screen_h);
	if (Loading::isError())	return;
	fps.setRightTop();
	Loading::add(background.load("images/other/chat.png"));
	if (Loading::isError())	return;
	background.setScale(scale_x, scale_y);
	background.setPosition(0, screen_h - background.getHeight());
	background.setAlpha(0);

	arrow.setFont(cmm::JAPOKKI_FONT_PATH);
	arrow.setText("|");
	arrow.setSize(fontSize);
	arrow.setAlpha(0);

	Loading::add(username.setFont(cmm::JAPOKKI_FONT_PATH));
	if (Loading::isError())	return;
	username.setSize(fontSize);
	username.setFillColor(cmm::LogConsole::getRedColor());
	username.setAlpha(0);
	username.setText(cmm::User::getUsername() + ":");
	setWritten();

	background.setScale(scale_x, scale_y *globalYScale / globalYLine);
	username.setScale(1, 1 * globalYScale / globalYLine);

	for (auto &it : writtens)
		it->setScale(1, 1 * globalYScale / globalYLine);
}

void Chat::handle(const sf::Event &event)
{
	if (active)
	{
		if (globalYScale == globalYLine)
		{
			if (event.type == sf::Event::TextEntered)
			{
				if (event.text.unicode < 128 && event.text.unicode > 31)
				{
					writtenStrs[0] += event.text.unicode;
					setWritten();
				}
			}

			if (event.type == sf::Event::KeyPressed)
			{
				used = false;

				// BackSpace
				if (event.key.code == sf::Keyboard::BackSpace)
				{
					if (!writtenStrs[0].empty())
					{
						writtenStrs[0].pop_back();
						setWritten();
					}
				}

				// Enter, return.
				else if (event.key.code == sf::Keyboard::Enter && !writtenStrs[0].empty())
				{
					int i = 0;	// remove space from the beggining
					while (writtenStrs[0][i] == ' ')
					{
						writtenStrs[0].erase(i, 1);
						++i;
					}

					// remove space from the end
					if (!writtenStrs[0].empty())
					{
						i = writtenStrs[0].size() - 1;
						while (writtenStrs[0][i] == ' ')
						{
							writtenStrs[0].erase(i, 1);
							--i;
						}
					}

					if (!writtenStrs[0].empty())
					{
						prepareWritten(0, 0);

						for (unsigned i = writtenStrs.size() - 1; i >= 1; i--)
							writtenStrs[i] = writtenStrs[i - 1];

						writtenStrs[0].clear();

						setWritten();

						if (writtenStrs[1][0] == '@')
							used = true;
					}
					else
						setWritten();
				}

				// Esc.
				else if (event.key.code == sf::Keyboard::Escape)
					active = false;
			}
		}
	}
	else
	{
		if (event.type == sf::Event::KeyPressed)
		{
			// 'o'
			if (event.key.code == 14)
			{
				active = true;
				globalYScale = 0.1f;
			}
		}
	}
}

void Chat::draw(sf::RenderWindow* &window)
{
	window->draw(background.get());

	for (unsigned i = 0; i < writtens.size(); i++)
	{
		if (writtens[i]->getY() >= background.getY())
		{
			if (!writtenStrs[i].empty() && i != 0)
			{
				username.setPosition(screen_w / 256, writtens[i]->getY());
				window->draw(username.get());
			}

			window->draw(writtens[i]->get());
		}
	}

	// draw arrow.
	if (counter < line / 2)
	{
		window->draw(arrow.get());
	}

	fps.draw(window);
}

void Chat::mechanics(const double &elapsedTime)
{
	counter += static_cast<float>(elapsedTime);
	if (counter > line)
	{
		counter = 0;
	}

	int max = 0xFF;
	if (active)
	{
		float value = static_cast<float>(elapsedTime) * 255.0f * 2.0f;
		
		background.fadein(value, max);
		username.fadein(value, max);
		arrow.fadein(value, max);
		for (auto &it : writtens)
		{
			it->fadein(value, max);
		}

		if (globalYScale < globalYLine)
		{
			globalYScale += static_cast<float>(elapsedTime) * 5.0f;
		}
		else
		{
			globalYScale = globalYLine;
		}
	}
	else
	{
		float value = static_cast<float>(elapsedTime) * 255.0f * 2.0f;
		background.fadeout(value, max);
		username.fadeout(value, max);
		arrow.fadeout(value, max);
		for (auto &it : writtens)
		{
			it->fadeout(value, max);
		}

		if (globalYScale > 0)
		{
			globalYScale -= static_cast<float>(elapsedTime) * 5.0f;
		}
		else
		{
			globalYScale = 0;
		}
	}

	if (globalYScale != 0 && globalYScale != globalYLine)
	{
		background.setPosition(0, screen_h - background.getHeight() + screen_h / 288);


		background.setScale(scale_x, scale_y *globalYScale / globalYLine);
		username.setScale(1, 1 * globalYScale / globalYLine);

		for (auto &it : writtens)
		{
			it->setScale(1, 1 * globalYScale / globalYLine);
		}
	}

	fps.mechanics(elapsedTime);
}

void Chat::fadeout(const float &v, const int &min)
{
	fps.fadeout(v, min);
	background.fadeout(v, min);
	username.fadeout(v, min);
	for (auto &it : writtens)
	{
		it->fadeout(v, min);
	}
}



bool& Chat::isActive()
{
	return active;
}

bool Chat::isCommand()
{
	if (active && used)
	{
		used = false;

		if (compCommand("@fps"))
		{
			fps.setActive(!fps.isActive());
			return false;
		}

		return true;
	}

	return false;
}

bool Chat::compCommand(std::string line)
{
	return writtenStrs[1] == line;
}

bool Chat::isNewMusicVolume()
{
	size_t pos = writtenStrs[1].find("@music volume ");
	if (pos == std::string::npos)
	{
		pos = writtenStrs[1].find("@music ");	// do not delete this space!
		if (pos != std::string::npos && pos == 0 && writtenStrs[1].size() > 7)
			return true;
	}
	else if (pos == 0 && writtenStrs[1].size() > 14)
		return true;

	return false;
}

bool Chat::isNewSoundVolume()
{
	size_t pos = writtenStrs[1].find("@sound volume ");
	if (pos == std::string::npos)
	{
		pos = writtenStrs[1].find("@sound ");	// do not delete this space!
		if (pos != std::string::npos && pos == 0 && writtenStrs[1].size() > 7)
			return true;
	}
	else if (pos == 0 && writtenStrs[1].size() > 14)
		return true;

	return false;
}

float Chat::getNewVolume()
{
	int s = 7;
	size_t pos = writtenStrs[1].find("volume ");
	if (pos != std::string::npos)
	{
		if (pos == 7)
			s = 14;
		else
		{
			setError(" - command is written incorrectly.");
			return -1; // error, this command may look like this: @music blah blah volume 12
		}
	}
		
	std::string buf = writtenStrs[1].substr(s, writtenStrs[1].size() - 1);
	size_t size = buf.size();
	if (size <= 3 && size >= 1)
	{
		if (buf == "min")
			return 0.0f;
		else if (buf == "max")
			return 100.0f;
		else
		{
			// check if string contains only numbers
			for (unsigned i = 0; i <buf.size(); ++i)
			{
				if (buf[i] < '0' || buf[i] > '9')
				{
					setError(" - volume contains non-decimal characters.");
					return -1; // no decimal number detected - error
				}
			}

			if (s == 3 && (writtenStrs[1][0] != '1' && writtenStrs[1][0] != '0'))
			{
				setError(" - volume's range is from 0 to 100.");
				return -1; // it may look like this: @music volume 855, max is 100
			}
			
			// is ok
			return boost::lexical_cast<float>(buf);
		}
	}

	setError(" - volume is written incorrectly.");
	return -1;	// error
}




void Chat::setWritten()
{
	float y0 = screen_h - screen_h / 28;

	writtens[0]->setPosition(screen_w / 256, y0);
	if (!writtenStrs[0].empty())
	{
		if (writtenStrs[0][0] == '@')	writtens[0]->setFillColor(ccolor);
		else							writtens[0]->setFillColor(color);
	}

	for (unsigned i = 1; i < writtens.size(); i++)
	{
		writtens[i]->setText(writtenStrs[i]);

		float y_adder = screen_h / 26;
		for (unsigned j = 0; j < writtenStrs[i].size(); j++)
		{
			if (static_cast <int> (writtenStrs[i][j]) == 10)
			{
				y_adder += screen_h / 26;
			}
		}

		writtens[i]->setPosition(username.getRight() + screen_w / 256, writtens[i - 1]->getY() - y_adder);

		if (!writtenStrs[i].empty())
		{
			if (writtenStrs[i][0] == '@')		writtens[i]->setFillColor(ccolor);
			else								writtens[i]->setFillColor(color);
		}
	}

	writtens[0]->setText(writtenStrs[0]);

	if (writtens[0]->getRight() < background.getRight())	writtens[0]->setPosition(screen_w / 256, y0);
	else	writtens[0]->setPosition((background.getRight() - writtens[0]->getWidth()) - screen_w / 256, y0);

	if (writtenStrs[0].empty())	arrow.setPosition(0, y0);
	else	arrow.setPosition(writtens[0]->getRight(), y0);
}

void Chat::prepareWritten(int n, int k)
{
	// Add \n if needed.
	std::string buf = "";
	for (unsigned i = 0; i < writtenStrs[k].size(); i++)
	{
		buf += writtenStrs[k][i];
		writtens[n]->setText(buf);

		if (writtens[n]->getWidth() + username.getWidth() + screen_w / 64 >= background.getRight())
			buf = buf.substr(0, buf.size() - 1) + "\n" + buf[buf.size() - 1];
	}

	writtenStrs[k] = buf;
}

void Chat::setError(std::string msg)
{
	writtenStrs[1] += msg;
	prepareWritten(0, 1);
	setWritten();
}


void Chat::setStyleBlackish()
{
	color = sf::Color(0x68, 0x68, 0x68);
	ccolor = sf::Color(0, 0, 0);
}

void Chat::setStyleWhitish()
{
	color = sf::Color(0xBA, 0xBA, 0xBA);
	ccolor = sf::Color(0xFF, 0xFF, 0xFF);
}