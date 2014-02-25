
/*
create game instance
game.setup(scenemgr, camNode);
game.reset();







*/



#ifndef __Game_h_
#define __Game_h_

#include "PhysicsEngine.h"
#include "Ball.h"
#include "PlayGround.h"
#include "Player.h"
#include "Target.h"
#include <OgreVector3.h>
#include "Sound.h"

enum {
	BALL_SERVE_SPEED = 100
};
enum Gravity { EARTH_G = -10, MOON_G = -2 };
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
enum RoundProgress {
	HIT_BY_PLAYER, HIT_BY_PLAYER_AND_FLOOR, 
	HIT_BY_OPPONENT, HIT_BY_OPPONENT_AND_FLOOR, 
	ENDED
};
enum GameResult {
	ONGOING, WIN, LOSE
};

typedef struct GameState {
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

	int getScore(void) { 
		if (gameMode == PRACTICE)
			return target1->getScore() + target2->getScore() + target3->getScore(); 
		else
			return gameState.playerScore;
	}
	int getOpponentScore(void) { return gameState.computerScore; }
	int getComboBonus(void) { return gameState.comboBonus; }
	void handleKeyboardEvent(enum KeyboardEvent evt);
	void handleMouseClick(enum MouseEvent evt);
	void handleMouseMove(Ogre::Real dx, Ogre::Real dy);
	void runNextFrame(const Ogre::FrameEvent& evt);
};

//GLOBAL SOUND OBJECT INSTANCE
static Sound* soundHandler;

#endif // #ifndef __Game_h_
