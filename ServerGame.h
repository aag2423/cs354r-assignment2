#ifndef __ServerGame_h_
#define __ServerGame_h_
#include "Game.h"

class ServerGame
{
private:
	Ogre::SceneManager* graphicsEngine;
	PhysicsEngine physicsEngine;
	InitializationData initData;
	OutputState outputState;

	InputPlayerState player1Input;
	InputPlayerState player2Input;
	Gravity g;
	Ogre::Real ballRes;

	bool isFullGame;
	enum AppMode appMode;
	ServerCourt* sCourt;
	ServerPlayer* sPlayer1;
	ServerPlayer* sPlayer2;
	ServerBall* sBall;
	void outputPositionState(void);
	void outputPauseState(void);
public:
	ServerGame(Ogre::SceneManager* mSceneMgr, AppMode aMode=SINGLE_PLAYER, GameMode mode=FULL_GAME, Gravity g=EARTH_G, Ogre::Real ballRestitution=0.99);
	~ServerGame(void);
	InitializationData& getInitializationData(PlayerSide serverSide) { 
		initData.isClientNearSide = serverSide == SIDE_NEAR;
		return initData;
	}
	void setBallRestitution(Ogre::Real r) {
		if(r == 1) r = 0.99;
		ballRes = r;
		sBall->getPhysicsObject().setRestitution(r);
	}
	void toggleGravity(void) { 
		if (g == EARTH_G) {
			physicsEngine.setGravity(0, MOON_G, 0); 
			g = MOON_G;
		} else {
			physicsEngine.setGravity(0, EARTH_G, 0); 
			g = EARTH_G;
		}
	}
	void checkScoring(BallCollisionEvent ballEvent);
	void receive(InputState input);
	void restart(void);
	void runAI(void);
	void runNextFrame(void);
	OutputState send(bool isClientNearSide);
};

#endif // #ifndef __ServerGame_h_
