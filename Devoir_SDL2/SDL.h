#pragma once
#include "Resources.h"
#include "Map.h"
#include "Player.h"
#include "Menu.h"
#include "Plateforme.h"
#include "Sound.h"
#include "GO.h"
#include "Input.h"

class Map;
class Player;
class Plateforme;
class Menu;
class GO;
class Sound;
class Input;
class SDL
{
private:
	SDL_Window *screen;
	SDL_Renderer *renderer;
	SDL_Surface *plateformeSurface;
	int nombrePlateformes;
	TTF_Font *font;

public:
	//Constructeur
	SDL(char*, Input*);

	//Accesseurs et mutateurs
	SDL_Renderer *getrenderer() { return this->renderer; };
	void setrenderer(SDL_Renderer *);
	int getPlateformeNumber();

	SDL_Texture *loadImage(char *);
	void delay(unsigned int);
	void drawImage(SDL_Texture *, int, int);
	void resetPlateformes() { this->nombrePlateformes = 0; };
	void drawTile(SDL_Texture *, int, int, int, int);
	void loadFont(char *, int);
	void drawString(char *, int, int, int, int, int, int);
	void drawGame(int, Map*, Player*, Plateforme*, Menu*, SDL*);
	void loadGame(SDL*, Player*, Map*, Menu*, Sound*, Plateforme*);

	//Destructeur
	~SDL();
};

