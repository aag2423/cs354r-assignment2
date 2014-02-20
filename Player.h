#ifndef __Player_h_
#define __Player_h_


#include <Ogre.h>
#include "PlayGround.h"
#include "PhysicsEngine.h"


enum PlayerStep { NORMAL_STEP = 200, RUN_STEP = 350 };
enum HitStrength { WEAK_HIT, NORMAL_HIT, STRONG_HIT };
enum shotMode{ FOREHAND = -45, BACKHAND = 45, VOLLEY = -30, SMASH = 30 };

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

class Player {
protected:
	Ogre::Entity* playerEnt;
	Ogre::SceneNode* parentNode;
	Ogre::SceneNode* paddleNode;
	Ogre::AnimationState* walkAnimation;
	Ogre::AnimationState* shootAnimation;
 
	PhysicsObject physicsObject;
	PlayGround* court;
public:
	const static Ogre::Real HEIGHT = 70.0f;
	Player(Ogre::SceneManager* mSceneMgr, PhysicsEngine& physicsEngine, PlayGround* box, const Ogre::Vector3& pos=Ogre::Vector3::ZERO);
	~Player(void);
	void move(PlayerState& playerState, const Ogre::FrameEvent& evt) ;
	void setPosition(const Ogre::Vector3& pos);
	Ogre::SceneNode* getNode(void) { return parentNode; }
	PhysicsObject& getPhysicsObject(void) { return physicsObject; }
};
#endif // #ifndef __Player_h_
