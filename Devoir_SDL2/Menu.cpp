#include "Menu.h"


//Constructeur
Menu::Menu(SDL *SDL)
{
	titlescreen = SDL->loadImage("graphics/title.png");

	/* On charge les images du HUD */
	HUD_vie = SDL->loadImage("graphics/life.png");
	HUD_etoiles = SDL->loadImage("graphics/stars.png");
}

//Accesseurs
int Menu::getOnMenu() { 
	return onMenu; 
};

int Menu::getMenuType() { 
	return menuType;
};

//Mutateur
void Menu::setOnMenu(int valeur, int type)
{
	onMenu = valeur;
	menuType = type;
}

//Fonctions
void Menu::updateStartMenu(Input *input, Player *player, Map *map, Plateforme *plateforme, SDL *SDL)
{
	
	//Si on appuie sur BAS
	if (input->getDown() == 1)
	{
		//Si choice = O (il est sur start), on le met à 1 (quit)
		if (choice == 0)
			choice++;

		input->setDown(0);
	}

	//Si on appuie sur HAUT
	if (input->getUp() == 1)
	{
		//Si choice = 1 (il est sur Quit), on le met à 0 (Start)
		if (choice == 1)
			choice--;

		input->setUp(0);
	}

	//Choix du level
	if (input->getRight() == 1)
	{
		//Si choice = 1 (il est sur Quit), on le met à 0 (Start)
		if (player->getLevel() >= LEVEL_MAX)
			player->SetValeurDuNiveau(1);
		else
			player->SetValeurDuNiveau(player->getLevel() + 1);

		input->setRight(0);
	}

	if (input->getLeft() == 1)
	{
		//Si choice = 1 (il est sur Quit), on le met à 0 (Start)
		if (player->getLevel() <= 1)
			player->SetValeurDuNiveau(LEVEL_MAX);
		else
			player->SetValeurDuNiveau(player->getLevel() - 1);

		input->setLeft(0);
	}



	//Si on appuie sur Enter ou A (manette Xbox 360) et qu'on est sur Start, on recharge le jeu et on quitte l'état menu
	if (input->getEnter() || input->getJump())
	{
		if (choice == 0)
		{
			player->resetCheckpoint();
			player->reinitPlayer(map, plateforme, 1);
			map->changeLevel(SDL, player);
			/* On réinitialise les variables du jeu */
			player->setNombreDeVies(3);
			player->setNombreDetoiles(0);
			onMenu = 0;
		}
		//Sinon, on quitte le jeu
		else if (choice == 1)
		{
			exit(0);
		}
		input->setEnter(0);
		input->setJump(0);
	}


}

void Menu::updatePauseMenu(Input * input)
{

	//Si on appuie sur BAS
	if (input->getDown() == 1)
	{
		//Si choice = O (il est sur start), on le met à 1 (quit)
		if (choice == 0)
			choice++;

		input->setDown(0);
	}

	//Si on appuie sur HAUT
	if (input->getUp() == 1)
	{
		//Si choice = 1 (il est sur Quit), on le met à 0 (Start)
		if (choice == 1)
			choice--;

		input->setUp(0);
	}

	//Si on appuie sur Enter ou A (manette Xbox 360) et qu'on est sur Start, on recharge le jeu et on quitte l'état menu
	if (input->getEnter() || input->getJump())
	{
		if (choice == 0)
		{
			//Si on appuie sur Enter on quitte l'état menu
			onMenu = 0;
		}
		//Sinon, on quitte le jeu
		else if (choice == 1)
		{
			choice = 0;
			menuType = START;
		}
		input->setEnter(0);
		input->setJump(0);
	}

}

void Menu::drawStartMenu(SDL *SDL, Player *player)
{
	//On crée une variable qui contiendra notre texte
	char text[200];

	//On affiche l'écran-titre
	SDL->drawImage(titlescreen, 0, 0);

	//Si l'option n'est pas en surbrillance, on l'affiche normalement
	if (choice != 0)
	{

		sprintf_s(text, sizeof(text), "START: Lvl %d", player->getLevel());
		//Ombrage en noir
		SDL->drawString(text, 375, 252, 0, 0, 0, 255);
		SDL->drawString(text, 373, 250, 255, 255, 255, 255);
	}
	if (choice != 1)
	{

		sprintf_s(text, sizeof(text), "QUIT");
		//Ombrage en noir
		SDL->drawString(text, 425, 292, 0, 0, 0, 255);
		SDL->drawString(text, 422, 290, 255, 255, 255, 255);
	}

	//Si l'option est en surbrillance, on change la couleur
	if (choice == 0)
	{

		sprintf_s(text, sizeof(text), "START: Lvl %d", player->getLevel());
		//Ombrage en noir
		SDL->drawString(text, 375, 252, 0, 0, 0, 255);
		SDL->drawString(text, 373, 250, 255, 255, 0, 255);
	}
	else if (choice == 1)
	{

		sprintf_s(text, sizeof(text), "QUIT");
		//Ombrage en noir
		SDL->drawString(text, 425, 292, 0, 0, 0, 255);
		SDL->drawString(text, 422, 290, 255, 255, 0, 255);
	}


}

void Menu::drawPauseMenu(SDL *SDL, Player *player)
{
	char text[200];

	//On écrit PAUSE

	sprintf_s(text, sizeof(text), "** PAUSE **");
	SDL->drawString(text, 322, 200, 0, 0, 0, 255);
	SDL->drawString(text, 320, 198, 255, 255, 255, 255);


	//Si l'option n'est pas en surbrillance, on l'affiche normalement
	if (choice != 0)
	{

		sprintf_s(text, sizeof(text), "Continue");
		//Ombrage en noir
		SDL->drawString(text, 346, 252, 0, 0, 0, 255);
		SDL->drawString(text, 344, 250, 255, 255, 255, 255);
	}
	if (choice != 1)
	{

		sprintf_s(text, sizeof(text), "Exit");
		//Ombrage en noir
		SDL->drawString(text, 386, 292, 0, 0, 0, 255);
		SDL->drawString(text, 384, 290, 255, 255, 255, 255);
	}

	//Si l'option est en surbrillance, on change la couleur
	if (choice == 0)
	{

		sprintf_s(text, sizeof(text), "Continue");
		//Ombrage en noir
		SDL->drawString(text, 346, 252, 0, 0, 0, 255);
		SDL->drawString(text, 344, 250, 255, 255, 0, 255);
	}
	else if (choice == 1)
	{

		sprintf_s(text, sizeof(text), "Exit");
		//Ombrage en noir
		SDL->drawString(text, 386, 292, 0, 0, 0, 255);
		SDL->drawString(text, 384, 290, 255, 255, 0, 255);
	}


}

void Menu::drawHUD(Player *player, Map *map, SDL *SDL)
{
	//On crée une varuiable qui contiendra notre texte (jusqu'à 200 caractères, y'a de la marge ;) ).
	char text[200];

	int i;

	//Affiche le nombre de coeurs
	//On crée une boucle pour afficher de 1 à 3 coeurs
	//selon la vie, avec un décalage de 32 pixels
	for (i = 0; i < player->getLife(); i++)
	{
		// Calcul pour découper le tileset comme dans la fonction drawMap()
		int ysource = TILE_POWER_UP_COEUR / 10 * TILE_SIZE;
		int xsource = TILE_POWER_UP_COEUR % 10 * TILE_SIZE;

		SDL->drawTile(map->getTileSetA(), 60 + i * 32, 20, xsource, ysource);
	}

	/* Affiche le nombre de vies en bas à droite - Adaptation à la fenêtre auto */
	SDL->drawImage(HUD_vie, SCREEN_WIDTH - 120, SCREEN_HEIGHT - 70);

	//Pour afficher le nombre de vies, on formate notre string pour qu'il prenne la valeur de la variable
	//sprintf(text, "x %d", getNombreDeVies());   //Version dépréciée remplacée par :
	sprintf_s(text, sizeof(text), "x %d", player->getNombreDeVies());


	//Puis on utilise notre fonction créée précédemment pour écrire en noir (0, 0, 0, 255)
	//et en blanc (255, 255, 255, 255) afin de surligner le texte et de le rendre plus
	//visible
	SDL->drawString(text, SCREEN_WIDTH - 80, SCREEN_HEIGHT - 60, 0, 0, 0, 255);
	SDL->drawString(text, SCREEN_WIDTH - 82, SCREEN_HEIGHT - 62, 255, 255, 255, 255);

	/* Affiche le nombre d'étoiles en haut à gauche */
	SDL->drawImage(HUD_etoiles, 60, 60);

	sprintf_s(text, sizeof(text), "%d", player->getNombreDetoiles());
	SDL->drawString(text, 100, 57, 0, 0, 0, 255);
	SDL->drawString(text, 98, 55, 255, 255, 255, 255);
}

//Destructeur
Menu::~Menu()
{
	// Libère la texture de l'écran-titre 
	if (titlescreen != NULL)
	{
		SDL_DestroyTexture(titlescreen);
		titlescreen = NULL;
	}

	//On libère les ressources du HUD
	if (HUD_etoiles != NULL)
	{
		SDL_DestroyTexture(HUD_etoiles);
	}
	if (HUD_vie != NULL)
	{
		SDL_DestroyTexture(HUD_vie);
	}
}
