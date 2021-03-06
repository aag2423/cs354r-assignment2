#ifndef __Game_h_
#define __Game_h_

#include "PhysicsEngine.h"
#include "Ball.h"
#include "PlayGround.h"
#include "Player.h"
#include "Target.h"
#include <OgreVector3.h>
#include "Sound.h"
#include "ServerPlayer.h"
#include "ServerCourt.h"
#include "ServerBall.h"
#include "ClientCourt.h"
#include "ClientPlayer.h"
#include "ClientBall.h"

enum {
	BALL_SERVE_SPEED = 100
};
enum Gravity { EARTH_G = -15, MOON_G = -2 };
enum CameraMode { ABOVE_CAM, THIRD_PERSON_CAM, FIRST_PERSON_CAM };


enum KeyboardEvent {
	GO_LEFT, GO_RIGHT, GO_FORWARD, GO_BACKWARD,		// player movement
	STOP_LEFT, STOP_RIGHT, STOP_FORWARD, STOP_BACKWARD,	// player movement
	RUN, STOP_RUN, USE_WEAK_HIT, USE_NORMAL_HIT, USE_STRONG_HIT,	// change player state
	TOGGLE_CAMERA, PAUSE, RESTART, TOGGLE_GAME_MODE		// overall game params
};
enum MouseEvent { HIT_START, HIT_STOP };
enum GameMode {
	PRACTICE, FULL_GAME
};
enum AppMode {
	NO_GAME, SINGLE_PLAYER, MULTI_PLAYER_SERVER, MULTI_PLAYER_CLIENT
};
enum RoundProgress {
	HIT_BY_PLAYER, HIT_BY_PLAYER_AND_FLOOR, 
	HIT_BY_OPPONENT, HIT_BY_OPPONENT_AND_FLOOR, 
	NOT_STARTED, ENDED
};
enum GameResult {
	ONGOING, WIN, LOSE
};

typedef struct GameState {
	Gravity g;
	float ballRestitution;
	enum CameraMode camMode;
	bool paused;	// whether the game is paused (in menu)
	bool gameStarted;
	bool combo;
	int comboBonus;
	int playerScore;
	int computerScore;
	enum RoundProgress progress;
	GameResult result;
} GameState;


typedef struct InitializationData {
	bool isClientNearSide;
	GameMode gameMode;
	GameResult result; 
} InitializationData;

typedef struct OutputSceneState {
	Ogre::Vector3 ballPos;
	Ogre::Quaternion ballOrient;
	Ogre::Vector3 playerNearPos;
	Ogre::Vector3 playerFarPos;
	Ogre::Quaternion playerNearOrient;
	Ogre::Quaternion playerFarOrient;
	bool playerNearMoving;
	bool playerFarMoving;
	bool playerNearHitting;
	bool playerFarHitting;
	bool isClientNearSide;
} OutputSceneState;

typedef struct OutputGameState {
	bool paused;
	bool gameStarted;
	bool restart;
	int playerScore;
	int opponentScore;
	int comboBonus;
	enum RoundProgress progress;
	GameResult result;
} OutputGameState;

typedef struct OutputState {
	OutputGameState gameState;
	OutputSceneState sceneState;
	BallCollisionEvent collisionEvent;
} OutputState;

typedef struct InputState {
	InputPlayerState playerState;
	bool startRound;
	bool startGame;
} InputState;

class Game
{
protected:
	Ogre::SceneManager* graphicsEngine;
	PhysicsEngine physicsEngine;
	Ogre::SceneNode* cameraNode;
	GameMode gameMode;
	GameState gameState;
	Ball* ball;
	PlayGround* court;
	Player* player;
	Player* computer;
	Target* target1;
	Target* target2;
	Target* target3;
	Target* wall;
private:
	void toggleGameMode(void);
	void toggleCamera(void);
	void checkScoring(BallCollisionEvent ballEvent);
	void rePosition(void);
	void reset(void);
	void runAI(void);
public:
	Game(Ogre::SceneManager* mSceneMgr, Ogre::SceneNode* camNod, GameMode mode=FULL_GAME);
 	~Game(void);
	
	PlayGround* getCourt(void) { return court; }
	void setBallRestitution(Ogre::Real r) {
		if(r == 1) r = 0.99;
		gameState.ballRestitution = r;
		ball->getPhysicsObject().setRestitution(r); }
	void toggleGravity(void) { 
		if (gameState.g == EARTH_G) {
			physicsEngine.setGravity(0, MOON_G, 0); 
			gameState.g = MOON_G;
		} else {
			physicsEngine.setGravity(0, EARTH_G, 0); 
			gameState.g = EARTH_G;
		}
	}
	
	int getScore(void) { 
		if (gameMode == PRACTICE)
			return target1->getScore() + target2->getScore() + target3->getScore(); 
		else
			return gameState.playerScore;
	}
	int getOpponentScore(void) { return gameState.computerScore; }
	int getComboBonus(void) { return gameState.comboBonus; }
	HitStrength getPlayerHitStrength(void) { return player->playerState.strength; }
	GameMode getGameMode(void) { return gameMode; }
	GameResult getGameResult(void) { return gameState.result; }
	void handleKeyboardEvent(enum KeyboardEvent evt);
	void handleMouseClick(enum MouseEvent evt);
	void handleMouseMove(Ogre::Real dx, Ogre::Real dy);
	void runNextFrame(const Ogre::FrameEvent& evt);
};

//GLOBAL SOUND OBJECT INSTANCE
static Sound* soundHandler;

#endif // #ifndef __Game_h_
