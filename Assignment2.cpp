
#include "Assignment2.h"
#include <OgreMath.h>
#include <OgreSphere.h>
#include <iostream>
#include <stdio.h>
//-------------------------------------------------------------------------------------
Assignment2::Assignment2(void) : 
	game(NULL),
	mPaused(true),
	mouseClicked(false),
	bounces(0),
	collisions(0),
	mRotate(0.15),
	mMove(250),
	mDirection(Ogre::Vector3::ZERO)
{
}
//-------------------------------------------------------------------------------------
Assignment2::~Assignment2(void)
{
	delete game;
	delete soundHandler;
	std::cout << "========= Debug: Assignment2 Deleted =========" << std::endl;
}
//------------------------------------------------------------------------------------
CEGUI::MouseButton convertButton(OIS::MouseButtonID buttonID)
{
    switch (buttonID)
    {
    case OIS::MB_Left:
        return CEGUI::LeftButton;

    case OIS::MB_Right:
        return CEGUI::RightButton;

    case OIS::MB_Middle:
        return CEGUI::MiddleButton;

    default:
        return CEGUI::LeftButton;
    }
}

//-------------------------------------------------------------------------------------
void Assignment2::setupCEGUI(void) {
	mRenderer = &CEGUI::OgreRenderer::bootstrapSystem();
	CEGUI::Imageset::setDefaultResourceGroup("Imagesets");
	CEGUI::Font::setDefaultResourceGroup("Fonts");
	CEGUI::Scheme::setDefaultResourceGroup("Schemes");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
	CEGUI::WindowManager::setDefaultResourceGroup("Layouts");

	CEGUI::SchemeManager::getSingleton().create("TaharezLook.scheme");
	CEGUI::System::getSingleton().setDefaultMouseCursor("TaharezLook", "MouseArrow");
	// CEGUI::MouseCursor::getSingleton().hide();

	// GUI imageset(s)
	CEGUI::Imageset *PauseImageset = &CEGUI::ImagesetManager::getSingleton().createFromImageFile("Background", "black_bg.jpg");

	// GUI layout
	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window *sheet = wmgr.createWindow("DefaultWindow", "Game/Sheet");
	CEGUI::System::getSingleton().setGUISheet(sheet);

	CEGUI::Window *titleRoot = wmgr.loadWindowLayout("GameTitle.layout");
	sheet->addChildWindow(titleRoot);

	CEGUI::Window *scoreRoot = wmgr.loadWindowLayout("GameScore.layout");
	sheet->addChildWindow(scoreRoot);

	CEGUI::Window *pauseRoot = wmgr.loadWindowLayout("GamePause.layout");
	sheet->addChildWindow(pauseRoot);

	CEGUI::Window *configRoot = wmgr.loadWindowLayout("GameConfig.layout");
	sheet->addChildWindow(configRoot);

	CEGUI::Window *endRoot = wmgr.loadWindowLayout("GameEnd.layout");
	sheet->addChildWindow(endRoot);

	// set controls text
	std::string controlText ("");
	controlText += " \n- SPACE:   Pause/Unpause game\n \n";
	controlText += "- WASD:    Move player\n \n";
	controlText += "- UIOJKL:  Move camera\n \n";
	controlText += "- Left Mouse Button:   Hit ball\n \n";
	controlText += "- 1:   Weak hit\n \n";
	controlText += "- 2:   Normal hit\n \n";
	controlText += "- 3:   Strong hit\n \n";
	controlText += "- C:   Toggle camera type\n \n";
	controlText += "- R:   Reset game\n \n";
	controlText += "- ESCAPE:  Quit game";
	wmgr.getWindow("PauseRoot/ControlScreen/Text")->setText(controlText);

	// animation
	CEGUI::AnimationManager::getSingleton().loadAnimationsFromXML("example.xml");
	CEGUI::Animation* anim = CEGUI::AnimationManager::getSingleton().getAnimation("TestA");
	CEGUI::Animation* anim2 = CEGUI::AnimationManager::getSingleton().getAnimation("TestB");
	CEGUI::AnimationInstance* anmtn = CEGUI::AnimationManager::getSingleton().instantiateAnimation(anim);
	CEGUI::AnimationInstance* anmtn2 = CEGUI::AnimationManager::getSingleton().instantiateAnimation(anim2);
	anmtn->setTargetWindow(wmgr.getWindow("TitleRoot/Menu/SinglePlayer"));
	anmtn2->setTargetWindow(wmgr.getWindow("TitleRoot/Menu/SinglePlayer"));
	anmtn = CEGUI::AnimationManager::getSingleton().instantiateAnimation(anim);
	anmtn2 = CEGUI::AnimationManager::getSingleton().instantiateAnimation(anim2);
	anmtn->setTargetWindow(wmgr.getWindow("TitleRoot/Menu/Multiplayer"));
	anmtn2->setTargetWindow(wmgr.getWindow("TitleRoot/Menu/Multiplayer"));
	anmtn = CEGUI::AnimationManager::getSingleton().instantiateAnimation(anim);
	anmtn2 = CEGUI::AnimationManager::getSingleton().instantiateAnimation(anim2);
	anmtn->setTargetWindow(wmgr.getWindow("PauseRoot/Menu/Resume"));
	anmtn2->setTargetWindow(wmgr.getWindow("PauseRoot/Menu/Resume"));
	anmtn = CEGUI::AnimationManager::getSingleton().instantiateAnimation(anim);
	anmtn2 = CEGUI::AnimationManager::getSingleton().instantiateAnimation(anim2);
	anmtn->setTargetWindow(wmgr.getWindow("PauseRoot/Menu/Quit"));
	anmtn2->setTargetWindow(wmgr.getWindow("PauseRoot/Menu/Quit"));
	anmtn = CEGUI::AnimationManager::getSingleton().instantiateAnimation(anim);
	anmtn2 = CEGUI::AnimationManager::getSingleton().instantiateAnimation(anim2);
	anmtn->setTargetWindow(wmgr.getWindow("PauseRoot/Menu/Controls"));
	anmtn2->setTargetWindow(wmgr.getWindow("PauseRoot/Menu/Controls"));
	anmtn = CEGUI::AnimationManager::getSingleton().instantiateAnimation(anim);
	anmtn2 = CEGUI::AnimationManager::getSingleton().instantiateAnimation(anim2);
	anmtn->setTargetWindow(wmgr.getWindow("PauseRoot/Menu/Config"));
	anmtn2->setTargetWindow(wmgr.getWindow("PauseRoot/Menu/Config"));

	// subscribe events
	wmgr.getWindow("TitleRoot/Menu/SinglePlayer")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Assignment2::title_start_sp_game, this));
	wmgr.getWindow("TitleRoot/Menu/Multiplayer")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Assignment2::title_mp_menu, this));
	wmgr.getWindow("TitleRoot/MP/Host")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Assignment2::title_host_game, this));
	wmgr.getWindow("TitleRoot/MP/Connect")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Assignment2::title_connect_to_game, this));
	wmgr.getWindow("PauseRoot/Menu/Resume")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Assignment2::resume_game, this));
	wmgr.getWindow("PauseRoot/Menu/Quit")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Assignment2::quit, this));
	wmgr.getWindow("PauseRoot/Menu/Controls")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Assignment2::game_controls, this));
	wmgr.getWindow("PauseRoot/ControlScreen/Return")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Assignment2::game_controls_return, this));
	wmgr.getWindow("PauseRoot/Menu/Config")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Assignment2::configure_game, this));
	wmgr.getWindow("ConfigRoot/Menu/Return")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Assignment2::config_return, this));
	wmgr.getWindow("ConfigRoot/Menu/VolumeScrollbar")->subscribeEvent(CEGUI::Scrollbar::EventScrollPositionChanged, CEGUI::Event::Subscriber(&Assignment2::config_setVolume, this));
	wmgr.getWindow("ConfigRoot/Menu/SFXScrollbar")->subscribeEvent(CEGUI::Scrollbar::EventScrollPositionChanged, CEGUI::Event::Subscriber(&Assignment2::config_setSFXVolume, this));
	wmgr.getWindow("ConfigRoot/Menu/RestitutionScrollbar")->subscribeEvent(CEGUI::Scrollbar::EventScrollPositionChanged, CEGUI::Event::Subscriber(&Assignment2::config_setRestitution, this));
	wmgr.getWindow("ConfigRoot/Menu/Court1")->subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged, CEGUI::Event::Subscriber(&Assignment2::config_court, this));
	wmgr.getWindow("ConfigRoot/Menu/Court2")->subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged, CEGUI::Event::Subscriber(&Assignment2::config_court, this));
	wmgr.getWindow("ConfigRoot/Menu/Court3")->subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged, CEGUI::Event::Subscriber(&Assignment2::config_court, this));
	wmgr.getWindow("ConfigRoot/Menu/ModeGame")->subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged, CEGUI::Event::Subscriber(&Assignment2::config_mode_game, this));
	wmgr.getWindow("ConfigRoot/Menu/GravityEarth")->subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged, CEGUI::Event::Subscriber(&Assignment2::config_gravity, this));
}

//-------------------------------------------------------------------------------------

void Assignment2::createCamera(void)
{
	mCamera = mSceneMgr->createCamera("MainCam");
	mCamera->lookAt(Ogre::Vector3(0,0,0));
	mCamera->setNearClipDistance(10);
	mCamNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("MainCamNode", Ogre::Vector3(0,100,300));
        mCamNode->attachObject(mCamera);
        //mCamNode->pitch(Ogre::Degree(360));
}
//-------------------------------------------------------------------------------------

void Assignment2::createScene(void)
{
	setupCEGUI();

	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
	mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

	appMode = SINGLE_PLAYER;
	//game = new Game(mSceneMgr, mCamNode);
	//game->handleKeyboardEvent(PAUSE);

	sGame= new ServerGame(mSceneMgr);
	cGame= new ClientGame(mSceneMgr, mCamNode, sGame->getInitializationData(SIDE_NEAR), sGame->send(SIDE_NEAR));

	mPaused =true;

	Ogre::Light* spotLight1 = mSceneMgr->createLight("spotLight1");
    	spotLight1->setType(Ogre::Light::LT_SPOTLIGHT);
	spotLight1->setDiffuseColour(Ogre::ColourValue::Blue);
	spotLight1->setSpecularColour(Ogre::ColourValue::Blue);
	spotLight1->setDirection(1, -1, -1);
	spotLight1->setPosition(Ogre::Vector3(-80, 80, -80));
	spotLight1->setSpotlightRange(Ogre::Degree(10), Ogre::Degree(20));
	
	Ogre::Light* spotLight2 = mSceneMgr->createLight("spotLight2");
    	spotLight2->setType(Ogre::Light::LT_SPOTLIGHT);
	spotLight2->setDiffuseColour(Ogre::ColourValue::White);
	spotLight2->setSpecularColour(Ogre::ColourValue::White);
	spotLight2->setDirection(-1, -1, -1);
	spotLight2->setPosition(Ogre::Vector3(80, 80, -80));
	spotLight2->setSpotlightRange(Ogre::Degree(10), Ogre::Degree(20));

	Ogre::Light* pointLight = mSceneMgr->createLight("pointLight");
	pointLight->setType(Ogre::Light::LT_POINT);
	pointLight->setPosition(Ogre::Vector3(0, 80, -80));
	pointLight->setDiffuseColour(0.8, 0.8, 0.8);
	pointLight->setSpecularColour(0.8, 0.8, 0.8);
	
	soundHandler = new Sound();
	soundHandler->start_ambient();
}
//-------------------------------------------------------------------------------------
void Assignment2::createFrameListener(void) {
	//BaseApplication::createFrameListener();    
	Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
	OIS::ParamList pl;
	size_t windowHnd = 0;
	std::ostringstream windowHndStr;

	mWindow->getCustomAttribute("WINDOW", &windowHnd);
	windowHndStr << windowHnd;
	pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

	mInputManager = OIS::InputManager::createInputSystem( pl );

	mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, true ));
	mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject( OIS::OISMouse, true ));

	mMouse->setEventCallback(this);
	mKeyboard->setEventCallback(this);

	//Set initial mouse clipping size
	windowResized(mWindow);

	//Register as a Window listener
	Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);

	mRoot->addFrameListener(this);

}
//-------------------------------------------------------------------------------------
bool Assignment2::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	static char score_buf[5];
	static int hit;
	static CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
	if (mWindow->isClosed()) return false;
	if (mShutDown) return false;
	mKeyboard->capture();
	mMouse->capture();
	CEGUI::System::getSingleton().injectTimePulse(evt.timeSinceLastFrame);
	mCamNode->translate(mDirection * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
	//game->runNextFrame(evt);


	if(mPaused) return true;

        if (appMode == MULTI_PLAYER_SERVER) {
		sGame->receive(cGame->send());
		InputState input;
		if(conn->receiveInputState(&input))
			sGame->receive(input);

		sGame->runNextFrame();
		conn->sendOutputState(sGame->send(false));
		cGame->receive(sGame->send(true));
		cGame->runNextFrame();
	} else if (appMode == MULTI_PLAYER_CLIENT){

		OutputState serverState;
		if(conn->receiveOutputState(&serverState))
			cGame->receive(serverState);
		cGame->runNextFrame();
		conn->sendInputState(cGame->send());
	} else {
		sGame->receive(cGame->send());
		sGame->runNextFrame();
		cGame->receive(sGame->send(true));
		cGame->runNextFrame();
	}


	switch(cGame->getPlayerHitStrength()) {
		case WEAK_HIT:
			wmgr.getWindow("StrengthRoot")->setText("WEAK");
			break;
		case NORMAL_HIT:
			CEGUI::WindowManager::getSingleton().getWindow("StrengthRoot")->setText("NORMAL");
			break;
		case STRONG_HIT:
			CEGUI::WindowManager::getSingleton().getWindow("StrengthRoot")->setText("STRONG");
			break;
		default: break;
	}
/*
	switch(game->getGameResult()) {
		case WIN:
			wmgr.getWindow("EndGameRoot")->setVisible(true);
			wmgr.getWindow("EndGameRoot/Display")->setText("You Win!");
			break;
		case LOSE:
			wmgr.getWindow("EndGameRoot")->setVisible(true);
			wmgr.getWindow("EndGameRoot/Display")->setText("You Lose!");
			break;
		default:
			wmgr.getWindow("EndGameRoot")->setVisible(false);
			break;
	}
	if(game->getGameMode() == PRACTICE) {
		sprintf(score_buf, "%d", game->getScore());
		CEGUI::WindowManager::getSingleton().getWindow("PracticeScoreRoot/ScoreVal")->setText(score_buf);
		sprintf(score_buf, "%d", game->getComboBonus());
		CEGUI::WindowManager::getSingleton().getWindow("PracticeScoreRoot/ComboVal")->setText(score_buf);
	} else {
		sprintf(score_buf, "%d", game->getScore());
		CEGUI::WindowManager::getSingleton().getWindow("VsScoreRoot/PlayerScoreVal")->setText(score_buf);
		sprintf(score_buf, "%d", game->getOpponentScore());
		CEGUI::WindowManager::getSingleton().getWindow("VsScoreRoot/CPUScoreVal")->setText(score_buf);
	}
*/
	sprintf(score_buf, "%d", cGame->getPlayerScore());
	CEGUI::WindowManager::getSingleton().getWindow("VsScoreRoot/PlayerScoreVal")->setText(score_buf);
	sprintf(score_buf, "%d", cGame->getOpponentScore());
	CEGUI::WindowManager::getSingleton().getWindow("VsScoreRoot/CPUScoreVal")->setText(score_buf);

	
        return true;
}
//-------------------------------------------------------------------------------------
// OIS::KeyListener
bool Assignment2::keyPressed( const OIS::KeyEvent& evt ){
	// pass event to CEGUI
	CEGUI::System &sys = CEGUI::System::getSingleton();
	sys.injectKeyDown(evt.key);
	sys.injectChar(evt.text);

	switch (evt.key) {
		case OIS::KC_ESCAPE: 
			mShutDown = true;
			break;
		case OIS::KC_SPACE: 
			mPaused = !mPaused;
			game->handleKeyboardEvent(PAUSE);
			static CEGUI::Window* pause_screen = CEGUI::WindowManager::getSingleton().getWindow("PauseRoot");
			pause_screen->setVisible(mPaused);
			CEGUI::WindowManager::getSingleton().getWindow("PauseRoot/Menu")->setVisible(true);
			CEGUI::WindowManager::getSingleton().getWindow("ConfigRoot")->setVisible(false);
			CEGUI::WindowManager::getSingleton().getWindow("PauseRoot/ControlScreen")->setVisible(false);
			if(mPaused)
				CEGUI::MouseCursor::getSingleton().show();
			else
				CEGUI::MouseCursor::getSingleton().hide();
			break;
		case OIS::KC_R:
			if (appMode == SINGLE_PLAYER) sGame->restart();
    			break;
		case OIS::KC_J:
    			mDirection.x = -mMove;
    			break;
		case OIS::KC_L:
    			mDirection.x = mMove;
    			break;
		case OIS::KC_U:
    			mDirection.y = mMove;
    			break;
		case OIS::KC_O:
    			mDirection.y = -mMove;
    			break;
		case OIS::KC_I:
    			mDirection.z = -mMove;
    			break;
		case OIS::KC_K:
    			mDirection.z = mMove;
    			break;
		case OIS::KC_W:
			cGame->handleKeyboardEvent(GO_FORWARD);
			//game->handleKeyboardEvent(GO_FORWARD);
    			break;
		case OIS::KC_S:
			cGame->handleKeyboardEvent(GO_BACKWARD);
			//game->handleKeyboardEvent(GO_BACKWARD);
    			break;
		case OIS::KC_A:
			cGame->handleKeyboardEvent(GO_LEFT);
			//game->handleKeyboardEvent(GO_LEFT);
    			break;
		case OIS::KC_D:
			cGame->handleKeyboardEvent(GO_RIGHT);
			//game->handleKeyboardEvent(GO_RIGHT);
    			break;
		case OIS::KC_LCONTROL:
			cGame->handleKeyboardEvent(RUN);
			//game->handleKeyboardEvent(RUN);
    			break;
		case OIS::KC_C:
			cGame->handleKeyboardEvent(TOGGLE_CAMERA);
			//game->handleKeyboardEvent(TOGGLE_CAMERA);
    			break;
		case OIS::KC_1:
			cGame->handleKeyboardEvent(USE_WEAK_HIT);
    			break;
		case OIS::KC_2:
			cGame->handleKeyboardEvent(USE_NORMAL_HIT);
    			break;
		case OIS::KC_3:
			cGame->handleKeyboardEvent(USE_STRONG_HIT);
    			break;
		default:
			break;
	}
	return true;
}
//-------------------------------------------------------------------------------------
bool Assignment2::keyReleased( const OIS::KeyEvent& evt ){
	CEGUI::System::getSingleton().injectKeyUp(evt.key);

	switch (evt.key) {
		case OIS::KC_LCONTROL:
			cGame->handleKeyboardEvent(STOP_RUN);
			//game->handleKeyboardEvent(STOP_RUN);
    			break;
		case OIS::KC_J:
		case OIS::KC_L:
    			mDirection.x = 0;
    			break;
		case OIS::KC_U:
		case OIS::KC_O:
    			mDirection.y = 0;
    			break;
		case OIS::KC_I:
		case OIS::KC_K:
    			mDirection.z = 0;
    			break;
		case OIS::KC_W:
			cGame->handleKeyboardEvent(STOP_FORWARD);
			//game->handleKeyboardEvent(STOP_FORWARD);
    			break;
		case OIS::KC_S:
			cGame->handleKeyboardEvent(STOP_BACKWARD);
			//game->handleKeyboardEvent(STOP_BACKWARD);
    			break;
		case OIS::KC_A:
			cGame->handleKeyboardEvent(STOP_LEFT);
			//game->handleKeyboardEvent(STOP_LEFT);
    			break;
		case OIS::KC_D:
			cGame->handleKeyboardEvent(STOP_RIGHT);
			//game->handleKeyboardEvent(STOP_RIGHT);
    			break;
		default:
			break;
	}
	return true;
}
//-------------------------------------------------------------------------------------

// OIS::MouseListener
bool Assignment2::mouseMoved( const OIS::MouseEvent& evt ){
	CEGUI::System &sys = CEGUI::System::getSingleton();
	sys.injectMouseMove(evt.state.X.rel, evt.state.Y.rel);
	if(evt.state.Z.rel)
		sys.injectMouseWheelChange(evt.state.Z.rel / 120.0f);
	if(!mPaused) {
		//game->handleMouseMove(evt.state.X.rel, evt.state.Y.rel);
		cGame->handleMouseMove(evt.state.X.rel, evt.state.Y.rel);
	}
//	mCamNode->yaw(Ogre::Degree(-mRotate * evt.state.X.rel), Ogre::Node::TS_WORLD);
//    	mCamNode->pitch(Ogre::Degree(-mRotate * evt.state.Y.rel), Ogre::Node::TS_LOCAL);
	return true;
}
//-------------------------------------------------------------------------------------
bool Assignment2::mousePressed( const OIS::MouseEvent& evt, OIS::MouseButtonID id ){
	CEGUI::System::getSingleton().injectMouseButtonDown(convertButton(id));
	if(mPaused)
		return true;
	switch (id)
	{
	case OIS::MB_Left:
		cGame->handleMouseClick(HIT_START);
		//game->handleMouseClick(HIT_START);
		break;
	default:
	    break;
	}
	return true;
}
//-------------------------------------------------------------------------------------
bool Assignment2::mouseReleased( const OIS::MouseEvent& evt, OIS::MouseButtonID id ){
	CEGUI::System::getSingleton().injectMouseButtonUp(convertButton(id));
	if(mPaused)
		return true;
	switch (id)
	{
	case OIS::MB_Left:
		cGame->handleMouseClick(HIT_STOP);
		//game->handleMouseClick(HIT_STOP);
		break;
	case OIS::MB_Right:
		//ball->getPhysicsObject().toggleRigidBodyAndKinematic(2);
		break;
	default:
	    break;
	}
	return true;
}
//-------------------------------------------------------------------------------------

// CEGUI events
bool Assignment2::title_start_sp_game(const CEGUI::EventArgs &e) {
	//game->handleKeyboardEvent(PAUSE);
	appMode = SINGLE_PLAYER;

	mPaused = false;
	CEGUI::WindowManager::getSingleton().getWindow("TitleRoot")->setVisible(false);
	CEGUI::WindowManager::getSingleton().getWindow("VsScoreRoot")->setVisible(true);
	CEGUI::WindowManager::getSingleton().getWindow("StrengthRoot")->setVisible(true);
	CEGUI::MouseCursor::getSingleton().hide();
    return true;
}

//--------------------------------------------------------------------------------------
bool Assignment2::title_mp_menu(const CEGUI::EventArgs &e) {
	CEGUI::WindowManager::getSingleton().getWindow("TitleRoot/Menu")->setVisible(false);
	CEGUI::WindowManager::getSingleton().getWindow("TitleRoot/MP")->setVisible(true);
    return true;
}

//--------------------------------------------------------------------------------------
bool Assignment2::title_host_game(const CEGUI::EventArgs &e) {
	// Network server setup here
	conn = new Network(true, "", 0);

	appMode = MULTI_PLAYER_SERVER;

	delete sGame;
	delete cGame;
	sGame= new ServerGame(mSceneMgr, appMode);
	cGame= new ClientGame(mSceneMgr, mCamNode, sGame->getInitializationData(SIDE_NEAR), sGame->send(SIDE_NEAR));
	//game->handleKeyboardEvent(PAUSE);
	mPaused = false;
	CEGUI::WindowManager::getSingleton().getWindow("TitleRoot")->setVisible(false);
	CEGUI::WindowManager::getSingleton().getWindow("VsScoreRoot")->setVisible(true);
	CEGUI::WindowManager::getSingleton().getWindow("StrengthRoot")->setVisible(true);
	CEGUI::MouseCursor::getSingleton().hide();
    return true;
}

//--------------------------------------------------------------------------------------
bool Assignment2::title_connect_to_game(const CEGUI::EventArgs &e) {
	CEGUI::String ad = CEGUI::WindowManager::getSingleton().getWindow("TitleRoot/MP/AddressText")->getText();
	CEGUI::String prt = CEGUI::WindowManager::getSingleton().getWindow("TitleRoot/MP/PortText")->getText();
	std::cout << "IP: " << ad << "\tPort: " << prt << std::endl;

	appMode = MULTI_PLAYER_CLIENT;
	mPaused = false;

	const char* addr = ad.c_str();
	int port = atoi(prt.c_str());
	// Network client setup here
	conn = new Network(false, addr, port);

	delete cGame;
	cGame= new ClientGame(mSceneMgr, mCamNode, sGame->getInitializationData(SIDE_FAR), sGame->send(SIDE_FAR));

	//game->handleKeyboardEvent(PAUSE);
	CEGUI::WindowManager::getSingleton().getWindow("TitleRoot")->setVisible(false);
	CEGUI::WindowManager::getSingleton().getWindow("VsScoreRoot")->setVisible(true);
	CEGUI::WindowManager::getSingleton().getWindow("StrengthRoot")->setVisible(true);
	CEGUI::MouseCursor::getSingleton().hide();
    return true;
}

//--------------------------------------------------------------------------------------
bool Assignment2::quit(const CEGUI::EventArgs &e) {
        mShutDown = true;
        return true;
}
//-------------------------------------------------------------------------------------

bool Assignment2::resume_game(const CEGUI::EventArgs &e) {
        mPaused = false;
	//game->handleKeyboardEvent(PAUSE);
        CEGUI::WindowManager::getSingleton().getWindow("PauseRoot")->setVisible(false);
        CEGUI::MouseCursor::getSingleton().hide();
        return true;
}
//-------------------------------------------------------------------------------------

bool Assignment2::game_controls(const CEGUI::EventArgs &e) {
        CEGUI::WindowManager::getSingleton().getWindow("PauseRoot/Menu")->setVisible(false);
        CEGUI::WindowManager::getSingleton().getWindow("PauseRoot/ControlScreen")->setVisible(true);
        return true;
}
//-------------------------------------------------------------------------------------

bool Assignment2::game_controls_return(const CEGUI::EventArgs &e) {
        CEGUI::WindowManager::getSingleton().getWindow("PauseRoot/Menu")->setVisible(true);
        CEGUI::WindowManager::getSingleton().getWindow("PauseRoot/ControlScreen")->setVisible(false);
        return true;
}
//-------------------------------------------------------------------------------------

bool Assignment2::configure_game(const CEGUI::EventArgs &e) {
        CEGUI::WindowManager::getSingleton().getWindow("PauseRoot")->setVisible(false);
        CEGUI::WindowManager::getSingleton().getWindow("ConfigRoot")->setVisible(true);
        return true;
}
//-------------------------------------------------------------------------------------

bool Assignment2::config_return(const CEGUI::EventArgs &e) {
	CEGUI::WindowManager::getSingleton().getWindow("PauseRoot")->setVisible(true);
        CEGUI::WindowManager::getSingleton().getWindow("ConfigRoot")->setVisible(false);
        return true;
}
//-------------------------------------------------------------------------------------

bool Assignment2::config_setVolume(const CEGUI::EventArgs &e) {
	CEGUI::Scrollbar *vol = (CEGUI::Scrollbar*) CEGUI::WindowManager::getSingleton().getWindow("ConfigRoot/Menu/VolumeScrollbar");
	soundHandler->set_ambient_volume(a_channel, (vol->getScrollPosition() * 128));
	return true;
}
//-------------------------------------------------------------------------------------

bool Assignment2::config_setSFXVolume(const CEGUI::EventArgs &e) {
	CEGUI::Scrollbar *vol = (CEGUI::Scrollbar*) CEGUI::WindowManager::getSingleton().getWindow("ConfigRoot/Menu/SFXScrollbar");
	soundHandler->set_effects_volume(vol->getScrollPosition() * 128);
	return true;
}
//-------------------------------------------------------------------------------------

bool Assignment2::config_setRestitution(const CEGUI::EventArgs &e) {	
	if (appMode != SINGLE_PLAYER) return true;
	CEGUI::Scrollbar *vol = (CEGUI::Scrollbar*) CEGUI::WindowManager::getSingleton().getWindow("ConfigRoot/Menu/RestitutionScrollbar");
	sGame->setBallRestitution(vol->getScrollPosition());
	return true;
}
//-------------------------------------------------------------------------------------

bool Assignment2::config_court(const CEGUI::EventArgs &e) {
	static CEGUI::RadioButton *b1 = (CEGUI::RadioButton*) CEGUI::WindowManager::getSingleton().getWindow("ConfigRoot/Menu/Court1");
	static CEGUI::RadioButton *b2 = (CEGUI::RadioButton*) CEGUI::WindowManager::getSingleton().getWindow("ConfigRoot/Menu/Court2");
	if(b1->isSelected()) {
		cGame->getCourt()->toggleCourt(0);
	} else if (b2->isSelected()) {
		cGame->getCourt()->toggleCourt(1);
	} else {
		cGame->getCourt()->toggleCourt(2);
	}
	// Case 0: Tennis Court
	// Case 1: Grid
	// Case 2: Cloudy

	return true;	
}
//--------------------------------------------------------------------------------------

bool Assignment2::config_mode_game(const CEGUI::EventArgs &e) {	
	return true;
	if (appMode == SINGLE_PLAYER) {
		game->handleKeyboardEvent(TOGGLE_GAME_MODE);
		if(game->getGameMode() == PRACTICE) {
			CEGUI::WindowManager::getSingleton().getWindow("VsScoreRoot")->setVisible(false);
			CEGUI::WindowManager::getSingleton().getWindow("PracticeScoreRoot")->setVisible(true);
		} else {
			CEGUI::WindowManager::getSingleton().getWindow("VsScoreRoot")->setVisible(true);
			CEGUI::WindowManager::getSingleton().getWindow("PracticeScoreRoot")->setVisible(false);
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------

bool Assignment2::config_gravity(const CEGUI::EventArgs &e) {
	if (appMode == SINGLE_PLAYER)
		sGame->toggleGravity();
	return true;
}
//--------------------------------------------------------------------------------------
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
    int main(int argc, char *argv[])
#endif
    {
        // Create application object
        Assignment2 app;

        try {
            app.go();
        } catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
            std::cerr << "An exception has occured: " <<
                e.getFullDescription().c_str() << std::endl;
#endif
        }

        return 0;
    }

#ifdef __cplusplus
}
#endif
