#ifndef __Target_h_
#define __Target_h_

#include <Ogre.h>
#include <ctime>
#include "PlayGround.h"
#include "PhysicsEngine.h"

class Target {
protected:
	Ogre::SceneNode* parentNode;
	Ogre::Entity* obj; 
	PlayGround* court;
	PhysicsObject physicsObject;
	clock_t start;

	Ogre::Real length;
	Ogre::Real height;
	Ogre::Real width;
public:
	Target(const Ogre::String& name, Ogre::SceneManager* mSceneMgr, PhysicsEngine& physicsEngine, PlayGround* box, const Ogre::Vector3& pos, const Ogre::String& texture, Ogre::Real x = 0.8, Ogre::Real y = 0.8, Ogre::Real z = 0.05);
	~Target(void);
	Ogre::SceneNode* getNode(void) { return parentNode; }
	PhysicsObject& getPhysicsObject(void) { return physicsObject; }
	void setHitTexture(void) {
		start = std::clock();
		obj->setMaterialName("Examples/TennisBall"); 
	}
	void setNormalTexture(void) {
		if (start == 0)  return;
		double diff = ( std::clock() - start ) / (double)CLOCKS_PER_SEC;
		if (diff > 0.5) {
			obj->setMaterialName("Examples/Target");
			start = 0;
		}
	}
};
#endif // #ifndef __Target_h_
