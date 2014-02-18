
#ifndef __PlayGround_h_
#define __PlayGround_h_

#include <Ogre.h>
#include "PhysicsEngine.h"

class PlayGround {
private:
	void makePlane(Ogre::SceneManager* mSceneMgr, std::string name, const Ogre::Vector3& planeNormal, const Ogre::Vector3& textureUp, const Ogre::String& texture, const int distance, const int lenght, const int height);
protected:
	Ogre::SceneNode* parentNode;
	Ogre::Real halfLength;
	Ogre::Real halfWidth;
	Ogre::Real halfHeight;
	PhysicsObject bottomPlane;
	PhysicsObject topPlane;
	PhysicsObject leftPlane;
	PhysicsObject rightPlane;
	PhysicsObject frontPlane;
	PhysicsObject backPlane;
public:
	PlayGround(Ogre::SceneManager* mSceneMgr, PhysicsEngine& physicsEngine, Ogre::Real l, Ogre::Real w, Ogre::Real h);
	~PlayGround(void);
	Ogre::SceneNode* getNode(void) { return parentNode; }
	void getHalfDimension(Ogre::Vector3& result) { 
		result.y = halfWidth;
		result.z = halfHeight;
		result.x = halfLength;
	}
	void getHalfMovaRange(Ogre::Vector3& result) {
		result.x = halfWidth - 10;
		result.y = halfHeight - 20; 
		result.z = halfLength - 10; 
	}
};
#endif // #ifndef __PlayGround_h_
