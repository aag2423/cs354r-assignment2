
#ifndef __ServerBall_h_
#define __ServerBall_h_

#include "Ball.h"
#include "ServerPlayer.h"
#include "ServerCourt.h"

class ServerBall {
private:
	BallCollisionEvent collidesWithHelper(BallCollisionEvent result);
protected:
	const static Ogre::Real BALL_RADIUS = 5.0f;
	PhysicsEngine& physicsEngine;
	PhysicsObject physicsObject;
	bool colliding;
	bool shooting;
public:
	ServerBall(PhysicsEngine& physicsEngine, const Ogre::Vector3& pos=Ogre::Vector3::ZERO);
	~ServerBall(void);
	PhysicsObject getPhysicsObject(void) { return physicsObject; }
	const Ogre::Vector3& getPosition(void);
	const Ogre::Quaternion& getOrientation(void);
	bool hitByPlayer(ServerPlayer* player);
	BallCollisionEvent checkCollision(ServerCourt* court, ServerPlayer* p1, ServerPlayer* p2);
};
#endif // #ifndef __ServerBall_h_
