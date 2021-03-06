#include "core.h"
#include "loading.h"
#include "platform.h"
#include "simulator.h"
#include "scontent.h"
#include "filemanager.h"

cmm::Core* core = new cmm::Core(0, cmm::BACKGROUND_COLOR);
std::vector<cmm::State*> states;

// Load function.
void loading_loop()
{
	float screen_w = (float)core->getWidth();
	float screen_h = (float)core->getHeight();

	while (core->open)
	{
		while (core->isEvent())
		{
			if (core->getEvent().type == sf::Event::Closed)
				core->open = false;
		}

		states[core->state]->draw(core->getWindow());
		states[core->state]->mechanics(core->getElapsedTime());

		switch (LoadingState::getState())
		{
		case 30:
			states.push_back(NULL);	// Init
			states.push_back(NULL);	// Login
			states.push_back(NULL);	// Menu
			states.push_back(NULL);	// Level Menu
			states.push_back(new Platform);
			states[cmm::PLATFORM]->load(screen_w, screen_h);
			break;

		case 40:
			states.push_back(NULL);	// Table
			states.push_back(NULL);	// Editor
			states.push_back(new Simulator);
			states[cmm::SIMULATOR]->load(screen_w, screen_h);
			break;
		}

		states[core->state]->setState(core->state);
		if (core->state != cmm::LOADING)// game starts from here
			break;
	}

	//delete states[cmm::LOADING];
	//states[cmm::LOADING] = nullptr;
	states.shrink_to_fit();
}

void main_loop()
{
	SContent::type = SContent::TYPE::FILE;
	SContent::path = "local/testownik" + std::string(cmm::LOCALFILE_EXTENSION);
	core->state = cmm::STATES::SIMULATOR; // Start test state

	while (core->open)
	{
		if (core->state != cmm::PLATFORM && core->state != cmm::SIMULATOR)	// this if is only for test reason
			break;

		// clear
		core->clear();

		// events, mouse, keyboard
		while (core->isEvent())
		{
			if (core->getEvent().type == sf::Event::Closed)
				core->open = false;

			states[core->state]->handle(core->getEvent());
		}

		states[core->state]->mechanics(core->getElapsedTime());
		states[core->state]->draw(core->getWindow());
		states[core->state]->setState(core->state);

		// display
		core->display();
	}
}

int main(int argc, char** argv)
{
	// Init only loading state.
	states.push_back(new Loading);
	if (Loading::isError())
		return -1;

	// Create window.
	core->create("Combat Halloween");
	states[cmm::LOADING]->load(core->getWidth(), core->getHeight());

	// Loops.
	loading_loop();
	main_loop();

	// Free states.
	for (auto &it : states)
	{
		delete it;
		it = nullptr;
	}
	states.clear();
	delete core;
	return 0;
}