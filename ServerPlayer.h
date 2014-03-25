#ifndef __ServerPlayer_h_
#define __ServerPlayer_h_


#include <Player.h>


class ServerPlayer {
protected:
	Ogre::SceneNode* positionNode;
	PhysicsObject physicsObject;
	Ogre::SceneManager* graphicsEngine;
	PhysicsEngine* physicsEngine;
public:
	enum PlayerType playerType;
	enum PlayerSide playerSide;
	InputPlayerState playerState;
	const static Ogre::Real HEIGHT = 70.0f;
	const static Ogre::Real PADDLE_REACH = 90.0f;
	ServerPlayer(Ogre::SceneManager* mSceneMgr, PhysicsEngine& bulletEngine, PlayerType type, const Ogre::Vector3& pos, PlayerSide side, bool turnAround=false);
	~ServerPlayer(void);
	bool move(const Ogre::Vector3& range, bool isFullGame=true) ;
	void setPosition(const Ogre::Vector3& newPos);
	const Ogre::Vector3& getPosition(void) { return positionNode->getPosition();}
	const Ogre::Quaternion& getOrientation(void) { return positionNode->getOrientation();}
	bool canReachBall(const Ogre::Vector3& ballPos);
	PhysicsObject& getPhysicsObject(void) { return physicsObject; }
};
#endif // #ifndef __ServerPlayer_h_
