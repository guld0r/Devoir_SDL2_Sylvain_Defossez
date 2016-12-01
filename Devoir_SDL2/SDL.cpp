#include "SDL.h"

//Constructeur
SDL::SDL(char *title, Input *input)
{

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {

		cerr << "\nUnable to initialize SDL : " << SDL_GetError() << endl;
		exit(1);
	}

	this->screen = SDL_CreateWindow(title,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		SDL_WINDOW_SHOWN);

	//On crée un renderer pour la SDL et on active la synchro verticale : VSYNC
	this->renderer = SDL_CreateRenderer(this->screen, -1, SDL_RENDERER_PRESENTVSYNC);

	// Si on n'y arrive pas, on quitte en enregistrant l'erreur dans stdout.txt
	if (this->screen == NULL || this->renderer == NULL)
	{
		cerr << "Impossible d'initialiser le mode écran à " << SCREEN_WIDTH << " x " << SCREEN_HEIGHT << " : " << SDL_GetError() << endl;
		exit(1);
	}

	//Initialisation du chargement des images png avec SDL_Image 2
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		cerr << "SDL_image n'a pu être initialisée! SDL_image Error: " << IMG_GetError() << endl;
		exit(1);
	}

	//On cache le curseur de la souris 
	SDL_ShowCursor(SDL_DISABLE);
	
	//On initialise SDL_TTF 2 qui gérera l'écriture de texte
	if (TTF_Init() < 0)
	{
		cerr << "Impossible d'initialiser SDL TTF : " << TTF_GetError() << endl;
		exit(1);
	}

	/* Chargement de la police */
	loadFont("font/GenBasB.ttf", 32);

	//On initialise SDL_Mixer 2, qui gérera la musique et les effets sonores
	int flags = MIX_INIT_MP3;
	int initted = Mix_Init(flags);
	if ((initted & flags) != flags)
	{
		cerr << "Mix_Init: Failed to init SDL_Mixer" << endl;
		cerr << "Mix_Init : " << Mix_GetError() << endl;
		exit(1);
	}

	/* Open 44.1KHz, signed 16bit, system byte order,
	stereo audio, using 1024 byte chunks (voir la doc pour plus d'infos ;) ) */
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
		cerr << "Mix_OpenAudio : " << Mix_GetError() << endl;
		exit(1);
	}

	// Définit le nombre de pistes audio (channels) à mixer
	Mix_AllocateChannels(32);

	// Initialise le sous-sytème de la SDL gérant les joysticks
	SDL_InitSubSystem(SDL_INIT_JOYSTICK);

	//On cherche s'il y a des joysticks
	if (SDL_NumJoysticks() > 0)
	{
		input->openJoystick();
	}

	//On charge l'image de la plateforme
	this->plateformeSurface = IMG_Load("graphics/plateforme.png");

	/* Si on ne peut pas, on quitte, et on l'indique en erreur ;) */
	if (this->plateformeSurface == NULL)
	{
		cerr << "Impossible de charger l'image de la plateforme : graphics/plateforme.png/n" << endl;
		exit(1);
	}

}

//Accesseurs et mutateurs
void SDL::setrenderer(SDL_Renderer *) { 
	this->renderer = renderer; 
};

int SDL::getPlateformeNumber() { 
	return this->nombrePlateformes; 
};

SDL_Texture * SDL::loadImage(char *name)
{
	/* Charge les images avec SDL Image dans une SDL_Surface */

	SDL_Surface *loadedImage = NULL;
	SDL_Texture *texture = NULL;
	loadedImage = IMG_Load(name);

	if (loadedImage != NULL)
	{
		// Conversion de l'image en texture
		texture = SDL_CreateTextureFromSurface(this->getrenderer(), loadedImage);

		// On se débarrasse du pointeur vers une surface
		SDL_FreeSurface(loadedImage);
		loadedImage = NULL;
	}
	else
		fprintf(stderr, "L'image n'a pas pu être chargée! SDL_Error :  %s\n", SDL_GetError());

	return texture;
}

void SDL::delay(unsigned int frameLimit)
{
	// Gestion des 60 fps (images/stories/seconde)
	unsigned int ticks = SDL_GetTicks();

	if (frameLimit < ticks)
	{
		return;
	}

	if (frameLimit > ticks + 16)
	{
		SDL_Delay(16);
	}

	else
	{
		SDL_Delay(frameLimit - ticks);
	}
}

void SDL::drawTile(SDL_Texture * image, int destx, int desty, int srcx, int srcy)
{
	/* Rectangle de destination à dessiner */
	SDL_Rect dest;

	dest.x = destx;
	dest.y = desty;
	dest.w = TILE_SIZE;
	dest.h = TILE_SIZE;

	/* Rectangle source */
	SDL_Rect src;

	src.x = srcx;
	src.y = srcy;
	src.w = TILE_SIZE;
	src.h = TILE_SIZE;

	/* Dessine la tile choisie sur l'écran aux coordonnées x et y */
	SDL_RenderCopy(this->getrenderer(), image, &src, &dest);
}

void SDL::loadFont(char * name, int size)
{
	/* Use SDL_TTF pour charger la police */

	font = TTF_OpenFont(name, size);

	if (font == NULL)
	{
		cerr << "Failed to open Font " << name << " : " << TTF_GetError() << endl;
		exit(1);
	}

}

void SDL::drawString(char * text, int x, int y, int r, int g, int b, int a)
{
	SDL_Rect dest;
	SDL_Surface *surface; //Pour écrire le texte
	SDL_Texture *texture; //Pour convertir la surface en texture
	SDL_Color foregroundColor;

	/* Couleur du texte RGBA */
	foregroundColor.r = r;
	foregroundColor.g = g;
	foregroundColor.b = b;
	foregroundColor.a = a;


	/* On utilise SDL_TTF pour générer une SDL_Surface à partir d'une chaîne de caractères (string) */
	surface = TTF_RenderUTF8_Blended(this->font, text, foregroundColor);

	if (surface != NULL)
	{
		/* NOUS MODIFIONS QUELQUE PEU NOTRE CODE POUR PROFITER DE LA MEMOIRE GRAPHIQUE
		QUI GERE LES TEXTURES  */
		// Conversion de l'image en texture
		texture = SDL_CreateTextureFromSurface(getrenderer(), surface);

		// On se débarrasse du pointeur vers une surface
		/* On libère la SDL_Surface temporaire (pour éviter les fuites de mémoire - cf. chapitre
		dédié) */
		SDL_FreeSurface(surface);
		surface = NULL;

		// On dessine cette texture à l'écran 
		dest.x = x;
		dest.y = y;

		SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
		SDL_RenderCopy(getrenderer(), texture, NULL, &dest);

		//On supprime la texture
		SDL_DestroyTexture(texture);

	}
	else
	{
		cerr << "La chaine n'a pu être écrite " << text << " : " << TTF_GetError() << endl;

		exit(0);
	}
}

void SDL::drawGame(int pauseMenu, Map *map, Player *player, Plateforme *plateforme, Menu *menu, SDL *SDL)
{
	
	// Affiche le fond (background) aux coordonnées (0,0) 
	drawImage(map->getBackground(), 0, 0);
	
	// Affiche la map de tiles : layer 2 (couche du fond)
	map->drawMap(2, plateforme, this);

	// Affiche la map de tiles : layer 1 (couche active : sol, etc.)
	map->drawMap(1, plateforme, this);

	// Affiche le joueur 
	player->drawPlayer(map, this);

	
	//Affiche les plateformes flottantes
	plateforme->drawPlateforme(this, map);

	// Affiche la map de tiles : layer 3 (couche en foreground / devant) 
	map->drawMap(3, plateforme, this);

	//On affiche le HUD par-dessus tout le reste
	menu->drawHUD(player, map, SDL);

	//On affiche le menu Pause, si on est en Pause
	if (pauseMenu)
		menu->drawPauseMenu(this, player);

	// Affiche l'écran 
	SDL_RenderPresent(getrenderer());

	// Délai pour laisser respirer le proc 
	SDL_Delay(1);
}

void SDL::drawImage(SDL_Texture *image, int x, int y)
{
	SDL_Rect dest;

	/* Règle le rectangle à dessiner selon la taille de l'image source */
	dest.x = x;
	dest.y = y;

	/* Dessine l'image entière sur l'écran aux coordonnées x et y */
	SDL_QueryTexture(image, NULL, NULL, &dest.w, &dest.h);
	SDL_RenderCopy(getrenderer(), image, NULL, &dest);
}

void SDL::loadGame(SDL *SDL, Player *player, Map *map, Menu *menu, Sound *sound, Plateforme *plateforme)
{
	//On commence au premier niveau
	player->SetValeurDuNiveau(1);
	map->changeLevel(this, player);

	/* On initialise les variables du jeu */
	player->setNombreDeVies(3);
	player->setNombreDetoiles(0);

	/* On charge les sounds Fx */
	sound->loadSound();

	//On commence par le menu start
	menu->setOnMenu(1, START);
}

//Destructeur
SDL::~SDL()
{
	/* Ferme police */

	if (font != NULL)
	{
		TTF_CloseFont(font);
	}

	if (this->plateformeSurface != NULL)
	{
		SDL_FreeSurface(this->plateformeSurface);
	}

	//On quitte SDL_Mixer 2 et on décharge la mémoire
	Mix_CloseAudio();
	Mix_Quit();

	//On fait le ménage et on remet les pointeurs à NULL
	SDL_DestroyRenderer(renderer);
	renderer = NULL;
	SDL_DestroyWindow(screen);
	screen = NULL;

	//On quitte SDL_TTF 2
	TTF_Quit();

	//On quitte la SDL 
	SDL_Quit();
}
