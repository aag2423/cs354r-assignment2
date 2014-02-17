
#ifndef __Assignment2_h_
#define __Assignment2_h_

#include <btBulletDynamicsCommon.h>
#include "BaseApplication.h"
#include "Ball.h"
#include "PlayGround.h"
#include "PhysicsEngine.h"

class Assignment2 : public BaseApplication
{
public:
    Assignment2(void);
    virtual ~Assignment2(void);
protected:
	const static Ogre::Real BOX_SIDE_LENGTH = 200.0f;

	PlayGround *box;
	Ball* ball;

	bool mouseClicked;
	Ogre::Real mRotate;       
    	Ogre::Real mMove;
    	Ogre::Vector3 mDirection;
    	Ogre::SceneNode *mCamNode;
	PhysicsEngine physicsEngine;

	// not used for now
	OgreBites::ParamsPanel* scorePanel; 
	const static int MAX_NUM_BALLS = 10;
	bool pauses [MAX_NUM_BALLS];
	bool mPaused;
	int bounces;
	int collisions;
	
	

	void updatePanel(void);
	void moveBall(const Ogre::FrameEvent &evt);
	virtual void createCamera(void);
    	virtual void createScene(void);
	
	virtual void createFrameListener(void);
    	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
    	virtual bool keyPressed( const OIS::KeyEvent& evt );
	virtual bool keyReleased( const OIS::KeyEvent& evt );
	virtual bool mouseMoved( const OIS::MouseEvent& evt );
	virtual bool mousePressed( const OIS::MouseEvent& evt, OIS::MouseButtonID id );
	virtual bool mouseReleased( const OIS::MouseEvent& evt, OIS::MouseButtonID id );
};

#endif // #ifndef __Assignment2_h_
