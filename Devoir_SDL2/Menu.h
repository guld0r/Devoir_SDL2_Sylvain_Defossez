#pragma once
#include "resources.h"
#include "SDL.h"
#include "Player.h"
#include "Map.h"
#include "Input.h"

class Input;
class Player;
class Map;
class Plateforme;
class SDL;

class Menu
{
private:
	int onMenu, menuType, choice;
	SDL_Texture *titlescreen;
	SDL_Texture *HUD_vie, *HUD_etoiles;

public:
	//Constructeur
	Menu(SDL*);

	//Accesseurs
	int getOnMenu();
	int getMenuType();

	//Mutateur
	void setOnMenu(int, int);

	//Fonctions
	void updateStartMenu(Input*, Player*, Map*, Plateforme*, SDL*);
	void updatePauseMenu(Input *input);
	void drawStartMenu(SDL*, Player*);
	void drawPauseMenu(SDL*, Player*);
	void drawHUD(Player*, Map*, SDL*);

	//Destructeur
	~Menu();
};

