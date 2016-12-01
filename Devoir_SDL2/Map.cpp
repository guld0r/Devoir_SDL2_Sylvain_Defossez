#include "Map.h"


//Constructeur
Map::Map(SDL *SDL)
{
	// Charge l'image du fond (background)  
	this->background = SDL->loadImage("graphics/background.png");

	//On initialise le timer
	this->mapTimer = TIME_BETWEEN_2_FRAMES * 3;
	this->tileSetNumber = 0;
}

//Accesseurs et mutateurs
int Map::getStartX() {
	return this->startX; 
};

void Map::setStartX(int valeur) {
	this->startX = valeur; 
};

int Map::getStartY() {
	return this->startY;
};

void Map::setStartY(int valeur) {
	this->startY = valeur; 
};

int Map::getMaxX() {
	return this->maxX; 
};

int Map::getMaxY() {
	return this->maxY;
};

int Map::getBeginX() {
	return this->beginx;
};

int Map::getBeginY() {
	return this->beginy; 
};

int Map::getTileValue(int y, int x) {
	return this->tile[y][x];
};

//Fonctions
void Map::loadMap(char *name)
{
	int x, y;
	FILE *fp;
	errno_t err;

	//Ouvre le fichier en lecture (renvoie une erreur s'il n'existe pas)
	if ((err = fopen_s(&fp, name, "rb")) != 0)
	{
		cerr << "Le fichier map n'a pas pu etre ouvert." << endl;
		exit(1);
	}

	fscanf_s(fp, "%d", &this->beginx);
	fscanf_s(fp, "%d", &this->beginy);

	/* lit la val de tileset */

	fscanf_s(fp, "%d", &this->tilesetAffiche);

	this->maxX = this->maxY = 0;

	for (y = 0; y < MAX_MAP_Y; y++)
	{
		for (x = 0; x < MAX_MAP_X; x++)
		{
			/* On lit le numéro de la tile et on le copie dans notre tableau */

			fscanf_s(fp, "%d", &this->tile[y][x]);

			/* Permet de déterminer la taille de la map (voir plus bas) */
			if (this->tile[y][x] > 0)
			{
				if (x > this->maxX)
				{
					this->maxX = x;
				}

				if (y > this->maxY)
				{
					this->maxY = y;
				}
			}
		}
	}

	//Deuxième couche de tiles
	for (y = 0; y < MAX_MAP_Y; y++)
	{
		for (x = 0; x < MAX_MAP_X; x++)
		{
			/* On lit le numéro de la tile et on le copie dans notre tableau */

			fscanf_s(fp, "%d", &this->tile2[y][x]);
		}
	}

	//Troisième couche de tiles
	for (y = 0; y < MAX_MAP_Y; y++)
	{
		for (x = 0; x < MAX_MAP_X; x++)
		{
			/* On lit le numéro de la tile et on le copie dans notre tableau */

			fscanf_s(fp, "%d", &this->tile3[y][x]);
		}
	}

	/* maxX et maxY sont les coordonnées de fin de la this->
	On les trouve dès qu'il n'y a plus que des zéros à la suite.
	Comme ça, on peut faire des maps de tailles différentes avec la même
	structure de fichier. */
	this->maxX = (this->maxX + 1) * TILE_SIZE;
	this->maxY = (this->maxY + 1) * TILE_SIZE;

	/* Et on referme le fichier */
	fclose(fp);

}

void Map::drawMap(int layer, Plateforme *plateforme, SDL *SDL)
{
	int x, y, mapX, x1, x2, mapY, y1, y2, xsource, ysource, a;

	/* On initialise mapX à la 1ère colonne qu'on doit blitter.
	Celle-ci correspond au x de la map (en pixels) divisés par la taille d'une tile (32)
	pour obtenir la bonne colonne de notre map
	Exemple : si x du début de la map = 1026, on fait 1026 / 32
	et on sait qu'on doit commencer par afficher la 32eme colonne de tiles de notre map */
	mapX = this->getStartX() / TILE_SIZE;

	/* Coordonnées de départ pour l'affichage de la map : permet
	de déterminer à quels coordonnées blitter la 1ère colonne de tiles au pixel près
	(par exemple, si la 1ère colonne n'est visible qu'en partie, on devra commencer à blitter
	hors écran, donc avoir des coordonnées négatives - d'où le -1). */
	x1 = (this->getStartX() % TILE_SIZE) * -1;

	/* Calcul des coordonnées de la fin de la map : jusqu'où doit-on blitter ?
	Logiquement, on doit aller à x1 (départ) + SCREEN_WIDTH (la largeur de l'écran).
	Mais si on a commencé à blitter en dehors de l'écran la première colonne, il
	va falloir rajouter une autre colonne de tiles sinon on va avoir des pixels
	blancs. C'est ce que fait : x1 == 0 ? 0 : TILE_SIZE qu'on pourrait traduire par:
	if(x1 != 0)
	x2 = x1 + SCREEN_WIDTH + TILE_SIZE , mais forcément, c'est plus long ;)*/
	x2 = x1 + SCREEN_WIDTH + (x1 == 0 ? 0 : TILE_SIZE);

	/* On fait exactement pareil pour calculer y */
	mapY = this->getStartY() / TILE_SIZE;
	y1 = (this->getStartY() % TILE_SIZE) * -1;
	y2 = y1 + SCREEN_HEIGHT + (y1 == 0 ? 0 : TILE_SIZE);


	//On met en place un timer pour animer la map 
	if (this->mapTimer <= 0)
	{
		if (this->tileSetNumber == 0)
		{
			this->tileSetNumber = 1;
			this->mapTimer = TIME_BETWEEN_2_FRAMES * 3;
		}
		else
		{
			this->tileSetNumber = 0;
			this->mapTimer = TIME_BETWEEN_2_FRAMES * 3;
		}
	}
	else
		this->mapTimer--;


	/* Dessine la carte en commençant par startX et startY */

	/* On dessine ligne par ligne en commençant par y1 (0) jusqu'à y2 (480)
	A chaque fois, on rajoute TILE_SIZE (donc 32), car on descend d'une ligne
	de tile (qui fait 32 pixels de hauteur) */
	if (layer == 1)
	{
		for (y = y1; y < y2; y += TILE_SIZE)
		{
			/* A chaque début de ligne, on réinitialise mapX qui contient la colonne
			(0 au début puisqu'on ne scrolle pas) */
			mapX = this->getStartX() / TILE_SIZE;

			/* A chaque colonne de tile, on dessine la bonne tile en allant
			de x = 0 à x = 640 */
			for (x = x1; x < x2; x += TILE_SIZE)
			{

				//Si la tile à dessiner n'est pas une tile vide
				if (this->tile[mapY][mapX] != 0)
				{

					/*On teste si c'est une tile plateforme flottante */
					if (this->tile[mapY][mapX] >= TILE_PLATEFORME_DEBUT
						&& this->tile[mapY][mapX] <= TILE_PLATEFORME_FIN)
					{
						//On initialise une plateforme flottante
						//Pour obtenir le type (dernier arg), on enlève le numéro de la 1ère tile plateforme
						//(TILE_PLATEFORME_DEBUT) au numéro de la til en cours (this->tile[mapY][mapX])
						//et on rajoute 1 pour que le premier type soit le 1 et pas le 0 ;) 
						plateforme->initPlateforme(mapX * TILE_SIZE, mapY * TILE_SIZE, this->tile[mapY][mapX] - TILE_PLATEFORME_DEBUT + 1);

						//Et on efface cette tile de notre tableau pour éviter un spawn de plateformes
						//infini !
						this->tile[mapY][mapX] = 0;
					}
				}

				/* Suivant le numéro de notre tile, on découpe le tileset (a = le numéro
				de la tile */
				a = this->tile[mapY][mapX];

				/* Calcul pour obtenir son y (pour un tileset de 10 tiles
				par ligne, d'où le 10 */
				ysource = a / 10 * TILE_SIZE;
				/* Et son x */
				xsource = a % 10 * TILE_SIZE;

				/* Fonction qui blitte la bonne tile au bon endroit suivant le timer */
				if (this->tileSetNumber == 0)
					SDL->drawTile(this->tileSet, x, y, xsource, ysource);
				else
					SDL->drawTile(this->tileSetB, x, y, xsource, ysource);

				mapX++;
			}

			mapY++;
		}
	}

	else if (layer == 2)
	{
		//Deuxième couche de tiles ;)
		for (y = y1; y < y2; y += TILE_SIZE)
		{
			mapX = this->getStartX() / TILE_SIZE;

			for (x = x1; x < x2; x += TILE_SIZE)
			{
				//Si la tile à dessiner n'est pas une tile vide
				if (this->tile2[mapY][mapX] != 0)
				{


					/*On teste si c'est une tile plateforme flottante */
					if (this->tile2[mapY][mapX] >= TILE_PLATEFORME_DEBUT
						&& this->tile2[mapY][mapX] <= TILE_PLATEFORME_FIN)
					{
						//On initialise une plateforme flottante
						//Pour obtenir le type (dernier arg), on enlève le numéro de la 1ère tile plateforme
						//(TILE_PLATEFORME_DEBUT) au numéro de la til en cours (this->tile[mapY][mapX])
						//et on rajoute 1 pour que le premier type soit le 1 et pas le 0 ;)
						plateforme->initPlateforme(mapX * TILE_SIZE, mapY * TILE_SIZE, this->tile2[mapY][mapX] - TILE_PLATEFORME_DEBUT + 1);
						//Et on efface cette tile de notre tableau pour éviter un spawn de plateformes
						//infini !
						this->tile2[mapY][mapX] = 0;
					}
				}

				/* Suivant le numéro de notre tile, on découpe le tileset (a = le numéro
				de la tile */
				a = this->tile2[mapY][mapX];

				/* Calcul pour obtenir son y (pour un tileset de 10 tiles
				par ligne, d'où le 10 */
				ysource = a / 10 * TILE_SIZE;
				/* Et son x */
				xsource = a % 10 * TILE_SIZE;

				/* Fonction qui blitte la bonne tile au bon endroit suivant le timer */
				if (this->tileSetNumber == 0)
					SDL->drawTile(this->tileSet, x, y, xsource, ysource);
				else
					SDL->drawTile(this->tileSetB, x, y, xsource, ysource);

				mapX++;
			}
			mapY++;
		}
	}

	else if (layer == 3)
	{
		//Troisième couche de tiles ;)
		for (y = y1; y < y2; y += TILE_SIZE)
		{
			mapX = this->getStartX() / TILE_SIZE;

			for (x = x1; x < x2; x += TILE_SIZE)
			{
				//Si la tile à dessiner n'est pas une tile vide
				if (this->tile3[mapY][mapX] != 0)
				{


					/*On teste si c'est une tile plateforme flottante */
					if (this->tile3[mapY][mapX] >= TILE_PLATEFORME_DEBUT
						&& this->tile3[mapY][mapX] <= TILE_PLATEFORME_FIN)
					{
						//On initialise une plateforme flottante
						//Pour obtenir le type (dernier arg), on enlève le numéro de la 1ère tile plateforme
						//(TILE_PLATEFORME_DEBUT) au numéro de la til en cours (this->tile[mapY][mapX])
						//et on rajoute 1 pour que le premier type soit le 1 et pas le 0 ;)
						plateforme->initPlateforme(mapX * TILE_SIZE, mapY * TILE_SIZE, this->tile3[mapY][mapX] - TILE_PLATEFORME_DEBUT + 1);
						//Et on efface cette tile de notre tableau pour éviter un spawn de plateformes
						//infini !
						this->tile3[mapY][mapX] = 0;
					}
				}

				/* Suivant le numéro de notre tile, on découpe le tileset (a = le numéro
				de la tile */
				a = this->tile3[mapY][mapX];

				/* Calcul pour obtenir son y (pour un tileset de 10 tiles
				par ligne, d'où le 10 */
				ysource = a / 10 * TILE_SIZE;
				/* Et son x */
				xsource = a % 10 * TILE_SIZE;

				/* Fonction qui blitte la bonne tile au bon endroit suivant le timer */
				if (this->tileSetNumber == 0)
					SDL->drawTile(this->tileSet, x, y, xsource, ysource);
				else
					SDL->drawTile(this->tileSetB, x, y, xsource, ysource);

				mapX++;
			}
			mapY++;
		}
	}


}

void Map::changeLevel(SDL *SDL, Player *player)
{
	char file[200];

	/* Charge la map depuis le fichier */


	sprintf_s(file, sizeof(file), "map/map%d.txt", player->getLevel());

	loadMap(file);

	//Charge le tileset
	if (this->tileSet != NULL)
	{
		SDL_DestroyTexture(this->tileSet);
	}
	if (this->tileSetB != NULL)
	{
		SDL_DestroyTexture(this->tileSetB);
	}

	sprintf_s(file, sizeof(file), "graphics/tileset%d.png", this->tilesetAffiche);
	this->tileSet = SDL->loadImage(file);

	sprintf_s(file, sizeof(file), "graphics/tileset%dB.png", this->tilesetAffiche);
	this->tileSetB = SDL->loadImage(file);

}

void Map::mapCollision(GO *entity, Player *player, Plateforme *plateforme, Sound *sound, SDL *SDL, Map *map)
{
	int i, x1, x2, y1, y2;

	/* D'abord, on considère le joueur en l'air jusqu'à temps
	d'être sûr qu'il touche le sol */
	entity->setOnGround(0);

	/* Ensuite, on va tester les mouvements horizontaux en premier
	(axe des X). On va se servir de i comme compteur pour notre boucle.
	En fait, on va découper notre sprite en blocs de tiles pour voir
	quelles tiles il est susceptible de recouvrir.
	On va donc commencer en donnant la valeur de Tile_Size à i pour qu'il
	teste la tile où se trouve le x du joueur mais aussi la suivante SAUF
	dans le cas où notre sprite serait inférieur à 	la taille d'une tile.
	Dans ce cas, on lui donnera la vraie valeur de la taille du sprite
	Et on testera ensuite 2 fois la même tile. Mais comme ça notre code
	sera opérationnel quelle que soit la taille de nos sprites ! */

	if (entity->getH() > TILE_SIZE)
		i = TILE_SIZE;
	else
		i = entity->getH();


	//On lance alors une boucle for infinie car on l'interrompra selon 
	//les résultats de nos calculs
	for (;;)
	{
		//On va calculer ici les coins de notre sprite à gauche et à 
		//droite pour voir quelle tile ils touchent.
		x1 = (entity->getX() + entity->getDirX()) / TILE_SIZE;
		x2 = (entity->getX() + entity->getDirX() + entity->getW() - 1) / TILE_SIZE;

		//Même chose avec y, sauf qu'on va monter au fur et à mesure 
		//pour tester toute la hauteur de notre sprite, grâce à notre 
		//fameuse variable i.
		y1 = (entity->getY()) / TILE_SIZE;
		y2 = (entity->getY() + i - 1) / TILE_SIZE;

		//De là, on va tester les mouvements initiés dans updatePlayer 
		//grâce aux vecteurs dirX et dirY, tout en testant avant qu'on 
		//se situe bien dans les limites de l'écran.
		if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
		{
			//Si on a un mouvement à droite
			if (entity->getDirX() > 0)
			{

				//Test des tiles Power-up
				if (this->tile[y1][x2] >= TILE_POWER_UP_DEBUT
					&& this->tile[y1][x2] <= TILE_POWER_UP_FIN)
				{
					//On appelle la fonction getItem()
					player->getItem(this->tile[y1][x2] - TILE_POWER_UP_DEBUT + 1, sound);

					//On remplace la tile power-up par une tile transparente
					this->tile[y1][x2] = 0;
				}
				else if (this->tile[y2][x2] >= TILE_POWER_UP_DEBUT
					&& this->tile[y2][x2] <= TILE_POWER_UP_FIN)
				{
					//On appelle la fonction getItem()
					player->getItem(this->tile[y2][x2] - TILE_POWER_UP_DEBUT + 1, sound);

					//On remplace la tile power-up par une tile transparente
					this->tile[y2][x2] = 0;
				}


				//Test de la tile checkpoint
				if (this->tile[y1][x2] == TILE_CHECKPOINT)
				{
					//On active le booléen checkpoint
					entity->setCheckpointActif(1);

					//On enregistre les coordonnées
					entity->setRespawnX(x2 * TILE_SIZE);
					entity->setRespawnY((y1 * TILE_SIZE) - entity->getH());

					//On change la tile
					this->tile[y1][x2] += 1;
				}
				else if (this->tile[y2][x2] == TILE_CHECKPOINT)
				{
					//On active le booléen checkpoint
					entity->setCheckpointActif(1);

					//On enregistre les coordonnées
					entity->setRespawnX(x2 * TILE_SIZE);
					entity->setRespawnY((y2 * TILE_SIZE) - entity->getH());

					//On change la tile
					this->tile[y2][x2] += 1;
				}


				//On vérifie si les tiles recouvertes sont solides
				if (this->tile[y1][x2] > BLANK_TILE || this->tile[y2][x2] > BLANK_TILE)
				{
					// Si c'est le cas, on place le joueur aussi près que possible
					// de ces tiles, en mettant à jour ses coordonnées. Enfin, on 
					//réinitialise son vecteur déplacement (dirX).

					entity->setX(x2 * TILE_SIZE);
					entity->setX(entity->getX() - (entity->getW() + 1));
					entity->setDirX(0);

				}
			}

			//Même chose à gauche
			else if (entity->getDirX() < 0)
			{

				//Test des tiles Power-up : Etoile et vie
				if (this->tile[y1][x1] >= TILE_POWER_UP_DEBUT
					&& this->tile[y1][x1] <= TILE_POWER_UP_FIN)
				{
					//On appelle la fonction getItem()
					player->getItem(this->tile[y1][x1] - TILE_POWER_UP_DEBUT + 1, sound);

					//On remplace la tile power-up par une tile transparente
					this->tile[y1][x1] = 0;
				}
				else if (this->tile[y2][x1] >= TILE_POWER_UP_DEBUT
					&& this->tile[y2][x1] <= TILE_POWER_UP_FIN)
				{
					//On appelle la fonction getItem()
					player->getItem(this->tile[y2][x1] - TILE_POWER_UP_DEBUT + 1, sound);

					//On remplace la tile power-up par une tile transparente
					this->tile[y2][x1] = 0;
				}

				//Test de la tile checkpoint
				if (this->tile[y1][x1] == TILE_CHECKPOINT)
				{
					//On active le booléen checkpoint
					entity->setCheckpointActif(1);

					//On enregistre les coordonnées
					entity->setRespawnX(x1 * TILE_SIZE);
					entity->setRespawnY((y1 * TILE_SIZE) - entity->getH());

					//On change la tile
					this->tile[y1][x1] += 1;
				}
				else if (this->tile[y2][x1] == TILE_CHECKPOINT)
				{
					//On active le booléen checkpoint
					entity->setCheckpointActif(1);

					//On enregistre les coordonnées
					entity->setRespawnX(x1 * TILE_SIZE);
					entity->setRespawnY((y2 * TILE_SIZE) - entity->getH());

					//On change la tile
					this->tile[y2][x1] += 1;
				}


				if (this->tile[y1][x1] > BLANK_TILE || this->tile[y2][x1] > BLANK_TILE)
				{
					entity->setX((x1 + 1) * TILE_SIZE);
					entity->setDirX(0);
				}

			}

		}

		//On sort de la boucle si on a testé toutes les tiles le long de la hauteur du sprite.
		if (i == entity->getH())
		{
			break;
		}

		//Sinon, on teste les tiles supérieures en se limitant à la heuteur du sprite.
		i += TILE_SIZE;

		if (i > entity->getH())
		{
			i = entity->getH();
		}
	}

	//On recommence la même chose avec le mouvement vertical (axe des Y)
	if (entity->getW() > TILE_SIZE)
		i = TILE_SIZE;
	else
		i = entity->getW();


	for (;;)
	{
		x1 = (entity->getX()) / TILE_SIZE;
		x2 = (entity->getX() + i) / TILE_SIZE;

		y1 = (entity->getY() + entity->getDirY()) / TILE_SIZE;
		y2 = (entity->getY() + entity->getDirY() + entity->getH()) / TILE_SIZE;

		if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
		{
			if (entity->getDirY() > 0)
			{
				/* Déplacement en bas */

				//Test des tiles Power-up)
				if (this->tile[y2][x1] >= TILE_POWER_UP_DEBUT
					&& this->tile[y2][x1] <= TILE_POWER_UP_FIN)
				{
					//On appelle la fonction getItem()
					player->getItem(this->tile[y2][x1] - TILE_POWER_UP_DEBUT + 1, sound);

					//On remplace la tile power-up par une tile transparente
					this->tile[y2][x1] = 0;
				}
				else if (this->tile[y2][x2] >= TILE_POWER_UP_DEBUT
					&& this->tile[y2][x2] <= TILE_POWER_UP_FIN)
				{
					//On appelle la fonction getItem()
					player->getItem(this->tile[y2][x2] - TILE_POWER_UP_DEBUT + 1, sound);

					//On remplace la tile power-up par une tile transparente
					this->tile[y2][x2] = 0;
				}

				//Test de la tile checkpoint
				if (this->tile[y2][x1] == TILE_CHECKPOINT)
				{
					//On active le booléen checkpoint
					entity->setCheckpointActif(1);

					//On enregistre les coordonnées
					entity->setRespawnX(x1 * TILE_SIZE);
					entity->setRespawnY((y2 * TILE_SIZE) - entity->getH());

					//On change la tile
					this->tile[y2][x1] += 1;
				}
				else if (this->tile[y2][x2] == TILE_CHECKPOINT)
				{
					//On active le booléen checkpoint
					entity->setCheckpointActif(1);

					//On enregistre les coordonnées
					entity->setRespawnX(x2 * TILE_SIZE);
					entity->setRespawnY((y2 * TILE_SIZE) - entity->getH());

					//On change la tile
					this->tile[y2][x2] += 1;
				}


				/* Gestion des pics */
				if ((this->tile[y2][x1] == TILE_PIKES) || (this->tile[y2][x2] == TILE_PIKES))
				{

					//On joue le son
					sound->playSoundFx(DESTROY);
					//On fait sauter le joueur
					entity->setDirY(-JUMP_HEIGHT);

					if (entity->getLife() > 1)
					{
						//Si le timer d'invincibilité est à 0
						//on perd un coeur
						if (entity->getInvincibleTimer() == 0)
						{
							entity->setLife(entity->getLife() - 1);
							entity->setInvincibleTimer(80);
						}
					}
					else
					{
						//On met le timer à 1 pour tuer le joueur intantanément
						entity->setTimerMort(1);
						//On joue le son
						sound->playSoundFx(DESTROY);
					}
				}

				/* Gestion du ressort */
				else if ((this->tile[y2][x1] == TILE_RESSORT) || (this->tile[y2][x2] == TILE_RESSORT))
				{
					entity->setDirY(-20);
					//On indique au jeu qu'il a atterri pour réinitialiser le double saut
					entity->setOnGround(1);
					sound->playSoundFx(BUMPER);
				}


				//Gestion des plateformes traversables : elles se situent juste avant 
				//les tiles bloquantes dans notre tileset (dont la valeur butoire est 
				//BLANK_TILE). Il suffit donc d'utiliser le numéro de la première tile 
				//traversable au lieu de BLANK_TILE pour bloquer le joueur, 
				//seulement quand il tombe dessus (sinon, il passe au-travers
				//et le test n'est donc pas effectué dans les autres directions 
				else if (this->tile[y2][x1] > TILE_TRAVERSABLE || this->tile[y2][x2] > TILE_TRAVERSABLE)
				{
					//Si la tile est une plateforme ou une tile solide, on y colle le joueur et
					//on le déclare sur le sol (onGround).
					entity->setY(y2 * TILE_SIZE);
					entity->setY(entity->getY() - entity->getH());
					entity->setDirY(0);
					entity->setOnGround(1);
				}


				// Test de collision avec la plateforme mobile 
				if (plateforme->getPlateformeNumber() > 0)
					plateforme->checkCollisionsWithPlateforms(entity);

			}

			else if (entity->getDirY() < 0)
			{

				/* Déplacement vers le haut */

				//Test des tiles Power-up 
				if (this->tile[y1][x1] >= TILE_POWER_UP_DEBUT
					&& this->tile[y1][x1] <= TILE_POWER_UP_FIN)
				{
					//On appelle la fonction getItem()
					player->getItem(this->tile[y1][x1] - TILE_POWER_UP_DEBUT + 1, sound);

					//On remplace la tile power-up par une tile transparente
					this->tile[y1][x1] = 0;
				}
				if (this->tile[y1][x2] >= TILE_POWER_UP_DEBUT
					&& this->tile[y1][x2] <= TILE_POWER_UP_FIN)
				{
					//On appelle la fonction getItem()
					player->getItem(this->tile[y1][x2] - TILE_POWER_UP_DEBUT + 1, sound);

					//On remplace la tile power-up par une tile transparente
					this->tile[y1][x2] = 0;
				}

				//Test de la tile checkpoint
				if (this->tile[y1][x1] == TILE_CHECKPOINT)
				{
					//On active le booléen checkpoint
					entity->setCheckpointActif(1);

					//On enregistre les coordonnées
					entity->setRespawnX(x1 * TILE_SIZE);
					entity->setRespawnY((y1 * TILE_SIZE) - entity->getH());

					//On change la tile
					this->tile[y1][x1] += 1;
				}
				else if (this->tile[y1][x2] == TILE_CHECKPOINT)
				{
					//On active le booléen checkpoint
					entity->setCheckpointActif(1);

					//On enregistre les coordonnées
					entity->setRespawnX(x2 * TILE_SIZE);
					entity->setRespawnY((y1 * TILE_SIZE) - entity->getH());

					//On change la tile
					this->tile[y1][x2] += 1;
				}


				if (this->tile[y1][x1] > BLANK_TILE || this->tile[y1][x2] > BLANK_TILE)
				{
					entity->setY((y1 + 1) * TILE_SIZE);
					entity->setDirY(0);
				}

			}
		}

		//On teste la largeur du sprite (même technique que pour la hauteur précédemment)
		if (i == entity->getW())
		{
			break;
		}

		i += TILE_SIZE;

		if (i > entity->getW())
		{
			i = entity->getW();
		}
	}

	/* Maintenant, on applique les vecteurs de mouvement si le sprite n'est pas bloqué */
	entity->setX(entity->getX() + entity->getDirX());
	entity->setY(entity->getY() + entity->getDirY());

	//Et on contraint son déplacement aux limites de l'écran.
	if (entity->getX() < 0)
	{
		entity->setX(0);
	}

	else if (entity->getX() + entity->getW() >= this->maxX)
	{
		//Si on touche le bord droit de l'écran, on passe au niveau sup
		player->SetValeurDuNiveau(player->getLevel() + 1);
		//Si on dépasse le niveau max, on annule et on limite le déplacement du joueur
		if (player->getLevel() > LEVEL_MAX)
		{
			player->SetValeurDuNiveau(LEVEL_MAX);
			entity->setX(this->maxX - entity->getW() - 1);
		}
		//Sinon, on passe au niveau sup, on charge la nouvelle map et on réinitialise le joueur
		else
		{
			//On désactive le checkpoint
			entity->setCheckpointActif(0);

			this->changeLevel(SDL, player);
			player->reinitPlayer(map, plateforme, 0);
		}
	}

	//Maintenant, s'il sort de l'écran par le bas (chute dans un trou sans fond), on lance le timer
	//qui gère sa mort et sa réinitialisation (plus tard on gèrera aussi les vies).
	if (entity->getY() > this->maxY)
	{
		entity->setTimerMort(60);
	}
}

//Destructeur
Map::~Map()
{
	// Libère la texture du background 
	if (this->background != NULL)
	{
		SDL_DestroyTexture(this->background);
		this->background = NULL;
	}

	// Libère les textures des tilesets 
	if (this->tileSet != NULL)
	{
		SDL_DestroyTexture(this->tileSet);
		this->tileSet = NULL;
	}

	if (this->tileSetB != NULL)
	{
		SDL_DestroyTexture(this->tileSetB);
		this->tileSetB = NULL;
	}
}
