#include "Sound.h"


//Constructeur
Sound::Sound(char filename[200])
{
	/* On lib�re la chanson pr�c�dente s'il y en a une */
	if (musique != NULL)
	{
		Mix_HaltMusic();
		Mix_FreeMusic(musique);
	}

	/* On charge la nouvelle chanson */
	musique = Mix_LoadMUS(filename);
	if (musique == NULL)
	{
		cerr << "Can't read the music" << endl;
		exit(1);
	}

	/* On active la r�p�tition de la musique � l'infini */
	if (Mix_PlayMusic(musique, -1) == -1)
	{
		cout << "Mix_PlayMusic: " << Mix_GetError() << endl;
	}
}

//Fonctions
void Sound::loadSound()
{

	bumper_sound = Mix_LoadWAV("sounds/bumper.wav");
	if (bumper_sound == NULL)
	{
		fprintf(stderr, "Can't read the bumper sound FX \n");
		exit(1);
	}

	destroy_sound = Mix_LoadWAV("sounds/destroy.wav");
	if (destroy_sound == NULL)
	{
		fprintf(stderr, "Can't read the destroy sound FX \n");
		exit(1);
	}

	jump_sound = Mix_LoadWAV("sounds/jump.wav");
	if (jump_sound == NULL)
	{
		fprintf(stderr, "Can't read the jump sound FX \n");
		exit(1);
	}

	star_sound = Mix_LoadWAV("sounds/star.wav");
	if (star_sound == NULL)
	{
		fprintf(stderr, "Can't read the star sound FX \n");
		exit(1);
	}

}

void Sound::playSoundFx(int type)
{

	switch (type)
	{

	case BUMPER:
		Mix_PlayChannel(-1, bumper_sound, 0);
		break;

	case DESTROY:
		Mix_PlayChannel(-1, destroy_sound, 0);
		break;

	case JUMP:
		Mix_PlayChannel(-1, jump_sound, 0);
		break;

	case STAR:
		Mix_PlayChannel(-1, star_sound, 0);
		break;

	}


}

//Destructeur
Sound::~Sound()
{
	/* On lib�re la musique */
	if (musique != NULL)
		Mix_FreeMusic(musique);

	Mix_FreeChunk(bumper_sound);
	Mix_FreeChunk(destroy_sound);
	Mix_FreeChunk(jump_sound);
	Mix_FreeChunk(star_sound);
}
