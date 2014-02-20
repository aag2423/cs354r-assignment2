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
	int score;

	Ogre::Real length;
	Ogre::Real height;
	Ogre::Real width;
public:
	Target(const Ogre::String& name, Ogre::SceneManager* mSceneMgr, PhysicsEngine& physicsEngine, PlayGround* box, const Ogre::Vector3& pos, const Ogre::String& texture, Ogre::Real x = 0.8, Ogre::Real y = 0.8, Ogre::Real z = 0.05);
	~Target(void);
	Ogre::SceneNode* getNode(void) { return parentNode; }
	PhysicsObject& getPhysicsObject(void) { return physicsObject; }
	void resetScore(void) { score = 0; };
	int getScore(void) { return score; }
	bool handleHit(int bonus) {
		bool hit = false;
		if(start == 0) {
			score += bonus + 1;
			hit = true;
		}
		start = std::clock();
		obj->setMaterialName("Examples/Target_Hit"); 
		return hit;
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
