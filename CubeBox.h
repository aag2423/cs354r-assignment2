
#ifndef __PlayGround_h_
#define __PlayGround_h_

#include <Ogre.h>

class PlayGround {
protected:
	Ogre::SceneNode* parentNode;
	Ogre::Real size;
	void makePlane(Ogre::SceneManager* mSceneMgr, String name, const Ogre::Vector3& planeNormal, const Ogre::Vector3& textureUp, const Ogre::String& texture, const int distance, const int lenght, const int height);
public:
	PlayGround(Ogre::SceneManager* mSceneMgr, Ogre::Real size, Ogre::Real x, Ogre::Real y, Ogre::Real z);
	~PlayGround(void);
	Ogre::Real getHalfSideLength(void) { return halfSideLength; }
	Ogre::SceneNode* getNode(void) { return parentNode; }
};
#endif // #ifndef __PlayGround_h_
