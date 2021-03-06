#include "initialization.h"
#include "loading.h"
#include "definitions.h"

Initialization::Initialization()
{
	free();
}

Initialization::~Initialization()
{
	free();
}

void Initialization::free()
{
	state = 0;

	if (!texts.empty())
	{
		for (auto &it : texts)
		{
			delete it;
		}

		texts.clear();
	}

	prev = next = exit = false;
}



void Initialization::load(const float &screen_w, const float &screen_h)
{
	free();

	for (unsigned i = 0; i < AMOUNT; ++i)
		texts.push_back(new cmm::Text);

	// Set font.
	const char* path = cmm::JAPOKKI_FONT_PATH;
	for (auto &it : texts)
	{
		Loading::add(it->setFont(path));
		if (Loading::isError()) return;
	}

	// Set text.
	texts[AUTHOR]->setTextW(L"Adrian Micha\u0142ek");
	texts[PRESENTS]->setText("presents");
	texts[HALLOWEEN]->setText("Halloween");
	texts[HALLOWEEN]->setOutlineThickness(3);
	texts[COMBAT]->setText("Combat");
	texts[COMBAT]->setOutlineThickness(3);
	texts[SKIP]->setText("Press enter to skip introduction...");

	// Set size.
	for(unsigned i = 0; i < HALLOWEEN; ++i)
		texts[i]->setSize(screen_h / 24);
	texts[PRESENTS]->setSize(screen_h / 24);
	texts[HALLOWEEN]->setSize(screen_h / 10);
	texts[COMBAT]->setSize(screen_h / 10);
	texts[SKIP]->setSize(screen_h / 32);

	// Set color.
	texts[AUTHOR]->setFillColor(cmm::DULL_IRON_COLOR);
	texts[PRESENTS]->setFillColor(cmm::CYAN_COLOR);
	texts[HALLOWEEN]->setFillColor(cmm::BACKGROUND_COLOR);
	texts[HALLOWEEN]->setOutlineColor(cmm::RED_COLOR);
	texts[COMBAT]->setFillColor(cmm::BACKGROUND_COLOR);
	texts[COMBAT]->setOutlineColor(cmm::DULL_IRON_COLOR);
	texts[SKIP]->setFillColor(cmm::IRON_COLOR);

	// Set position.
	texts[AUTHOR]->center(screen_w / 2, screen_h / 2 - screen_h / 20);
	texts[PRESENTS]->setPosition(screen_w / 2 - texts[PRESENTS]->getWidth() / 2, texts[AUTHOR]->getBot());
	texts[COMBAT]->center(screen_w / 2 - texts[HALLOWEEN]->getWidth() / 2 - screen_h / TILE_HEIGHT - screen_h / 144, screen_h / 2 - screen_h / 20);
	texts[HALLOWEEN]->setPosition(texts[COMBAT]->getRight() + screen_h / 72 + screen_h / 16, texts[COMBAT]->getBot() - texts[HALLOWEEN]->getHeight());
	texts[SKIP]->setPosition((screen_w - texts[SKIP]->getWidth()) - (screen_h / 72), screen_h - texts[SKIP]->getHeight() - screen_h / 72);
}

void Initialization::handle(const sf::Event &event)
{
	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Enter)
		{
			next = true;
		}
	}
}

void Initialization::draw(sf::RenderWindow* &window)
{
	if (!next)
	{
		for(auto &it : texts)
			window->draw(*it);
	}
}

void Initialization::mechanics(const double &elapsedTime)
{
	if (!next)
	{
		int min = (int)MIN_ALPHA, max = (int)MAX_ALPHA;

		float velocity = static_cast<float>(elapsedTime) * MAX_ALPHA / 2;
		if (state == 0)
		{
			texts[AUTHOR]->fadein(velocity, max);
			texts[SKIP]->fadein(velocity, max);
		}
		else if (state == 1)
		{
			texts[PRESENTS]->fadein(velocity * 1.5f, max);
			texts[SKIP]->fadeout(velocity * 1.5f, min);
		}
		else if (state == 2)
		{
			texts[AUTHOR]->fadeout(velocity / 2, min);
			texts[PRESENTS]->fadeout(velocity / 2, min);
		}
		else if (state == 3)
		{
			texts[COMBAT]->fadein(velocity, max);
		}
		else if (state == 4)
		{
			texts[HALLOWEEN]->fadein(velocity, max);
		}
		else if (state == 5)
		{
			texts[COMBAT]->fadeout(velocity, min);
			texts[HALLOWEEN]->fadeout(velocity, min);
		}

		// 1
		if (texts[AUTHOR]->getAlpha() == MAX_ALPHA && state == 0)
		{
			state = 1;
		}
		else if (texts[PRESENTS]->getAlpha() == MAX_ALPHA && state == 1)
		{
			state = 2;
		}
		else if (texts[PRESENTS]->getAlpha() == 0 && state == 2)
		{
			state = 3;
		}

		// 2
		else if (texts[COMBAT]->getAlpha() == MAX_ALPHA && state == 3)
		{
			state = 4;
		}
		else if (texts[HALLOWEEN]->getAlpha() == MAX_ALPHA && state == 4)
		{
			state = 5;
		}
		else if (texts[HALLOWEEN]->getAlpha() == 0 && state == 5)
		{
			next = true;
		}
	}
}



bool Initialization::isReady() const
{
	return next;
}

void Initialization::setState(int &state)
{
	if (isReady())
	{
		state = cmm::LOGIN;
	}
}