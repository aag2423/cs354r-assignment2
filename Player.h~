#ifndef __Player_h_
#define __Player_h_


#include <Ogre.h>
#include "PlayGround.h"
#include "PhysicsEngine.h"


enum PlayerType { HUMAN, AI };
enum PlayerStep { SMALL_STEP = 100, NORMAL_STEP = 200, RUN_STEP = 350 };
enum HitStrength { WEAK_HIT = 50, NORMAL_HIT = 100, STRONG_HIT = 150 };
enum shotMode{ FOREHAND = -30, BACKHAND = 30, VOLLEY = 35, SMASH = -10 };

typedef struct PlayerState {
	Ogre::Vector3 shotDirection;
	enum PlayerStep step;
	enum HitStrength strength;
	enum PlayerType type;
	bool movingLeft;
	bool movingRight;
	bool movingForward;
	bool movingBackward;
	bool hitting;
} PlayerState;

class Player {
protected:
	Ogre::SceneManager* graphicsEngine;
	Ogre::Entity* playerEnt;
	Ogre::SceneNode* parentNode;
	Ogre::SceneNode* playerNode;
	Ogre::SceneNode* paddleNode;
	Ogre::AnimationState* walkAnimation;
	Ogre::AnimationState* shootAnimation;
 
	bool visible;
	PhysicsObject physicsObject;
	PhysicsEngine* engine;
	PlayGround* court;
public:
	PlayerState playerState;
	const static Ogre::Real HEIGHT = 70.0f;
	Player(Ogre::SceneManager* mSceneMgr, PhysicsEngine& physicsEngine, PlayerType type, PlayGround* box, const Ogre::Vector3& pos=Ogre::Vector3::ZERO);
	~Player(void);
	void toggleVisible(void);
	void resetState(void);
	void move(const Ogre::FrameEvent& evt, bool isFullGame=true) ;
	void setPosition(const Ogre::Vector3& pos);
	Ogre::SceneNode* getNode(void) { return parentNode; }
	PhysicsObject& getPhysicsObject(void) { return physicsObject; }
};
#endif // #ifndef __Player_h_
