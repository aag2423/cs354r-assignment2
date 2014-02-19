#ifndef __Target_h_
#define __Target_h_

#include <Ogre.h>
#include "PlayGround.h"
#include "PhysicsEngine.h"

class Target {
protected:
	Ogre::SceneNode* parentNode;
	PlayGround* court;
	PhysicsObject physicsObject;

	Ogre::Real length;
	Ogre::Real height;
	Ogre::Real width;
public:
	Target(const Ogre::String& name, Ogre::SceneManager* mSceneMgr, PhysicsEngine& physicsEngine, PlayGround* box, const Ogre::Vector3& pos, const Ogre::String& texture, Ogre::Real x = 0.8, Ogre::Real y = 0.8, Ogre::Real z = 0.05);
	~Target(void);
	Ogre::SceneNode* getNode(void) { return parentNode; }
};
#endif // #ifndef __Target_h_