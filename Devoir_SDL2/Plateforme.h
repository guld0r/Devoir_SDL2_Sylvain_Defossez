#pragma once
#include "resources.h"
#include "SDL.h"
#include "GO.h"
#include "Player.h"
#include "Map.h"

class GO;
class Player;
class SDL;
class Menu;
class Map;
class Plateforme
{
private:
	GO plateforme[50];
	SDL_Texture *plateformeTexture;
	int nombrePlateformes;


public:
	//Constructeur
	Plateforme(SDL*);

	//Accesseurs et mutateurs
	GO *getPlateforme();
	int getPlateformeNumber();

	//Fonctions
	void initPlateforme(int, int, int);
	void resetPlateformes();
	void doPlateforme(Player *);
	void drawPlateforme(SDL*, Map*);
	void checkCollisionsWithPlateforms(GO *);

	//Destructeur
	~Plateforme();
};

