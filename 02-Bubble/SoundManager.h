#pragma once

#include <string>
#include <vector>

#include <irrKlang.h>
using namespace irrklang;
using namespace std;

//Singleton
class SoundManager
{
public:
	SoundManager();
	~SoundManager();
	
	static SoundManager &instance() {
		static SoundManager sm;
		return sm;
	}

	//Needed to update the music!
	void update();
	void dropAll();

	float getMasterVolume();
	//Sets volume for all audio (0..1)
	void setMasterVolume(float volume);

	//Removes old music and starts playing the new file
	void setMusic(const char* fileName);
	
	bool isMusicPaused();
	//Pauses or unpauses music
	void pauseMusic(bool pause);
	void toggleMusicPause();
	float getMusicVolume();
	//Sets volume for music (0..1)
	void setMusicVolume(float volume);

	//Play an individual sound. Can't be paused or modified
	void playSound(const char* fileName);
	//Adds a sound to the array
	void addSound(const char* fileName);
	//Adds a sound to the array with a default volume
	void addSound(const char* fileName, float volume);
	float getSoundVolumeFactor();
	//Sets volume for all sounds (0..1)
	void setSoundVolumeFactor(float volume);
	//Sets volume for specified sound (0..1)
	void setSoundVolume(const char* fileName, float volume);

private:
	ISoundEngine *_music_engine, *_sound_engine;
	bool _musicIsPaused;
	vector<ISound*> _sounds;

	//Affects all sounds
	float _masterVolume;
	//Preserve individual sound values
	float _musicVolume, _soundVolume;

	ISound * isoundFromName(const char* fileName);
};

