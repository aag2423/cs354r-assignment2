
#ifndef __Assignment2_h_
#define __Assignment2_h_

#include "BaseApplication.h"
#include "Game.h"
#include "Sound.h"

#include <CEGUI.h>
#include <CEGUI/RendererModules/Ogre/CEGUIOgreRenderer.h>
#include <stdio.h>

class Assignment2 : public BaseApplication
{
public:
    Assignment2(void);
    virtual ~Assignment2(void);
protected:
	const static Ogre::Real BOX_SIDE_LENGTH = 200.0f;

	Game* game;

	bool mouseClicked;
	bool mPaused;
	Ogre::Real mRotate;       
    	Ogre::Real mMove;
    	Ogre::Vector3 mDirection;
    	Ogre::SceneNode *mCamNode;

	// not used for now
	//OgreBites::ParamsPanel* scorePanel; 
	const static int MAX_NUM_BALLS = 10;
	bool pauses [MAX_NUM_BALLS];
	int bounces;
	int collisions;
	
	CEGUI::OgreRenderer* mRenderer;	

	void setupCEGUI(void);
	bool quit(const CEGUI::EventArgs &e);
	bool resume_game(const CEGUI::EventArgs &e);
	bool configure_game(const CEGUI::EventArgs &e);
	bool config_return(const CEGUI::EventArgs &e);
	bool config_setVolume(const CEGUI::EventArgs &e);
	bool config_setSFXVolume(const CEGUI::EventArgs &e);
	bool config_setRestitution(const CEGUI::EventArgs &e);
	bool config_mode_game(const CEGUI::EventArgs &e);
	bool config_gravity(const CEGUI::EventArgs &e);
	
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
