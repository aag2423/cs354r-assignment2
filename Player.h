#ifndef __Player_h_
#define __Player_h_


#include <Ogre.h>
#include "PlayGround.h"
#include "PhysicsEngine.h"

class Player {
protected:
	Ogre::SceneNode* parentNode;
	PhysicsObject physicsObject;
	PlayGround* court;
public:
	const static Ogre::Real HEIGHT = 70.0f;
	Player(Ogre::SceneManager* mSceneMgr, PhysicsEngine& physicsEngine, PlayGround* box, const Ogre::Vector3& pos=Ogre::Vector3::ZERO);
	~Player(void);
	void move(const Ogre::Vector3& direction) ;
	void setPosition(const Ogre::Vector3& pos);
	Ogre::SceneNode* getNode(void) { return parentNode; }
	PhysicsObject& getPhysicsObject(void) { return physicsObject; }
};
#endif // #ifndef __Player_h_
