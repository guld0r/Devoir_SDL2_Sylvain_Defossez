#include "Plateforme.h"


//Constructeur
Plateforme::Plateforme(SDL *SDL)
{
	//On charge l'image de la plateforme
	plateformeTexture = SDL->loadImage("graphics/plateforme.png");

	/* Si on ne peut pas, on quitte, et on l'indique en erreur ;) */
	if (plateformeTexture == NULL)
	{
		cout << "Impossible de charger l'image de la plateforme : graphics/plateforme.png/n" << endl;
		exit(1);
	}

	this->nombrePlateformes = 0;
}

//Accesseurs et mutateurs
int Plateforme::getPlateformeNumber() { 
	return this->nombrePlateformes; 
};

void Plateforme::resetPlateformes() { 
	this->nombrePlateformes = 0; 
};

//Fonctions
void Plateforme::initPlateforme(int x, int y, int type)
{
	/* On rajoute une plateforme à la liste des plateformes */
	this->nombrePlateformes++;

	//S'il y en a trop, on fait une erreur et on quitte
	if (this->nombrePlateformes > PLATEFORMES_MAX)
	{
		cout << "Trop de plateformes initialisees !\nEnlevez-en ou changez leur nombre max." << endl;
		exit(1);
	}

	//On remplit notre structure
	this->plateforme[this->nombrePlateformes].setX(x);
	this->plateforme[this->nombrePlateformes].setBeginX(x);
	this->plateforme[this->nombrePlateformes].setY(y);
	this->plateforme[this->nombrePlateformes].setBeginY(y);

	//On enregistre la taille de l'image
	int w, h;
	SDL_QueryTexture(this->plateformeTexture, NULL, NULL, &w, &h);

	this->plateforme[this->nombrePlateformes].setW(w);
	this->plateforme[this->nombrePlateformes].setH(h);

	this->plateforme[this->nombrePlateformes].setType(type);

	//Si elle est du type 2, elle monte, sinon elle va à droite
	if (this->plateforme[this->nombrePlateformes].getType() == 2)
		this->plateforme[this->nombrePlateformes].setDirection(UP);
	else
		this->plateforme[this->nombrePlateformes].setDirection(RIGHT);
}

void Plateforme::doPlateforme(Player *player)
{
	int i;

	for (i = 1; i <= this->nombrePlateformes; i++)
	{
		/* Déplacement UP/DOWN (haut/bas) */
		if (plateforme[i].getType() == 2)
		{
			if (plateforme[i].getDirection() == UP)
			{
				plateforme[i].setY(plateforme[i].getY() - PLATEFORM_SPEED);
				/* Si le joueur est dessus, on le déplace avec
				pour éviter qu'il ne reste immobile et que la
				plateforme se barre comme sur certains vieux
				(mauvais) jeux...*/
				if (plateforme[i].getPlayer_Dessus() == 1)
					player->setPlayery(player->getPlayery() - PLATEFORM_SPEED);
			}
			else
			{
				plateforme[i].setY(plateforme[i].getY() + PLATEFORM_SPEED);
				/* Si le joueur est dessus, on le déplace avec */
				if (plateforme[i].getPlayer_Dessus() == 1)
					player->setPlayery(player->getPlayery() + PLATEFORM_SPEED);
			}

			if (plateforme[i].getY() > plateforme[i].getBeginY() + 5 * TILE_SIZE)
				plateforme[i].setDirection(UP);


			if (plateforme[i].getY() < plateforme[i].getBeginY())
				plateforme[i].setDirection(DOWN);
		}

		/* Déplacement RIGHT/LEFT */
		else
		{
			if (plateforme[i].getDirection() == RIGHT)
			{
				plateforme[i].setX(plateforme[i].getX() + PLATEFORM_SPEED);
				/* Si le joueur est dessus, on le déplace avec */
				if (plateforme[i].getPlayer_Dessus() == 1)
					player->setPlayerx(player->getPlayerx() + PLATEFORM_SPEED);
			}
			else
			{
				plateforme[i].setX(plateforme[i].getX() - PLATEFORM_SPEED);
				/* Si le joueur est dessus, on le déplace avec */
				if (plateforme[i].getPlayer_Dessus() == 1)
					player->setPlayerx(player->getPlayerx() - PLATEFORM_SPEED);
			}

			//Test : si la plateforme dépasse 5 tiles de longueur,
			//on lui fait faire demi-tour pour ne pas qu'elle
			//fasse toute la map ! :D
			if (plateforme[i].getX() > plateforme[i].getBeginX() + 5 * TILE_SIZE)
				plateforme[i].setDirection(LEFT);

			if (plateforme[i].getX() < plateforme[i].getBeginX())
				plateforme[i].setDirection(RIGHT);
		}

	}
}

void Plateforme::drawPlateforme(SDL *SDL, Map *map)
{
	int i;


	for (i = 1; i <= nombrePlateformes; i++)
	{
		SDL->drawImage(plateformeTexture, plateforme[i].getX() - map->getStartX(), plateforme[i].getY() - map->getStartY());
	}
}

void Plateforme::checkCollisionsWithPlateforms(GO *entity)
{
	int j;

	for (j = 1; j <= this->nombrePlateformes; j++)
	{
		if (entity->getX() + entity->getW() >= plateforme[j].getX()
			&& entity->getX() <= plateforme[j].getX() + plateforme[j].getW()
			&& entity->getY() + entity->getH() >= plateforme[j].getY()
			&& entity->getY() + entity->getH() < plateforme[j].getY() + 32)
		{
			// Place the player as close to the plateform as possible 
			entity->setY(plateforme[j].getY() - entity->getH());
			entity->setDirY(0);
			entity->setOnGround(1);

			plateforme[j].setPlayer_Dessus(1);
		}
		else
			plateforme[j].setPlayer_Dessus(0);
	}
}

//Destructeur
Plateforme::~Plateforme()
{
	if (plateformeTexture != NULL)
	{
		SDL_DestroyTexture(plateformeTexture);
	}
}
