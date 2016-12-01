#pragma once
#include "Resources.h"

class Input
{
private:
	int left, right, up, down, jump, attack, enter, erase, pause;
	SDL_Joystick *joystick;
	int DPADinUse;

public:
	//Constructeur
	Input();

	//Accesseurs et mutateurs
	int getLeft() const;
	int getRight() const;
	int getUp() const;
	int getDown() const;
	int getJump() const;
	int getAttack() const;
	int getEnter() const;
	int getErase() const;
	int getPause() const;
	void setLeft(int input);
	void setRight(int input);
	void setUp(int input);
	void setDown(int input);
	void setJump(int input);
	void setAttack(int input);
	void setEnter(int input);
	void setErase(int input);
	void setPause(int input);

	//Fonctions
	void gestionInputs(Input *input);
	void getInput(Input *input);
	void getJoystick(Input *input);
	void openJoystick();

	//Destructeur
	~Input();
};

