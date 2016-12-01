#pragma once
class GO
{
private:
	// Points de vie/santé + chrono d'invicibilité
	int life, invincibleTimer;

	// Coordonnées du sprite
	int x, y;

	// Largeur, hauteur du sprite 
	int h, w;

	// Checkpoint pour le héros (actif ou non)
	int checkpointActif;
	// + coordonnées de respawn (réapparition)
	int respawnX, respawnY;

	// Variables utiles pour l'animation :
	// Numéro de la frame (= image) en cours + timer
	int frameNumber, frameTimer, frameMax;
	// Nombre max de frames, état du sprite et direction
	//   dans laquelle il se déplace (gauche / droite)
	int etat, direction;

	// Variables utiles pour la gestion des collisions :
	//Est-il sur le sol, chrono une fois mort
	int onGround, timerMort;
	//Vecteurs de déplacement temporaires avant détection
	//des collisions avec la map
	float dirX, dirY;
	//Sauvegarde des coordonnées de départ
	int saveX, saveY;

	//Variable pour le double saut
	int jump;

	//Variables pour gérer les plateformes flottantes
	//Coordonnées de départ
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

