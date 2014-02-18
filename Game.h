
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
#include <OgreVector3.h>

enum {
	COURT_LENGTH = 400,
	COURT_WIDTH = 200,
	COURT_HEIGHT = 200,
	BALL_SERVE_SPEED = 100
};

enum PlayerStep { NORMAL_STEP = 200, RUN_STEP = 350 };
enum HitStrength { WEAK_HIT, NORMAL_HIT, STRONG_HIT };
enum Gravity { EARTH_G = -10, MOON_G = -2 };
enum CameraMode { ABOVE_CAM, FIRST_PERSON_CAM };
enum shotHorizontalMode{ FOREHAND = -45, BACKHAND = 45, VOLLEY = -20, SMASH = 45 };

enum KeyboardEvent {
	GO_LEFT, GO_RIGHT, GO_FORWARD, GO_BACKWARD,		// player movement
	STOP_LEFT, STOP_RIGHT, STOP_FORWARD, STOP_BACKWARD,	// player movement
	RUN, STOP_RUN, USE_WEAK_HIT, USE_NORMAL_HIT, USE_STRONG_HIT,	// change player state
	TOGGLE_CAMERA, PAUSE, RESTART				// overall game params
};
enum MouseEvent { HIT_START, HIT_STOP };


typedef struct GameState {
	enum CameraMode camMode;
	bool paused;	// whether the game is paused (in menu)
	bool gameStarted;
} GameState;

typedef struct PlayerState {
	Ogre::Vector3 shotDirection;
	enum PlayerStep step;
	enum HitStrength strength;
	bool movingLeft;
	bool movingRight;
	bool movingForward;
	bool movingBackward;
	bool hitting;
} PlayerState;

class Game
{
protected:
	Ogre::SceneManager* graphicsEngine;
	PhysicsEngine physicsEngine;
	Ogre::SceneNode* cameraNode;

	GameState gameState;

	Ball* ball;
	PlayGround* court;
	Player* player;

	PlayerState playerState;
private:
	void movePlayer(const Ogre::FrameEvent& evt);
	void toggleCamera(void);
public:
	Game(Ogre::SceneManager* mSceneMgr, Ogre::SceneNode* camNod);
 	~Game(void);

	void handleKeyboardEvent(enum KeyboardEvent evt);
	void handleMouseClick(enum MouseEvent evt);
	void handleMouseMove(Ogre::Real dx, Ogre::Real dy);	
	void reset(void);
	void runNextFrame(const Ogre::FrameEvent& evt);
	void serveBall(void);
};

#endif // #ifndef __Game_h_
