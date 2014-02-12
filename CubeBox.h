/*
-----------------------------------------------------------------------------
Filename:    CubeBox.h
-----------------------------------------------------------------------------
Jiawei Guo, jg44347
-----------------------------------------------------------------------------
*/

#ifndef __CubeBox_h_
#define __CubeBox_h_

#include <Ogre.h>

class CubeBox {
protected:
	Ogre::SceneNode* parentNode;
	Ogre::Real halfSideLength;
	void makePlane(Ogre::SceneManager* mSceneMgr, const Ogre::Vector3& planeNormal, const Ogre::Vector3& textureUp, const Ogre::String& texture);
public:
	CubeBox(Ogre::SceneManager* mSceneMgr, Ogre::Real sideLength, Ogre::Real x, Ogre::Real y, Ogre::Real z);
	~CubeBox(void);
	Ogre::Real getHalfSideLength(void) { return halfSideLength; }
	Ogre::SceneNode* getNode(void) { return parentNode; }
};
#endif // #ifndef __CubeBox_h_
