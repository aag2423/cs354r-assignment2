#ifndef __ClientPlayer_h_
#define __ClientPlayer_h_


#include "Player.h"

class ClientPlayer {
protected:
	Ogre::SceneManager* graphicsEngine;
	Ogre::Entity* playerEnt;
	Ogre::SceneNode* parentNode;
	Ogre::SceneNode* playerNode;
	Ogre::SceneNode* paddleNode;
	Ogre::AnimationState* walkAnimation;
	Ogre::AnimationState* shootAnimation;
	bool visible;
	clock_t start;
	bool playShootAnimation;
public:
	const static Ogre::Real HEIGHT = 70.0f;
	ClientPlayer(Ogre::SceneManager* mSceneMgr, enum PlayerType type, const Ogre::Vector3& pos=Ogre::Vector3::ZERO);
	~ClientPlayer(void);
	bool isVisible(void) { return visible; }
	Ogre::SceneNode* getSceneNode(void) { return parentNode; }
	void toggleVisible(void);
	void move(const Ogre::Vector3& pos, const Ogre::Quaternion& q, bool isMoving, bool isShotting);
	void setPosition(const Ogre::Vector3& pos) { parentNode->setPosition(pos); }
	void setOrientation(const Ogre::Quaternion& q) { parentNode->setOrientation(q); }
};
#endif // #ifndef __ClientPlayer_h_
