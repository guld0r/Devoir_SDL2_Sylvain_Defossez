#pragma once
class GO
{
private:
	// Points de vie/sant� + chrono d'invicibilit�
	int life, invincibleTimer;

	// Coordonn�es du sprite
	int x, y;

	// Largeur, hauteur du sprite 
	int h, w;

	// Checkpoint pour le h�ros (actif ou non)
	int checkpointActif;
	// + coordonn�es de respawn (r�apparition)
	int respawnX, respawnY;

	// Variables utiles pour l'animation :
	// Num�ro de la frame (= image) en cours + timer
	int frameNumber, frameTimer, frameMax;
	// Nombre max de frames, �tat du sprite et direction
	//   dans laquelle il se d�place (gauche / droite)
	int etat, direction;

	// Variables utiles pour la gestion des collisions :
	//Est-il sur le sol, chrono une fois mort
	int onGround, timerMort;
	//Vecteurs de d�placement temporaires avant d�tection
	//des collisions avec la map
	float dirX, dirY;
	//Sauvegarde des coordonn�es de d�part
	int saveX, saveY;

	//Variable pour le double saut
	int jump;

	//Variables pour g�rer les plateformes flottantes
	//Coordonn�es de d�part
	int beginX, beginY;
	// Type de plateforme (horizontale ou verticale)
	// Le joueur est-il dessus ?
	int type, player_dessus;

public:
	//Constructeur
	GO();

	//Accesseurs et mutateurs
	int getLife()const;
	void setLife(int value);
	int getInvincibleTimer()const;
	void setInvincibleTimer(int value);
	int getX()const;
	void setX(int value);
	int getY()const;
	void setY(int value);
	int getH()const;
	void setH(int value);
	int getW()const;
	void setW(int value);
	int getCheckpointActif()const;
	void setCheckpointActif(int value);
	int getRespawnX()const;
	void setRespawnX(int value);
	int getRespawnY()const;
	void setRespawnY(int value);
	int getFrameNumber()const;
	void setFrameNumber(int value);
	int getFrameTimer()const;
	void setFrameTimer(int value);
	int getFrameMax()const;
	void setFrameMax(int value);
	int getEtat()const;
	void setEtat(int value);
	int getDirection()const;
	void setDirection(int value);
	int getOnGround()const;
	void setOnGround(int value);
	int getTimerMort()const;
	void setTimerMort(int value);
	float getDirX()const;
	void setDirX(float value);
	float getDirY()const;
	void setDirY(float value);
	int getSaveX()const;
	void setSaveX(int value);
	int getSaveY()const;
	void setSaveY(int value);
	int getJump()const;
	void setJump(int value);
	int getBeginX()const;
	void setBeginX(int value);
	int getBeginY()const;
	void setBeginY(int value);
	int getType()const;
	void setType(int value);
	int getPlayer_Dessus()const;
	void setPlayer_Dessus(int value);

	//Destructeur
	~GO();
};

