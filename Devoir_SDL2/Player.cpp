#include "Player.h"

//Constructeur
Player::Player(Map *map, Plateforme *plateforme, int newLevel)
{

	//PV à 3
	this->player.setLife(3);

	//Timer d'invincibilité à 0
	this->player.setInvincibleTimer(0);

	//Nombre de plateformes flottantes à 0
	plateforme->resetPlateformes();

	//Indique l'état et la direction de notre héros
	this->player.setDirection(RIGHT);
	this->player.setEtat(IDLE);

	//Indique le numéro de la frame où commencer
	this->player.setFrameNumber(0);
	//...la valeur de son chrono ou timer
	this->player.setFrameTimer(TIME_BETWEEN_2_FRAMES_PLAYER);
	//... et son nombre de frames max (8 pour l'anim' IDLE
	// = ne fait rien)
	this->player.setFrameMax(8);

	/* Coordonnées de démarrage/respawn de notre héros */
	if (this->player.getCheckpointActif() == 1)
	{
		this->player.setX(this->player.getRespawnX());
		this->player.setY(this->player.getRespawnY());
	}
	else
	{
		player.setX(this->player.getBeginX());
		player.setY(this->player.getBeginY());
	}

	//On réinitiliase les coordonnées de la caméra 
	//si on change de niveau
	if (newLevel == 1)
	{
		map->setStartX(this->player.getBeginX());
		map->setStartY(this->player.getBeginY());
	}

	/* Hauteur et largeur de notre héros */
	player.setW(PLAYER_WIDTH);
	player.setH(PLAYER_HEIGTH);

	//Variables nécessaires au fonctionnement de la gestion des collisions
	player.setTimerMort(0);
	player.setOnGround(0);
}

//Accesseurs et mutateurs
int Player::getPlayerDirection() const { 
	return this->player.getDirection(); 
};

void Player::resetCheckpoint() {
	this->player.setCheckpointActif(0); 
};

int Player::getPlayerx() const {
	return this->player.getX();
};

int Player::getPlayery() const {
	return this->player.getY(); 
};

void Player::setPlayerx(int valeur) {
	this->player.setX(valeur);
};

void Player::setPlayery(int valeur) {
	this->player.setY(valeur); 
};

int Player::getLevel() {
	return this->level; 
};

void Player::SetValeurDuNiveau(int valeur) {
	this->level = valeur;
}

int Player::getLife() {
	return player.getLife(); 
};

int Player::getNombreDeVies() {
	return vies; 
};

void Player::setNombreDeVies(int valeur) {
	vies = valeur;
};

int Player::getNombreDetoiles() {
	return etoiles; 
};

void Player::setNombreDetoiles(int valeur) {
	etoiles = valeur;
};

//Fonctions
void Player::reinitPlayer(Map *map, Plateforme *plateforme, int newLevel)
{
	//3 pv
	this->player.setLife(3);

	//Timer d'invincibilité
	this->player.setInvincibleTimer(0);

	//Nombre de plateformes flottantes
	plateforme->resetPlateformes();

	//Indique l'état et la direction du personnage
	this->player.setDirection(RIGHT);
	this->player.setEtat(IDLE);

	//Indique le numéro de la frame où commencer
	this->player.setFrameNumber(0);
	//la valeur de son chrono ou timer
	this->player.setFrameTimer(TIME_BETWEEN_2_FRAMES_PLAYER);
	//son nombre de frames max
	this->player.setFrameMax(8);

	// Coordonnées de démarrage/respawn du personnage
	if (this->player.getCheckpointActif() == 1)
	{
		this->player.setX(this->player.getRespawnX());
		this->player.setY(this->player.getRespawnY());
	}
	else
	{
		this->player.setX(this->player.getBeginX());
		this->player.setY(this->player.getBeginY());
	}

	//Coordonnées caméra sur chgt de niveau
	if (newLevel == 1)
	{
		map->setStartX(this->player.getBeginX());
		map->setStartY(this->player.getBeginY());
	}

	/* Hauteur et largeur du personnage */
	this->player.setW(PLAYER_WIDTH);
	this->player.setH(PLAYER_HEIGTH);

	//gestion des collisions
	this->player.setTimerMort(0);
	this->player.setOnGround(0);
}

void Player::initPlayerSprites(SDL *SDL)
{
	this->playerSpriteSheet = SDL->loadImage("graphics/rabidja.png");
}

void Player::killPlayer(Sound *sound)
{
	//On met le timer à 1 pour tuer le joueur intantanément
	this->player.setTimerMort(1);
	//On joue le son
	sound->playSoundFx(DESTROY);
}

void Player::playerHurts(GO *monster, Sound *sound)
{
	//Si le timer d'invincibilité est à 0
	//on perd un coeur
	if (this->player.getInvincibleTimer() == 0)
	{
		this->player.setLife(this->player.getLife() - 1);
		this->player.setInvincibleTimer(80);
		//On joue le son
		sound->playSoundFx(DESTROY);
		monster->setTimerMort(1);
		this->player.setDirY(-JUMP_HEIGHT);
	}
}

void Player::updatePlayer(Input *input, Map *map, Sound *sound, Menu *menu, Plateforme *plateforme, SDL *SDL)
{
	if (this->player.getTimerMort() == 0)
	{
		//On gère le timer de l'invincibilité
		if (this->player.getInvincibleTimer()> 0)
			this->player.setInvincibleTimer(this->player.getInvincibleTimer() - 1);

		//On réinitialise notre vecteur de déplacement latéral (X), pour éviter que le perso
		//ne fonce de plus en plus vite pour atteindre la vitesse de la lumière ! ;)
		//Essayez de le désactiver pour voir !
		this->player.setDirX(0);

		// La gravité fait toujours tomber le perso : on incrémente donc le vecteur Y
		this->player.setDirY(this->player.getDirY() + GRAVITY_SPEED);

		//Mais on le limite pour ne pas que le joueur se mette à tomber trop vite quand même
		if (this->player.getDirY() >= MAX_FALL_SPEED)
		{
			this->player.setDirY(MAX_FALL_SPEED);
		}

		if (input->getLeft() == 1)
		{
			this->player.setDirX(this->player.getDirX() - PLAYER_SPEED);
			this->player.setDirection(LEFT);

			//Si ce n'était pas son état auparavant et qu'il est bien sur
			//le sol (car l'anim' sera différente s'il est en l'air)
			if (this->player.getEtat() != WALK && this->player.getOnGround() == 1)
			{
				//On enregistre l'anim' de la marche et on l'initialise à 0
				this->player.setEtat(WALK);
				this->player.setFrameNumber(0);
				this->player.setFrameTimer(TIME_BETWEEN_2_FRAMES_PLAYER);
				this->player.setFrameMax(8);
			}
		}

		//Si on détecte un appui sur la touche fléchée droite
		else if (input->getRight() == 1)
		{
			//On augmente les coordonnées en x du joueur
			this->player.setDirX(this->player.getDirX() + PLAYER_SPEED);
			//Et on indique qu'il va à droite (pour le flip
			//de l'affichage, rappelez-vous).
			this->player.setDirection(RIGHT);

			//Si ce n'était pas son état auparavant et qu'il est bien sur
			//le sol (car l'anim' sera différente s'il est en l'air)
			if (this->player.getEtat() != WALK && this->player.getOnGround() == 1)
			{
				//On enregistre l'anim' de la marche et on l'initialise à 0
				this->player.setEtat(WALK);
				this->player.setFrameNumber(0);
				this->player.setFrameTimer(TIME_BETWEEN_2_FRAMES_PLAYER);
				this->player.setFrameMax(8);
			}
		}
		else if (input->getRight() == 0 && input->getLeft() == 0 && this->player.getOnGround() == 1)
		{
			//On teste si le joueur n'était pas déjà inactif, pour ne pas recharger l'animation
			//à chaque tour de boucle
			if (this->player.getEtat() != IDLE)
			{
				//On enregistre l'anim' de l'inactivité et on l'initialise à 0
				this->player.setEtat(IDLE);
				this->player.setFrameNumber(0);
				this->player.setFrameTimer(TIME_BETWEEN_2_FRAMES_PLAYER);
				this->player.setFrameMax(8);
			}

		}

		if (input->getJump() == 1)
		{
			if (this->player.getOnGround() == 1)
			{
				this->player.setDirY(-JUMP_HEIGHT);
				this->player.setOnGround(0);
				this->player.setJump(1);
				sound->playSoundFx(JUMP);
			}
			/* Si on est en saut 1, on peut faire un deuxième bond et on remet jump1 à 0 */
			else if (this->player.getJump() == 1)
			{
				this->player.setDirY(-JUMP_HEIGHT);
				this->player.setJump(0);
				sound->playSoundFx(JUMP);
			}
			input->setJump(0);
		}

		//Si on appuie sur Enter
		if (input->getEnter() == 1)
		{
			//On met le jeu en pause
			menu->setOnMenu(1, PAUSE);
			input->setEnter(0);
		}



		/* Réactive la possibilité de double saut si on tombe sans sauter */
		if (this->player.getOnGround() == 1)
			this->player.setJump(1);


		//On gère l'anim du saut
		if (this->player.getOnGround() == 0)
		{
			//Si on est en saut 1, on met l'anim' du saut normal
			if (this->player.getJump() == 1)
			{
				if (this->player.getEtat() != JUMP1)
				{
					this->player.setEtat(JUMP1);
					this->player.setFrameNumber(0);
					this->player.setFrameTimer(TIME_BETWEEN_2_FRAMES_PLAYER);
					this->player.setFrameMax(2);
				}
			}
			else
			{
				if (this->player.getEtat() != JUMP2)
				{
					this->player.setEtat(JUMP2);
					this->player.setFrameNumber(0);
					this->player.setFrameTimer(TIME_BETWEEN_2_FRAMES_PLAYER);
					this->player.setFrameMax(4);
				}
			}
		}

		//On rajoute notre fonction de détection des collisions qui va mettre à 
		//jour les coordonnées de notre super lapin.
		map->mapCollision(&this->player, this, plateforme, sound, SDL, map);

		//On gère le scrolling (fonction ci-dessous)
		this->centerScrollingOnPlayer(map);

	}

	if (this->player.getTimerMort() > 0)
	{
		this->player.setTimerMort(this->player.getTimerMort() - 1);

		if (this->player.getTimerMort() == 0)
		{
			// Si on est mort, on perd une vie 
			this->setNombreDeVies(this->getNombreDeVies() - 1);

			//Sauf si on a plus de vies...
			if (this->getNombreDeVies() < 0)
			{
				//Dans ce cas on retourne au menu start
				menu->setOnMenu(1, START);
			}

			//Sinon on réinitialise le niveau
			map->changeLevel(SDL, this);
			this->reinitPlayer(map, plateforme, 0);
		}
	}
}

void Player::drawPlayer(Map *map, SDL *SDL)
{
	/* Gestion du timer */
	
	// Si notre timer (un compte à rebours en fait) arrive à zéro
	if (this->player.getFrameTimer() <= 0)
	{
		//On le réinitialise
		this->player.setFrameTimer(TIME_BETWEEN_2_FRAMES_PLAYER);

		//Et on incrémente notre variable qui compte les frames de 1 pour passer à la suivante
		this->player.setFrameNumber(this->player.getFrameNumber() + 1);

		//Mais si on dépasse la frame max, il faut revenir à la première :
		if (this->player.getFrameNumber() >= this->player.getFrameMax())
			this->player.setFrameNumber(0);
	}
	//Sinon, on décrémente notre timer
	else
		this->player.setFrameTimer(this->player.getFrameTimer() - 1);


	//Ensuite, on peut passer la main à notre fonction

	/* Rectangle de destination à dessiner */
	SDL_Rect dest;

	// On soustrait des coordonnées de notre héros, ceux du début de la map, pour qu'il colle
	//au scrolling :
	dest.x = this->player.getX() - map->getStartX();
	dest.y = this->player.getY() - map->getStartY();
	dest.w = this->player.getW();
	dest.h = this->player.getH();

	/* Rectangle source */
	SDL_Rect src;

	//Pour connaître le X de la bonne frame à dessiner, il suffit de multiplier
	//la largeur du sprite par le numéro de la frame à afficher -> 0 = 0; 1 = 40; 2 = 80...
	src.x = this->player.getFrameNumber() * this->player.getW();
	src.w = this->player.getW();
	src.h = this->player.getH();

	//On calcule le Y de la bonne frame à dessiner, selon la valeur de l'état du héros :
	//Aucun Mouvement (Idle) = 0, marche (walk) = 1, etc...
	//Tout cela en accord avec notre spritesheet, of course ;)
	src.y = this->player.getEtat() * this->player.getH();

	//Si on a été touché et qu'on est invincible
	if (this->player.getInvincibleTimer() > 0)
	{
		//On fait clignoter le héros une frame sur deux
		//Pour ça, on calcule si le numéro de la frame en 
		//cours est un multiple de deux
		if (this->player.getFrameNumber() % 2 == 0)
		{
			//Gestion du flip (retournement de l'image selon que le sprite regarde à droite ou à gauche
			if (this->player.getDirection() == LEFT)
				SDL_RenderCopyEx(SDL->getrenderer(), this->playerSpriteSheet, &src, &dest, 0, 0, SDL_FLIP_HORIZONTAL);
			else
				SDL_RenderCopyEx(SDL->getrenderer(), this->playerSpriteSheet, &src, &dest, 0, 0, SDL_FLIP_NONE);
		}
		//Sinon, on ne dessine rien, pour le faire clignoter
	}

	//Sinon, on dessine normalement
	else
	{
		//Gestion du flip (retournement de l'image selon que le sprite regarde à droite ou à gauche
		if (this->player.getDirection() == LEFT)
			SDL_RenderCopyEx(SDL->getrenderer(), this->playerSpriteSheet, &src, &dest, 0, 0, SDL_FLIP_HORIZONTAL);
		else
			SDL_RenderCopyEx(SDL->getrenderer(), this->playerSpriteSheet, &src, &dest, 0, 0, SDL_FLIP_NONE);
	}

}

void Player::centerScrollingOnPlayer(Map *map)
{

	int cxperso = this->player.getX() + this->player.getW() / 2;
	int cyperso = this->player.getY() + this->player.getH() / 2;
	int xlimmin = map->getStartX() + LIMITE_X;
	int xlimmax = xlimmin + LIMITE_W;
	int ylimmin = map->getStartY() + LIMITE_Y;
	int ylimmax = ylimmin + LIMITE_H;

	//Effet de retour en arrière quand on est mort :
	//Si on est très loin de la caméra, plus loin que le bord
	//de la map, on accélère le scrolling :
	if (cxperso < map->getStartX())
	{
		map->setStartX(map->getStartX() - 30);
	}

	//Si on dépasse par la gauche, on recule la caméra
	else if (cxperso < xlimmin)
	{
		map->setStartX(map->getStartX() - 4);
	}

	if (cxperso > map->getStartX() + SCREEN_WIDTH)
	{
		map->setStartX(map->getStartX() + 30);
	}

	//Si on dépasse par la droite, on avance la caméra
	else if (cxperso > xlimmax)
	{
		map->setStartX(map->getStartX() + 4);
	}

	//Si on arrive au bout de la map à gauche, on stoppe le scrolling
	if (map->getStartX() < 0)
	{
		map->setStartX(0);
	}

	//Si on arrive au bout de la map à droite, on stoppe le scrolling à la 
	//valeur Max de la map - la moitié d'un écran (pour ne pas afficher du noir).
	else if (map->getStartX() + SCREEN_WIDTH >= map->getMaxX())
	{
		map->setStartX(map->getMaxX() - SCREEN_WIDTH);
	}

	//Si on dépasse par le haut, on remonte la caméra 
	if (cyperso < ylimmin)
	{
		map->setStartY(map->getStartY() - 4);
	}

	//Si on dépasse par le bas, on descend la caméra
	if (cyperso > ylimmax)
	{
		//Sauf si on tombe très vite, auquel cas, on accélère la caméra :
		if (this->player.getDirY() >= MAX_FALL_SPEED - 2)
		{
			map->setStartY(map->getStartY() + MAX_FALL_SPEED + 1);
		}
		else
		{
			map->setStartY(map->getStartY() + 4);
		}
	}

	//Si on arrive au bout de la map en haut, on stoppe le scrolling
	if (map->getStartY() < 0)
	{
		map->setStartY(0);
	}

	//Si on arrive au bout de la map en bas, on stoppe le scrolling à la 
	//valeur Max de la map - la moitié d'un écran (pour ne pas afficher du noir ;) ).
	else if (map->getStartY() + SCREEN_HEIGHT >= map->getMaxY())
	{
		map->setStartY(map->getMaxY() - SCREEN_HEIGHT);
	}

}

void Player::getItem(int itemNumber, Sound *sound)
{
	switch (itemNumber)
	{
		//Gestion des étoiles
	case 1:
		//On incrémente le compteur Etoile
		this->setNombreDetoiles(this->getNombreDetoiles() + 1);
		sound->playSoundFx(STAR);

		//On teste s'il y a 100 étoiles : on remet le compteur à 0 et on rajoute une vie ;)
		if (this->getNombreDetoiles() >= 100)
		{
			this->setNombreDetoiles(0);
			//On incrémente le nombre de vies (max 99)
			if (this->getNombreDeVies() < 99)
				this->setNombreDeVies(this->getNombreDeVies() + 1);
		}
		break;

		//Gestion des coeurs
	case 2:
		//On incrémente le compteur Etoile
		if (this->player.getLife() < 3)
			this->player.setLife(this->player.getLife() + 1);

		sound->playSoundFx(STAR);
		break;

		//Gestion des vies
	case 3:
		//On incrémente le nombre de vies (max 99)
		if (this->getNombreDeVies() < 99)
			this->setNombreDeVies(this->getNombreDeVies() + 1);

		sound->playSoundFx(STAR);
		break;

	default:
		break;
	}


}

//Destructeur
Player::~Player()
{
	if (this->playerSpriteSheet != NULL)
	{
		SDL_DestroyTexture(this->playerSpriteSheet);
		this->playerSpriteSheet = NULL;
	}
}
