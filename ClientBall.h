
#ifndef __ClientBall_h_
#define __ClientBall_h_

#include "Ball.h"


class ClientBall {
protected:
	const static Ogre::Real BALL_RADIUS = 5.0f;
	Ogre::SceneManager* graphicsEngine;
	Ogre::SceneNode* parentNode;
	Ogre::Entity* entBall;
public:
	ClientBall(Ogre::SceneManager* mSceneMgr, const Ogre::Vector3& pos=Ogre::Vector3::ZERO);
	~ClientBall(void);
	Ogre::SceneNode* getSceneNode() { return parentNode; }
	void setPosition(const Ogre::Vector3& pos) { parentNode->setPosition(pos);}
	void setOrientation(const Ogre::Quaternion& q) { parentNode->setOrientation(q);}
};
#endif // #ifndef __ClientBall_h_
