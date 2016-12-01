#pragma once
#include "Resources.h"

class Sound
{
private:
	//Sounds Fx
	Mix_Chunk  *bumper_sound, *destroy_sound, *jump_sound, *star_sound;

	//Musique
	Mix_Music  *musique;

public:
	//Constructeur
	Sound(char filename[200]);

	//Fonctions
	void loadSound();
	void playSoundFx(int);

	//Destructeur
	~Sound();
};

