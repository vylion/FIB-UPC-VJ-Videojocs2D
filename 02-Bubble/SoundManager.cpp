#include <algorithm>

#pragma comment(lib, "irrKlang.lib")
#include "SoundManager.h"



SoundManager::SoundManager()
{
	_masterVolume = 1.f;

	_music_engine = createIrrKlangDevice();
	_musicVolume = 1.f;
	_musicIsPaused = false;

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
	//Read new music from file and start on loop
	_music_engine->play2D(fileName, true);
}



bool SoundManager::isMusicPaused()
{
	return _musicIsPaused;
}

void SoundManager::pauseMusic(bool pause)
{
	_musicIsPaused = pause;
	_music_engine->setAllSoundsPaused(pause);
}

void SoundManager::toggleMusicPause()
{
	pauseMusic(!isMusicPaused());
}

float SoundManager::getMusicVolume()
{
	return _music_engine->getSoundVolume();
}

void SoundManager::setMusicVolume(float volume)
{
	_music_engine->setSoundVolume(volume * _masterVolume);
}


/*********************************************SOUNDS****************************************************/



void SoundManager::playSound(const char* fileName)
{
	_sound_engine->play2D(fileName);
}


void SoundManager::addSound(const char * fileName)
{
	addSound(fileName, 1.0f);
}

void SoundManager::addSound(const char * fileName, float volume)
{
	ISound *sound = _sound_engine->play2D(fileName, false, true);
	sound->setVolume(volume);
	_sounds.push_back(sound);
}

float SoundManager::getSoundVolumeFactor()
{
	return _soundVolume;
}

void SoundManager::setSoundVolumeFactor(float factor)
{
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
