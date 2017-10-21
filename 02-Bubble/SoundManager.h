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

	void playSound(const char* fileName);

	float getMasterVolume();
	//Sets volume for all audio (0..1)
	void setMasterVolume(float volume);

	void setMusic(const char* fileName);
	float getMusicVolume();
	//Sets volume for music (0..1)
	void setMusicVolume(float volume);

	//Adds a sound to the array
	void addSound(const char* fileName);
	//Adds a sound to the array with a default volume
	void addSound(const char* fileName, float volume);
	float getSoundVolume();
	//Sets volume for all sounds (0..1)
	void setSoundVolume(float volume);
	//Sets volume for specified sound (0..1)
	void setSoundVolume(const char* fileName, float volume);

private:
	ISoundEngine *_engine;
	ISoundSource *_music;
	vector<ISoundSource*> _sounds;
	float _soundVolumeFactor;
};

