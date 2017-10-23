#include <algorithm>

#pragma comment(lib, "irrKlang.lib")
#include "SoundManager.h"



SoundManager::SoundManager()
{
	_masterVolume = 1.f;

	_music_engine = createIrrKlangDevice();
	_musicVolume = 1.f;

	_sound_engine = createIrrKlangDevice();
	_soundVolume = 1.f;
}


SoundManager::~SoundManager()
{
	_music_engine->drop();
	for (unsigned int i = 0; i < _sounds.size(); i++) {
		_sounds[i]->drop();
	}
	_sound_engine->drop();
}


/*********************************************UPDATE*****************************************************/


void SoundManager::update()
{
	_music_engine->update();

	//Refresh finished isounds for the next time they are played
	for (unsigned int i = 0; i < _sounds.size(); i++) {
		if (_sounds[i]->isFinished())
			_sounds[i] = _sound_engine->play2D(_sounds[i]->getSoundSource(), false, true);
	}
	_sound_engine->update();
}


/*********************************************MASTER*****************************************************/


float SoundManager::getMasterVolume()
{
	return _masterVolume;
}

void SoundManager::setMasterVolume(float volume)
{
	_masterVolume = volume;
	_music_engine->setSoundVolume(_masterVolume * _musicVolume);
	_sound_engine->setSoundVolume(_masterVolume * _soundVolume);
}


/*********************************************MUSIC******************************************************/



void SoundManager::setMusic(const char * fileName)
{
	//Remove old music
	_music_engine->removeAllSoundSources();
	//Read new music from file
	_music = _music_engine->play2D(fileName, true, true);
	//Start music on a loop
}

void SoundManager::pauseMusic(bool pause)
{
	//_music->setIsPaused(pause);
	_music_engine->setAllSoundsPaused(pause);
	//_engine->setAllSoundsPaused(pause);
}

float SoundManager::getMusicVolume()
{
	return _music_engine->getSoundVolume();
}

void SoundManager::setMusicVolume(float volume)
{
	//_engine->setSoundVolume(volume);
	if (_music != nullptr)
		_music->setVolume(volume);
}


/*********************************************SOUNDS****************************************************/



void SoundManager::playSound(const char* fileName)
{
	//Unpause sound in case it's loaded and paused

	ISound *isoundptr = isoundFromName(fileName);
	//Sound already loaded and ready to play or playing
	if (isoundptr != nullptr) {
		//Ready to play but paused
		if (isoundptr->getIsPaused())
			isoundptr->setIsPaused(false);
	}
	//Sound not loaded
	else addSound(fileName);

	_sound_engine->play2D(fileName, false);
	//Play the sound only if it's not playing already (prevent overlap)
	//if (!_sound_engine->isCurrentlyPlaying(fileName))
	//	_sound_engine->play2D(fileName, false);

}


void SoundManager::addSound(const char * fileName)
{
	addSound(fileName, 1.0f);
}

void SoundManager::addSound(const char * fileName, float volume)
{
	ISound *sound = _sound_engine->play2D(fileName, false, true);

	//ISoundSource *sound = _engine->getSoundSource(fileName, true);
	//sound->setDefaultVolume(volume * _soundVolumeFactor);
	sound->setVolume(volume);
	//sound->setIsPaused(false);
	_sounds.push_back(sound);
}

float SoundManager::getSoundVolumeFactor()
{
	return _soundVolume;
}

void SoundManager::setSoundVolumeFactor(float factor)
{
	/*	1. Divide by _soundFactorVolume to restore original volume individually
		2. Multiply by val, or 0.01 to deafen the sound enough without new factor being 0 as that would result in
		   a division by 0 on the next volume change
	*/
	/*
	for (unsigned int i = 0; i < _sounds.size(); i++) {
		_sounds[i]->setVolume(
			//_sounds[i]->getVolume() / _soundVolumeFactor * max(volume, 0.01f)
			_sounds[i]->getSoundSource()->getDefaultVolume() * max(factor, 0.01f)
		);
	}*/

	_soundVolume = max(factor, 0.01f);
	_sound_engine->setSoundVolume(_soundVolume * _masterVolume);
}

void SoundManager::setSoundVolume(const char * fileName, float volume)
{
	ISound *isoundptr = isoundFromName(fileName);
	if (isoundptr != nullptr) {
		isoundptr->setVolume(volume);
	}
	else addSound(fileName, volume);
}

ISound * SoundManager::isoundFromName(const char * fileName)
{

	for (unsigned int i = 0; i < _sounds.size(); i++) {
		char* name = (char*)_sounds[i]->getSoundSource()->getName();

		if ((char*)_sounds[i]->getSoundSource()->getName() == fileName) {

			return _sounds[i];
		}
	}
	return nullptr;
}
