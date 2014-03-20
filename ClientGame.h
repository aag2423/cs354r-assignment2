#ifndef __ClientGame_h_
#define __ClientGame_h_
#include "Game.h"

class ClientGame
{
private:
	Ogre::SceneManager* graphicsEngine;
	enum CameraMode camMode;
	Ogre::SceneNode* cameraNode;
	InputPlayerState localPlayerState;
	OutputState serverState;

	bool isNearSide;
	bool isServerFullGame;
	ClientCourt* cCourt;
	ClientPlayer* cPlayer1;
	ClientPlayer* cPlayer2;
	ClientBall* cBall;

	Sound* soundPlayer;
public:
	ClientGame(Ogre::SceneManager* mSceneMgr, Ogre::SceneNode* camNod, InitializationData& iniData, OutputState serverState);
	~ClientGame(void);
	ClientCourt* getCourt(void) { return cCourt; }
	InputState send(void);
	bool isFullGame(void) { return isServerFullGame; }
	void toggleCamera(void);
	void receive(OutputState newState);
	void runNextFrame(void);
	void handleKeyboardEvent(enum KeyboardEvent evt);
	void handleMouseMove(Ogre::Real dx, Ogre::Real dy);
	void handleMouseClick(enum MouseEvent evt);
	int getPlayerScore(void) { return serverState.gameState.playerScore; }
	int getOpponentScore(void) { return serverState.gameState.opponentScore; }
};

#endif // #ifndef __ClientGame_h_