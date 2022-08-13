#include "stdafx.h"
#include "GameSoundManager.h"

GameSoundManager::GameSoundManager()
{
	Initialize();
}

void GameSoundManager::LateInitialise()
{
	pSoundSystem = SoundManager::Get()->GetSystem();
}
void GameSoundManager::AddSound(Sound soundID, const std::string& path)
{
	if (m_SoundMap.contains(soundID))
		return;

	FMOD::Sound* pSound{};
	FMOD_RESULT result{};
	result = pSoundSystem->createStream(path.c_str(), FMOD_DEFAULT, nullptr, &pSound);
	m_SoundMap.insert({ soundID, pSound });

}

void GameSoundManager::Play2DSound(Sound soundID, bool isMusic)
{
	if (!m_SoundMap.contains(soundID))
	{
		std::cout << " no such sound with id: " << (int)soundID << std::endl;
		return;
	}


	FMOD::Sound* pSound = m_SoundMap.at(soundID);

	if (pSound == nullptr)
	{
		std::cout << " Check your path! No sound found with id: " << (int)soundID << std::endl;
		return;
	}
	if (!isMusic)
	{
		pSoundSystem->playSound(pSound, nullptr, false, &m_pSFXChannel);
	}
	else
	{
		pSoundSystem->playSound(pSound, nullptr, false, &m_pMusicChannel);
	}

}


