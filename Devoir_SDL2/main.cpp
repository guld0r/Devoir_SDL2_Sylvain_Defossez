#include "resources.h"
#include "SDL.h"
#include "Input.h"
#include "Map.h"
#include "Menu.h"
#include "Plateforme.h"
#include "Player.h"
#include "Sound.h"
#include "GO.h"

int main(int argc, char *argv[])
{
	int go;
	unsigned int frameLimit = SDL_GetTicks() + 16;

	//On initialise nos inputs
	Input input;

	//On initialise les outils de la SDL dont la fenêtre
	SDL SDL("Rabidja C++ - SDL 2 - Sylvain Defossez", &input);
	
	//On charge les données pour la map
	Map map(&SDL);

	//On initialise les menus
	Menu menu(&SDL);

	//On charge notre plateforme
	Plateforme plateforme(&SDL);

	//On initialise notre joueur
	Player player(&map, &plateforme, 1);

	//On charge la musique
	Sound sound("music/Caviator.mp3");

	//On charge la feuille de sprites (spritesheet) de notre héros 
	player.initPlayerSprites(&SDL);
	
	// Chargement des ressources (graphismes, sons)
	SDL.loadGame(&SDL, &player, &map, &menu, &sound, &plateforme);

	go = 1;

	// Boucle infinie, principale, du jeu 
	while (go == 1)
	{
		
		//Gestion des inputs et du joystick
		input.gestionInputs(&input);
		
		//Si on n'est pas dans un menu
		if (menu.getOnMenu() == 0)
		{
			// On met à jour le jeu 
			player.updatePlayer(&input, &map, &sound, &menu, &plateforme, &SDL);

			plateforme.doPlateforme(&player);
		}
		else
		{
			if (menu.getMenuType() == START) 
			{
				menu.updateStartMenu(&input, &player, &map, &plateforme, &SDL);
			}
			else if (menu.getMenuType() == PAUSE) 
			{
				menu.updatePauseMenu(&input);
			}
		}


		//Si on n'est pas dans un menu, on affiche le jeu
		if (menu.getOnMenu() == 0)
			SDL.drawGame(0, &map, &player, &plateforme, &menu, &SDL);

		else
		{
			if (menu.getMenuType() == START)
			{
				menu.drawStartMenu(&SDL, &player);
				SDL_RenderPresent(SDL.getrenderer());
				SDL_Delay(1);
			}
			else if (menu.getMenuType() == PAUSE) 
			{
				SDL.drawGame(1, &map, &player, &plateforme, &menu, &SDL);
			}

		}

		// Gestion des 60 fps (1000ms/60 = 16.6 -> 16 
		SDL.delay(frameLimit);
		frameLimit = SDL_GetTicks() + 16;
	}

	getchar();

	return 0;
}