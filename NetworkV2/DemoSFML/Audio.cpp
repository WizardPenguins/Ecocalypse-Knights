#include "stdafx.h"
#include "Audio.h"

Audio::Audio()
{
	soundMultiplier = 0.5f;

	soundNumberTabMax[(int)playerSoundCategory::GET_HIT] = 1;
	soundNumberTabMax[(int)playerSoundCategory::DEATH] = 2;
	soundNumberTabMax[(int)playerSoundCategory::SPELL] = 2;
	soundNumberTabMax[(int)playerSoundCategory::SPELL_SCREAM] = 1;
	soundNumberTabMax[(int)playerSoundCategory::ATTACK] = 3;
	soundNumberTabMax[(int)playerSoundCategory::ATTACK_SCREAM] = 1;
	soundNumberTabMax[(int)playerSoundCategory::NOMANA] = 1;
	soundNumberTabMax[(int)playerSoundCategory::REZ] = 1;
	soundNumberTabMax[(int)playerSoundCategory::JUMP] = 3;
	soundNumberTabMax[(int)playerSoundCategory::HAZELNUT] = 1;
	soundNumberTabMax[(int)playerSoundCategory::MUSHROOM] = 2;
	soundNumberTabMax[(int)playerSoundCategory::PIG] = 2;
	soundNumberTabMax[(int)playerSoundCategory::BIRD] = 1;
	soundNumberTabMax[(int)playerSoundCategory::HAMOURAI] = 1;
	soundNumberTabMax[(int)playerSoundCategory::WINOAKY] = 1;
	soundNumberTabMax[(int)playerSoundCategory::OSEF] = INT_MAX;
}

Audio::~Audio()
{
	if (!soundMap.empty())
	{
		soundMap.erase(soundMap.begin(), soundMap.end());
		soundMap.clear();
	}
}

void Audio::CheckNumberSound()
{
	for (int i = 0; i < soundMapManager.size(); i++)
	{
		if (soundMap[soundMapManager[i].key].hasBeenPlayed && soundMap[soundMapManager[i].key].sound.getStatus() != sf::Sound::Playing)
		{
			soundMap[soundMapManager[i].key].hasBeenPlayed = false;
			soundNumberTab[(int)soundMap[soundMapManager[i].key].category]--;
		}
	}
}

void Audio::AddSound(const std::string& _fileName, const std::string& _key)
{
	for (unsigned char i = 0; i < soundMapManager.size(); i++)
	{
		if (soundMapManager[i].fileName == _fileName)
		{
			//std::cerr << "ERROR : SOUND ALREADY IN MAP" << std::endl;
			return;
		}
	}

	Sound temp = Sound();
	AudioMapManager temp2 = AudioMapManager();

	temp.buffer = new sf::SoundBuffer();
	temp.buffer->loadFromFile(_fileName);
	temp.sound.setBuffer(*(temp.buffer));
	soundMap[_key] = std::move(temp);

	temp2.fileName = _fileName;
	temp2.key = _key;
	soundMapManager.push_back(temp2);
}

void Audio::AddSound(const std::string& _fileName, const std::string& _key, playerSoundCategory _category)
{
	for (unsigned char i = 0; i < soundMapManager.size(); i++)
	{
		if (soundMapManager[i].fileName == _fileName)
		{
			//std::cerr << "ERROR : SOUND ALREADY IN MAP" << std::endl;
			return;
		}
	}

	Sound temp = Sound();
	AudioMapManager temp2 = AudioMapManager();

	temp.buffer = new sf::SoundBuffer();
	temp.buffer->loadFromFile(_fileName);
	temp.sound.setBuffer(*(temp.buffer));
	temp.hasBeenPlayed = false;
	temp.category = _category;
	soundMap[_key] = std::move(temp);

	temp2.fileName = _fileName;
	temp2.key = _key;

	soundMapManager.push_back(temp2);
}

void Audio::AddMusic(const std::string& _fileName, const std::string& _key)
{
	for (unsigned char i = 0; i < musicMapManager.size(); i++)
	{
		if (musicMapManager[i].fileName == _fileName)
		{
			//std::cerr << "ERROR : MUSIC ALREADY IN MAP" << std::endl;
			return;
		}
	}

	Music* temp = new Music;
	AudioMapManager temp2 = AudioMapManager();

	temp->music.openFromFile(_fileName);
	musicMap[_key] = std::move(temp);

	temp2.fileName = _fileName;
	temp2.key = _key;
	musicMapManager.push_back(temp2);
}

void Audio::PlaySoundNotRepeated(const std::string& _key, const float _volume)
{
	if ((soundMap[_key].sound.getStatus() == sf::Sound::Stopped
		|| soundMap[_key].sound.getStatus() == sf::Sound::Paused) && soundNumberTab[(int)soundMap[_key].category] < soundNumberTabMax[(int)soundMap[_key].category])
	{
		soundMap[_key].sound.setVolume(_volume * soundMultiplier);
		soundMap[_key].sound.play();
		soundMap[_key].hasBeenPlayed = true;
		soundNumberTab[(int)soundMap[_key].category]++;
	}
}

//void Audio::PlaySoundNotRepeated(const std::string& _key, playerSoundCategory _category, const float _volume)
//{
//	if ((soundMap[_key].sound.getStatus() == sf::Sound::Stopped
//		|| soundMap[_key].sound.getStatus() == sf::Sound::Paused) && soundNumberTab[(int)soundMap[_key].category] <= soundNumberTabMax[(int)soundMap[_key].category])
//	{
//		soundMap[_key].sound.setVolume(_volume * soundMultiplier);
//		soundMap[_key].sound.play();
//		soundMap[_key].hasBeenPlayed = true;
//		soundNumberTab[(int)soundMap[_key].category]++;
//	}
//}

void Audio::PlaySoundRepeated(const std::string& _key, const float _volume)
{
	soundMap[_key].sound.setVolume(_volume * soundMultiplier);
	soundMap[_key].sound.play();
}

void Audio::StopSound(const std::string& _key)
{
	if (soundMap[_key].sound.getStatus() == sf::Sound::Playing)
	{
		soundMap[_key].sound.stop();
	}
}

void Audio::StopAllSounds(void)
{
	for (auto it = soundMap.begin(); it != soundMap.end(); ++it)
	{
		if (it->second.sound.getStatus() == sf::Sound::Playing)
		{
			it->second.sound.stop();
		}
	}
}

void Audio::PauseAllSounds(void)
{
	for (auto it = soundMap.begin(); it != soundMap.end(); ++it)
	{
		if (it->second.sound.getStatus() == sf::Sound::Playing)
		{
			it->second.sound.pause();
		}
	}
}

void Audio::UnPauseAllSounds(void)
{
	for (auto it = soundMap.begin(); it != soundMap.end(); ++it)
	{
		if (it->second.sound.getStatus() == sf::Sound::Paused)
		{
			it->second.sound.play();
		}
	}
}

void Audio::PlayMusic(const std::string& _key, const bool _setLoop, const float _volume)
{
	for (auto it = musicMap.begin(); it != musicMap.end(); ++it)
	{
		it->second->volume = _volume;
	}
	if (musicMap[_key]->music.getStatus() == sf::Music::Stopped || musicMap[_key]->music.getStatus() == sf::Music::Paused)
	{
		musicMap[_key]->music.setPosition(Screen::GetWidth() / 2.f, Screen::GetHeight() / 2.f, 0.f);
		musicMap[_key]->music.setVolume(_volume * soundMultiplier);
		musicMap[_key]->music.setLoop(_setLoop);
		musicMap[_key]->music.play();
	}
}

void Audio::StopMusic(const std::string& _key)
{
	if (musicMap[_key]->music.getStatus() == sf::Music::Playing)
	{
		musicMap[_key]->music.stop();
	}
}

void Audio::StopAllMusics(void)
{
	for (auto it = musicMap.begin(); it != musicMap.end(); ++it)
	{
		if (it->second->music.getStatus() == sf::Music::Playing)
		{
			it->second->music.stop();
		}
	}
}

void Audio::PauseAllMusics(void)
{
	for (auto it = musicMap.begin(); it != musicMap.end(); ++it)
	{
		if (it->second->music.getStatus() == sf::Music::Playing)
		{
			it->second->music.pause();
		}
	}
}

void Audio::UnPauseAllMusics(void)
{
	for (auto it = musicMap.begin(); it != musicMap.end(); ++it)
	{
		if (it->second->music.getStatus() == sf::Music::Paused)
		{
			it->second->music.setPosition(Screen::GetWidth() / 2.f, Screen::GetHeight() / 2.f, 0.f);
			it->second->music.play();
		}
	}
}

const sf::Sound::Status Audio::GetSoundStatus(const std::string& _key)
{
	return soundMap[_key].sound.getStatus();
}

const float Audio::GetSoundVolume(const std::string& _key)
{
	return soundMap[_key].sound.getVolume();
}

const sf::SoundBuffer* Audio::GetSoundBuffer(const std::string& _key)
{
	return soundMap[_key].buffer;
}

sf::Sound Audio::GetSound(const std::string& _key)
{
	return soundMap[_key].sound;
}

void Audio::SetSoundVolume(const std::string& _key, const float _volume)
{
	soundMap[_key].sound.setVolume(_volume * soundMultiplier);
}

void Audio::SetAllSoundsVolume(void)
{
	for (auto it = soundMap.begin(); it != soundMap.end(); ++it)
	{
		it->second.sound.setVolume(it->second.sound.getVolume() * soundMultiplier);
	}
}

const sf::Music::Status Audio::GetMusicStatus(const std::string& _key)
{
	return musicMap[_key]->music.getStatus();
}

const float Audio::GetMusicVolume(const std::string& _key)
{
	return musicMap[_key]->music.getVolume();
}

void Audio::SetMusicVolume(const std::string& _key, const float _volume)
{
	musicMap[_key]->music.setVolume(_volume * soundMultiplier);
}

void Audio::SetAllMusicsVolume(void)
{
	for (auto it = musicMap.begin(); it != musicMap.end(); ++it)
	{
		it->second->music.setVolume(it->second->volume * soundMultiplier);
	}
}

const float& Audio::GetSoundMultiplier(void)
{
	return soundMultiplier;
}

void Audio::SetSoundMultiplier(const float _soundMultiplier)
{
	soundMultiplier = _soundMultiplier;
	SetAllMusicsVolume();
	SetAllSoundsVolume();
}

void Audio::ModifySoundMultiplier(const float _modifier)
{
	soundMultiplier += _modifier;
	SetAllMusicsVolume();
	SetAllSoundsVolume();
}

void Audio::SetPitch(const std::string& _key, const float _pitch)
{
	soundMap[_key].sound.setPitch(_pitch);
}

void Audio::FadeOutSound(const std::string& _key, float _dt)
{

	if (soundMultiplier != 0)
	{
		if (GetSoundStatus(_key) == sf::Sound::Status::Playing)
		{
			float volume = GetSoundVolume(_key) - _dt * 10.f;
			if (volume < 0.f)
			{

				StopSound(_key);
			}
			else
			{
				SetSoundVolume(_key, volume / soundMultiplier);
			}
		}
	}
}

void Audio::FadeInSound(const std::string& _key, float _dt)
{
	if (soundMultiplier != 0)
	{
		if (GetSoundStatus(_key) != sf::Sound::Status::Playing)
		{
			PlayMusic(_key, true, 0);
		}
		else
		{
			float volume = GetSoundVolume(_key) + _dt * 10.f;
			if (volume > 100.f)
			{

				volume = 100.f;
			}

			SetSoundVolume(_key, volume / soundMultiplier);

		}
	}
}

void Audio::FadeOutMusic(const std::string& _key, float _dt)
{

	if (soundMultiplier != 0)
	{
		if (GetMusicStatus(_key) == sf::Sound::Status::Playing)
		{
			float volume = GetMusicVolume(_key) - _dt * 10.f;
			if (volume < 0.f)
			{

				StopMusic(_key);
			}
			else
			{
				SetMusicVolume(_key, volume / soundMultiplier);
			}
		}
	}
}

void Audio::FadeInMusic(const std::string& _key, float _dt)
{
	if (soundMultiplier != 0)
	{
		if (GetMusicStatus(_key) != sf::Sound::Status::Playing)
		{
			PlayMusic(_key, true, 0);
		}
		else
		{
			float volume = GetMusicVolume(_key) + _dt * 10.f;
			if (volume > 100.f)
			{

				volume = 100.f;
			}

			SetMusicVolume(_key, volume / soundMultiplier);

		}
	}
}