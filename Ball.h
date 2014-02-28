
#ifndef __Ball_h_
#define __Ball_h_


#include <Ogre.h>
#include "PlayGround.h"
#include "PhysicsEngine.h"
#include "Player.h"
#include "Target.h"
#include <iostream>
#include <btBulletDynamicsCommon.h>

enum BallCollisionEvent {
	NOTHING_HAPPENED,
	HIT_TARGET_1, HIT_TARGET_2, HIT_TARGET_3,
	HIT_WALL, HIT_FLOOR, HIT_CEILING, HIT_PLAYER, HIT_OPPONENT
};

class Ball {
private:
	BallCollisionEvent collidesWithHelper(BallCollisionEvent result);
protected:
	const static Ogre::Real BALL_RADIUS = 5.0f;
	Ogre::SceneManager* graphicsEngine;
	Ogre::SceneNode* parentNode;
	Ogre::Entity* entBall;
	PhysicsEngine* physicsEngine;
	PhysicsObject physicsObject;
	bool colliding;
	bool shooting;
public:
	Ball(Ogre::SceneManager* mSceneMgr, PhysicsEngine& physicsEngine, PlayGround* box, const Ogre::Vector3& pos=Ogre::Vector3::ZERO);
	~Ball(void);
	Ogre::SceneNode* getNode() { return parentNode; }
	PhysicsObject& getPhysicsObject(void) { return physicsObject; }
	void updateGraphicsScene(void);
	bool hitBy(Player* player);
	BallCollisionEvent hitTarget(Target* t1, Target* t2, Target* t3);
	BallCollisionEvent collidesWith(PlayGround* court, Player* p);
};
#endif // #ifndef __Ball_h_
