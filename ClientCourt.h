
#ifndef __ClientCourt_h_
#define __ClientCourt_h_

#include <Ogre.h>
#include "PlayGround.h"

class ClientCourt {
private:
	void makePlane(std::string name, const Ogre::Vector3& planeNormal, const Ogre::Vector3& textureUp, const Ogre::String& texture, const int distance, const int lenght, const int height, Ogre::SceneNode* parent=NULL);
	void setup(void);
	void destroy(void);
protected:
	PlayGroundType courtType;
	Ogre::SceneManager* graphicsEngine;
	Ogre::SceneNode* parentNode;
	Ogre::Real l;
	Ogre::Real w;
	Ogre::Real h;
	int courtNum;
public:
	ClientCourt(Ogre::SceneManager* mSceneMgr, PlayGroundType type);
	~ClientCourt(void);
	void toggleType(void);
	void toggleCourt(const int court);
};
#endif // #ifndef __ClientCourt_h_
