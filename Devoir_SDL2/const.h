#pragma once

#ifndef DEF_DEFS
#define DEF_DEFS

/* define buffer données et port communication socket*/
#define DEFAULT_BUFLEN 256
#define DEFAULT_PORT "27015"

// Taille de la fenêtre : 800x480 pixels 
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 480

/* Taille maxi de la map : 400 x 150 tiles */
#define MAX_MAP_X 400
#define MAX_MAP_Y 150

/* Taille d'une tile (32 x 32 pixels) */
#define TILE_SIZE 32

/* Constantes pour l'animation */
#define TIME_BETWEEN_2_FRAMES 20
#define TIME_BETWEEN_2_FRAMES_PLAYER 4

/* Taille du sprite de notre héros (largeur = width et hauteur = heigth) */
#define PLAYER_WIDTH 40
#define PLAYER_HEIGTH 50

//Vitesse de déplacement en pixels du sprite
#define PLAYER_SPEED 4

//Valeurs attribuées aux états/directions
#define IDLE 0
#define WALK 1
#define JUMP1 2
#define JUMP2 3
#define DEAD 4

#define RIGHT 1
#define LEFT 2

//Constante pour les limites de la caméra avant scrolling
#define LIMITE_X 400
#define LIMITE_Y 220
#define LIMITE_W 100
#define LIMITE_H 80

//Constantes définissant la gravité et la vitesse max de chute
#define GRAVITY_SPEED 0.6
#define MAX_FALL_SPEED 15
#define JUMP_HEIGHT 10

/* VALEURS DES TILES (cf. chapitre 7) */

// Constante définissant le seuil entre les tiles traversables
// (blank) et les tiles solides
#define BLANK_TILE 89

//Plateformes traversables
#define TILE_TRAVERSABLE 70

//Tiles Power-ups
#define TILE_POWER_UP_DEBUT 67
#define TILE_POWER_UP_FIN 69
#define TILE_POWER_UP_COEUR 68

//Autres Tiles spéciales
#define TILE_RESSORT 115
#define TILE_CHECKPOINT 23
#define TILE_MONSTRE 126
#define TILE_PIKES 117

//Tiles plateformes mobiles
#define TILE_PLATEFORME_DEBUT 120
#define TILE_PLATEFORME_FIN 121

//Dead zone de la manette pour éviter les mouvements involontaires
#define DEAD_ZONE 8000

//Mapping de la manette Xbox 360
#define BOUTON_HAUT 0
#define BOUTON_BAS 1
#define BOUTON_GAUCHE 2
#define BOUTON_DROITE 3

#define BOUTON_SAUT 10
#define BOUTON_ATTAQUE 12
#define BOUTON_PAUSE 4
#define BOUTON_QUIT 5

//Une enum pour la gestion des sons.
enum
{
	BUMPER,
	DESTROY,
	JUMP,
	STAR
};

/* Gestion des plateformes mobiles */
#define PLATEFORMES_MAX 50
#define PLATEFORM_SPEED 2
#define UP 3
#define DOWN 4

//Nombre max de levels
#define LEVEL_MAX 2

//Une enum pour la gestion du menu.
enum
{
	START,
	PAUSE
};

#endif
