#include "Input.h"


//Constructeur
Input::Input()
{
	this->left = 0;
	this->right = 0;
	this->up = 0;
	this->down = 0;
	this->jump = 0;
	this->attack = 0;
	this->enter = 0;
	this->erase = 0;
	this->pause = 0;
	this->DPADinUse = 0;
}

//Accesseurs et mutateurs
int Input::getLeft() const { 
	return this->left;
};

int Input::getRight() const {
	return this->right;
};

int Input::getUp() const {
	return this->up; 
};

int Input::getDown() const {
	return this->down; 
};

int Input::getJump() const {
	return this->jump;
};

int Input::getAttack() const {
	return this->attack;
};

int Input::getEnter() const {
	return this->enter; 
};

int Input::getErase() const {
	return this->erase;
};

int Input::getPause() const {
	return this->pause; 
};

void Input::setLeft(int input) {
	this->left = input;
};

void Input::setRight(int input) {
	this->right = input;
};

void Input::setUp(int input) {
	this->up = input; 
};

void Input::setDown(int input) {
	this->down = input;
};

void Input::setJump(int input) {
	this->jump = input;
};

void Input::setAttack(int input) {
	this->attack = input;
};

void Input::setEnter(int input) {
	this->enter = input;
};

void Input::setErase(int input) {
	this->erase = input;
};

void Input::setPause(int input) {
	this->pause = input;
};

//Fonctions
void Input::gestionInputs(Input *input)
{
	/* On prend en compte les inputs (clavier, joystick... */
	if (this->joystick != NULL)
	{
		//On v�rifie si le joystick est toujours connect�
		if (SDL_NumJoysticks() > 0)
			getJoystick(input);
		//Sinon on retourne au clavier
		else
		{
			SDL_JoystickClose(this->joystick);
			this->joystick = NULL;
		}

	}
	//S'il n'y a pas de manette on g�re le clavier
	else
	{
		//On v�rifie d'abord si une nouvelle manette a �t� branch�e
		if (SDL_NumJoysticks() > 0)
		{
			//Si c'est le cas, on ouvre le joystick, qui sera op�rationnel au prochain tour de boucle
			this->joystick = SDL_JoystickOpen(0);
			if (!this->joystick)
				cerr << "Couldn't open Joystick 0" << endl;
		}
		//On g�re le clavier
		getInput(input);
	}
}


void Input::getInput(Input *input)
{
	SDL_Event event;

	/* Keymapping : g�re les appuis sur les touches et les enregistre
	dans la structure input */

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{

		case SDL_QUIT:
			exit(0);
			break;

		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				exit(0);
				break;

			case SDLK_DELETE:
				input->erase = 1;
				break;

			case SDLK_c:
				input->jump = 1;
				break;

			case SDLK_v:
				input->attack = 1;
				break;

			case SDLK_LEFT:
				input->left = 1;
				break;

			case SDLK_RIGHT:
				input->right = 1;
				break;

			case SDLK_DOWN:
				input->down = 1;
				break;

			case SDLK_UP:
				input->up = 1;
				break;


			case SDLK_RETURN:
				input->enter = 1;
				break;


			default:
				break;
			}
			break;

		case SDL_KEYUP:
			switch (event.key.keysym.sym)
			{
			case SDLK_DELETE:
				input->erase = 0;
				break;

			case SDLK_c:
				input->jump = 0;
				break;

			case SDLK_LEFT:
				input->left = 0;
				break;

			case SDLK_RIGHT:
				input->right = 0;
				break;

			case SDLK_DOWN:
				input->down = 0;
				break;

			case SDLK_UP:
				input->up = 0;
				break;

			case SDLK_RETURN:
				input->enter = 0;
				break;

			default:
				break;
			}
			break;

		}

	}
}


void Input::getJoystick(Input *input)
{
	SDL_Event event;

	//Si on ne touche pas au D-PAD, on le d�sactive (on teste les 4 boutons du D-PAD)
	if (SDL_JoystickGetButton(this->joystick, BOUTON_HAUT) == 0 && SDL_JoystickGetButton(this->joystick, BOUTON_BAS) == 0
		&& SDL_JoystickGetButton(this->joystick, BOUTON_DROITE) == 0 && SDL_JoystickGetButton(this->joystick, BOUTON_GAUCHE) == 0)
		DPADinUse = 0;

	/* On passe les events en revue */
	while (SDL_PollEvent(&event))
	{


		if (event.type == SDL_QUIT)
			exit(0);


		else if (event.type == SDL_KEYDOWN)
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				exit(0);
				break;

			default:
				break;

			}
		}

		else if (event.type == SDL_JOYBUTTONDOWN)
		{

			if (event.jbutton.button == BOUTON_SAUT)
				input->jump = 1;

			else if (event.jbutton.button == BOUTON_ATTAQUE)
				input->attack = 1;

			else if (event.jbutton.button == BOUTON_PAUSE)
				input->enter = 1;

			else if (event.jbutton.button == BOUTON_QUIT)
				exit(0);

			else if (event.jbutton.button == BOUTON_HAUT)
			{
				input->up = 1;
				DPADinUse = 1;
			}

			else if (event.jbutton.button == BOUTON_BAS)
			{
				input->down = 1;
				DPADinUse = 1;
			}

			else if (event.jbutton.button == BOUTON_GAUCHE)
			{
				input->left = 1;
				DPADinUse = 1;
			}

			else if (event.jbutton.button == BOUTON_DROITE)
			{
				input->right = 1;
				DPADinUse = 1;
			}

		}

		else if (event.type == SDL_JOYBUTTONUP)
		{
			if (event.jbutton.button == BOUTON_PAUSE)
				input->enter = 0;

			else if (event.jbutton.button == BOUTON_SAUT)
				input->jump = 0;

			else if (event.jbutton.button == BOUTON_HAUT)
				input->up = 0;

			else if (event.jbutton.button == BOUTON_BAS)
				input->down = 0;

			else if (event.jbutton.button == BOUTON_GAUCHE)
				input->left = 0;

			else if (event.jbutton.button == BOUTON_DROITE)
				input->right = 0;

		}

		//Gestion du thumbpad, seulement si on n'utilise pas d�j� le D-PAD
		else if (event.type == SDL_JOYAXISMOTION && DPADinUse == 0)
		{
			//Si le joystick a d�tect� un mouvement
			if (event.jaxis.which == 0)
			{
				//Si le mouvement a eu lieu sur l'axe des X
				if (event.jaxis.axis == 0)
				{
					//Si l'axe des X est neutre ou � l'int�rieur de la "dead zone"
					if ((event.jaxis.value > -DEAD_ZONE) && (event.jaxis.value < DEAD_ZONE))
					{
						input->right = 0;
						input->left = 0;
					}
					//Sinon, de quel c�t� va-t-on ?
					else
					{
						//Si sa valeur est n�gative, on va � gauche
						if (event.jaxis.value < -DEAD_ZONE)
						{
							input->right = 0;
							input->left = 1;
						}
						//Sinon, on va � droite
						else if (event.jaxis.value > DEAD_ZONE)
						{
							input->right = 1;
							input->left = 0;
						}
					}
				}

				//Si le mouvement a eu lieu sur l'axe des Y
				else if (event.jaxis.axis == 1)
				{
					//Si l'axe des Y est neutre ou � l'int�rieur de la "dead zone"
					if ((event.jaxis.value > -DEAD_ZONE) && (event.jaxis.value < DEAD_ZONE))
					{
						input->up = 0;
						input->down = 0;
					}
					//Sinon, de quel c�t� va-t-on ?
					else
					{
						//Si sa valeur est n�gative, on va en haut
						if (event.jaxis.value < 0)
						{
							input->up = 1;
							input->down = 0;
						}
						//Sinon, on va en bas
						else
						{
							input->up = 0;
							input->down = 1;
						}
					}
				}
			}
		}

	}

}

void Input::openJoystick()
{
	//On ouvre le joystick
	this->joystick = SDL_JoystickOpen(0);

	if (!this->joystick)
		cerr << "Le joystick 0 n'a pas pu �tre ouvert !" << endl;
}

//Destructeur
Input::~Input()
{
	/* Ferme la prise en charge du joystick */
	if (SDL_JoystickGetAttached(joystick))
		SDL_JoystickClose(joystick);
}
