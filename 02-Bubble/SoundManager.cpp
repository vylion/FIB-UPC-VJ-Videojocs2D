#include <algorithm>

#pragma comment(lib, "irrKlang.lib")
#include "SoundManager.h"



SoundManager::SoundManager()
{
	_engine = createIrrKlangDevice();
	_soundVolumeFactor = 1.f;
}


SoundManager::~SoundManager()
{
	_engine->drop();
}

void SoundManager::update()
{
	_engine->update();
}

void SoundManager::playSound(const char* fileName)
{
	//Play the sound only if it's not playing already (prevent overlap)
	if (!_engine->isCurrentlyPlaying(fileName))
		_engine->play2D(fileName, false);
}

float SoundManager::getMasterVolume()
{
	return _engine->getSoundVolume();
}

void SoundManager::setMasterVolume(float volume)
{
	_engine->setSoundVolume(volume);
}

void SoundManager::setMusic(const char * fileName)
{
	_engine->removeSoundSource(_music);
	_music = _engine->addSoundSourceFromFile(fileName);
	_engine->play2D(_music);
}

float SoundManager::getMusicVolume()
{
	return _music->getDefaultVolume();
}

void SoundManager::setMusicVolume(float volume)
{
	_music->setDefaultVolume(volume);
}

void SoundManager::addSound(const char * fileName)
{
	addSound(fileName, 1.0f);
}

void SoundManager::addSound(const char * fileName, float volume)
{
	ISoundSource *sound = _engine->getSoundSource(fileName, true);
	sound->setDefaultVolume(volume * _soundVolumeFactor);
	_sounds.push_back(sound);
}

float SoundManager::getSoundVolume()
{
	return _soundVolumeFactor;
}

void SoundManager::setSoundVolume(float volume)
{
	/*	1. Divide by _soundFactorVolume to restore original volume individually
		2. Multiply by val, or 0.01 to deafen the sound enough without new factor being 0 as that would result in
		   a division by 0 on the next volume change
	*/
	for (int i = 0; i < _sounds.size(); i++) {
		_sounds[i]->setDefaultVolume(
			_sounds[i]->getDefaultVolume() / _soundVolumeFactor * max(volume, 0.01f)
		);
	}
	_soundVolumeFactor = max(volume, 0.01f);
}

void SoundManager::setSoundVolume(const char * fileName, float volume)
{
	_engine->getSoundSource(fileName, true)->setDefaultVolume(volume);
}
