#ifndef __ServerGame_h_
#define __ServerGame_h_
#include "Game.h"

typedef struct ServerGameState {
	bool paused;
	bool gameStarted;
	bool restart;
	Ogre::Real ballRestitution;
	int playerScore;
	int computerScore;
	int comboBonus;
	enum RoundProgress progress;
	GameResult result;
} ServerGameState;

class ServerGame
{
private:
	Ogre::SceneManager* graphicsEngine;
	PhysicsEngine physicsEngine;
	InitializationData initData;
	OutputState outputState;
	ServerGameState gameState;
	InputPlayerState player1Input;
	InputPlayerState player2Input;

	bool isFullGame;
	enum AppMode appMode;
	ServerCourt* sCourt;
	ServerPlayer* sPlayer1;
	ServerPlayer* sPlayer2;
	ServerBall* sBall;
	void outputPositionState(void);
	void outputPauseState(void);
public:
	ServerGame(Ogre::SceneManager* mSceneMgr, AppMode playMode=SINGLE_PLAYER, GameMode mode=FULL_GAME, Gravity g=EARTH_G, Ogre::Real ballRestitution=0.99);
	~ServerGame(void);
	InitializationData& getInitializationData(PlayerSide serverSide) { 
		initData.isClientNearSide = serverSide == SIDE_NEAR;
		return initData;
	}
	void setBallRestitution(Ogre::Real r) {
		if(r == 1) r = 0.99;
		gameState.ballRestitution = r;
		sBall->getPhysicsObject().setRestitution(r);
	}
	void checkScoring(BallCollisionEvent ballEvent);
	void receive(InputState input);
	void restart(void);
	void runAI(void);
	void runNextFrame(void);
	OutputState send(bool isClientNearSide);
};

#endif // #ifndef __ServerGame_h_
