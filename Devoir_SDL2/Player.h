#pragma once
#include "resources.h"
#include "GO.h"
#include "Map.h"
#include "Plateforme.h"

class Input;
class Map;
class Sound;
class Menu;
class Plateforme;
class GO;
class SDL;
class Player
{
private:
	int vies, etoiles, level;
	GO player;
	SDL_Texture *playerSpriteSheet;

public:
	//Constructeur
	Player(Map*, Plateforme*, int);

	//Accesseurs et mutateurs
	GO *getPlayer() { return &this->player; };
	int getPlayerDirection() const;
	void resetCheckpoint();
	int getPlayerx() const;
	int getPlayery() const;
	void setPlayerx(int valeur);
	void setPlayery(int valeur);
	int getLevel();
	void SetValeurDuNiveau(int valeur);
	int getLife();
	int getNombreDeVies();
	void setNombreDeVies(int valeur);
	int getNombreDetoiles();
	void setNombreDetoiles(int valeur);

	//Fonctions
	void reinitPlayer(Map*, Plateforme*, int);
	void initPlayerSprites(SDL *);
	void killPlayer(Sound*);
	void playerHurts(GO *, Sound*);
	void updatePlayer(Input *, Map*, Sound*, Menu*, Plateforme*, SDL*);
	void drawPlayer(Map*, SDL*);
	void centerScrollingOnPlayer(Map*);
	void getItem(int, Sound*);

	//Destructeur
	~Player();
};
