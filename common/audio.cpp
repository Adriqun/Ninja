#include "audio.h"
#include "converter.h"
#include "definitions.h"
#include <boost/filesystem.hpp>
#include <fstream>


bool cmm::SoundData::sound_playable = true;
float cmm::SoundData::sound_volume = START_SOUND_VOLUME;
bool cmm::MusicData::music_playable = true;
float cmm::MusicData::music_volume = START_MUSIC_VOLUME;

cmm::SoundData::SoundData()
{
	// ...
}

cmm::SoundData::~SoundData()
{
	// ...
}

cmm::MusicData::MusicData()
{
	// ...
}

cmm::MusicData::~MusicData()
{
	// ...
}

cmm::Audio::Audio()
{
	// ...
}

cmm::Audio::~Audio()
{
	// ...
}

const bool& cmm::Audio::isSoundPlayable()
{
	return sound_playable;
}

const bool& cmm::Audio::isMusicPlayable()
{
	return music_playable;
}

const float& cmm::Audio::getSoundVolume()
{
	return sound_volume;
}

const float& cmm::Audio::getMusicVolume()
{
	return music_volume;
}

void cmm::Audio::resetAudio()
{
	sound_playable = true;
	sound_volume = START_SOUND_VOLUME;
	music_playable = true;
	music_volume = START_MUSIC_VOLUME;
	saveAudio();
}

void cmm::Audio::saveAudio()
{
	// Create directory if it does not exist
	if (!boost::filesystem::is_directory("local"))
		boost::filesystem::create_directories("local");

	std::ofstream file;
	file.open("local/audio.settings");

	file << std::to_string(sound_playable) << cmm::SSPACE << std::to_string(static_cast<int>(sound_volume)) << std::endl;
	file << std::to_string(music_playable) << cmm::SSPACE << std::to_string(static_cast<int>(music_volume));

	file.close();	// not needed actually
}

void cmm::Audio::loadAudio()
{
	std::fstream file;
	file.open("local/audio.settings");
	if (!file.is_open())	// file does not exist -> reset audio
	{
		file.close();
		resetAudio();
	}
	else
	{
		std::string line = cmm::SEMPTY;
		int counter = 0;
		while (std::getline(file, line))
		{
			if (counter > 1)
			{
				file.close();
				resetAudio();
				break;
			}

			if ((line[0] != '0' && line[0] != '1') || line.size() < 3 || line.size() > 5) // file is broken
			{
				file.close();
				resetAudio();
				break;
			}
			else
			{
				size_t size = line.size();
				if (size == 5 && (line[2] != '1' || line[3] != '0' || line[4] != '0')) // max is 100
				{
					file.close();
					resetAudio();
					break;
				}

				if (size >= 4 && (line[3] < '0' || line[3] > '9')) // no digit
				{
					file.close();
					resetAudio();
					break;
				}

				if (size >= 3 && (line[2] < '0' || line[2] > '9')) // no digit
				{
					file.close();
					resetAudio();
					break;
				}

				if (counter == 0) // sound
				{
					sound_playable = boost::lexical_cast<int>(line[0]);
					sound_volume = boost::lexical_cast<float>(line.substr(2, size - 2));
				}
				else
				{
					music_playable = boost::lexical_cast<int>(line[0]);
					music_volume = boost::lexical_cast<float>(line.substr(2, size - 2));
				}

				++counter;
			}
		}
	}
	file.close();
}