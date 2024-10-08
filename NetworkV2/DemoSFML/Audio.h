#ifndef  AUDIO_H
#define AUDIO_H

#include"Common.h"
#include"Screen.h"
#include "stdafx.h"

#define MAX_SOUNDS 2

enum class playerSoundCategory
{
	GET_HIT,
	DEATH,
	SPELL,
	SPELL_SCREAM,
	ATTACK,
	ATTACK_SCREAM,
	NOMANA,
	REZ,
	JUMP,
	HAZELNUT,
	MUSHROOM,
	PIG,
	BIRD,
	HAMOURAI,
	WINOAKY,
	OSEF,

	COUNT
};

class Sound
{
public:
	sf::Sound sound;
	sf::SoundBuffer* buffer = nullptr;
	bool hasBeenPlayed;
	playerSoundCategory category;
};

class Music
{
public:
	Music() {};
	~Music() {};
	sf::Music music;
	float volume = 100.f;
};

class AudioMapManager
{
public:
	std::string key;
	std::string fileName;
};


class Audio
{
public:
	Audio();
	~Audio();

	void CheckNumberSound();
	void AddSound(const std::string& _fileName, const std::string& _key);
	void AddSound(const std::string& _fileName, const std::string& _key, playerSoundCategory _category);
	void AddMusic(const std::string& _fileName, const std::string& _key);
	void PlaySoundNotRepeated(const std::string& _key, const float _volume = 100.f);
	//void PlaySoundNotRepeated(const std::string& _key, playerSoundCategory _category, const float _volume = 100.f);
	void PlaySoundRepeated(const std::string& _key, const float _volume = 100.f);
	void StopSound(const std::string& _key);
	void StopAllSounds(void);
	void PauseAllSounds(void);
	void UnPauseAllSounds(void);
	void PlayMusic(const std::string& _key, const bool _setLoop, const float _volume = 100.f);
	void StopMusic(const std::string& _key);
	void StopAllMusics(void);
	void PauseAllMusics(void);
	void UnPauseAllMusics(void);
	const sf::Sound::Status GetSoundStatus(const std::string& _key);
	const float GetSoundVolume(const std::string& _key);
	const sf::SoundBuffer* GetSoundBuffer(const std::string& _key);
	sf::Sound GetSound(const std::string& _key);
	void SetSoundVolume(const std::string& _key, const float _volume = 100.f);
	void SetAllSoundsVolume(void);
	const sf::Music::Status GetMusicStatus(const std::string& _key);
	const float GetMusicVolume(const std::string& _key);
	void SetMusicVolume(const std::string& _key, const float _volume = 100.f);
	void SetAllMusicsVolume(void);
	const float& GetSoundMultiplier(void);
	void SetSoundMultiplier(const float _soundMultiplier);
	void ModifySoundMultiplier(const float _modifier);
	void SetPitch(const std::string& _key, const float _pitch);
	void FadeOutMusic(const std::string& _key,float _dt);
	void FadeInMusic(const std::string& _key,float _dt);
	void FadeOutSound(const std::string& _key, float _dt);
	void FadeInSound(const std::string& _key, float _dt);
private:
	int soundNumberTab[(int)playerSoundCategory::COUNT];
	int soundNumberTabMax[(int)playerSoundCategory::COUNT];

	std::vector<AudioMapManager> soundMapManager;
	std::unordered_map<std::string, Sound> soundMap;

	std::vector<AudioMapManager> musicMapManager;
	std::unordered_map<std::string, Music*> musicMap;

	float soundMultiplier = 0.f;
};
#endif // ! AUDIO_H