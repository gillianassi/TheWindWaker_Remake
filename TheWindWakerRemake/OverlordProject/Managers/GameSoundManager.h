#pragma once
#include "Utils/Singleton.h"
class GameSoundManager final : public Singleton<GameSoundManager>
{
public:

	enum class Sound
	{
		ThemeSong,
		OutsetIsland,
		GreenRupee,
		BlueRupee,
		YellowRupee,
		Link_Hop1,
		Link_Hop2,
		Link_Hop3,
		Link_Roll_Base,
		Link_Roll1,
		Link_Roll2,
		Link_Roll3,
		Menu_Back,
		Menu_Close,
		Menu_Cursor,
		Menu_Open,
		Menu_Quit,
		Menu_Select,
		PressStart,
		winner
	};

	GameSoundManager(const GameSoundManager& other) = delete;
	GameSoundManager(GameSoundManager&& other) noexcept = delete;
	GameSoundManager& operator=(const GameSoundManager& other) = delete;
	GameSoundManager& operator=(GameSoundManager&& other) noexcept = delete;
	void AddSound(Sound soundID, const std::string& path);

	void Play2DSound(Sound soundID, bool isMusic = false);

	void LateInitialise();

protected:
	void Initialize() override {};

private:
	friend Singleton<GameSoundManager>;
	GameSoundManager();
	~GameSoundManager() = default;


	FMOD::Channel* m_pMusicChannel{};
	FMOD::Channel* m_pSFXChannel{};
	FMOD::System* pSoundSystem{};
	std::unordered_map<Sound, FMOD::Sound*> m_SoundMap{};
};




