#pragma once
#include "Resources.h"
#include "Plateforme.h"
#include "Player.h"
#include "Sound.h"
#include "SDL.h"
#include "GO.h"

class SDL;
class Player;
class GO;
class Plateforme;
class Sound;

class Map
{
private:
	SDL_Texture *background;
	SDL_Texture *tileSet, *tileSetB;

	//Numéro du tileset à utiliser
	int tilesetAffiche;

	/* Coordonnées de départ du héros, lorsqu'il commence le niveau */
	int beginx, beginy;

	/* Coordonnées de début, lorsqu'on doit dessiner la map */
	int startX, startY;

	/* Coordonnées max de fin de la map */
	int maxX, maxY;

	/* Tableau à double dimension représentant la map de tiles */
	int tile[MAX_MAP_Y][MAX_MAP_X];

	//Deuxième couche de tiles
	int tile2[MAX_MAP_Y][MAX_MAP_X];

	//Troisième couche de tiles
	int tile3[MAX_MAP_Y][MAX_MAP_X];

	/* Timer et numéro du tileset à afficher pour animer la map */
	int mapTimer, tileSetNumber;

public:
	//Constructeur
	Map(SDL*);

	//Accesseurs et mutateurs
	int getStartX();
	void setStartX(int valeur);
	int getStartY();
	void setStartY(int valeur);
	int getMaxX();
	int getMaxY();
	int getBeginX();
	int getBeginY();
	SDL_Texture *getBackground() { return this->background; };
	int getTileValue(int y, int x);
	SDL_Texture *getTileSetA() { return this->tileSet; };
	SDL_Texture *getTileSetB() { return this->tileSetB; };

	//Fonctions
	void loadMap(char*);
	void drawMap(int, Plateforme*, SDL*);
	void changeLevel(SDL*, Player*);
	void mapCollision(GO*, Player*, Plateforme*, Sound*, SDL*, Map*);

	//Destructeur
	~Map();
};

