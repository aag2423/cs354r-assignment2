#include "Game.h"

#include <OgreFrameListener.h>
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <iostream>

Game::Game(Ogre::SceneManager* mSceneMgr, Ogre::SceneNode* camNode) {

	gameState.camMode = ABOVE_CAM;
	gameState.paused = false;
	gameState.gameStarted = false;
	gameState.combo = false;
	gameState.comboBonus = 0;


	physicsEngine.setGravity(0, EARTH_G, 0);

	graphicsEngine = mSceneMgr;
	cameraNode = camNode;

	court = new PlayGround(mSceneMgr, physicsEngine, COURT_LENGTH, COURT_WIDTH, COURT_HEIGHT);
	ball = new Ball(mSceneMgr, physicsEngine, court, Ogre::Vector3(5, -75, COURT_LENGTH/2 - 310));
	player = new Player(mSceneMgr, physicsEngine, HUMAN, court, Ogre::Vector3(0, -125, COURT_LENGTH/2 - 300));
	computer = new Player(mSceneMgr, physicsEngine, AI, court, Ogre::Vector3(0, -125, -COURT_LENGTH/2 + 300));
	computer->getNode()->yaw(Ogre::Degree(180));
	
	target1 = new Target("t1", mSceneMgr, physicsEngine, court, Ogre::Vector3(-150, 0, -COURT_LENGTH/2), "Examples/Target");
	target2 = new Target("t2", mSceneMgr, physicsEngine, court, Ogre::Vector3(0, -50, -COURT_LENGTH/2), "Examples/Target");
	target3 = new Target("t3", mSceneMgr, physicsEngine, court, Ogre::Vector3(150, 0, -COURT_LENGTH/2), "Examples/Target");
	
	toggleCamera();
}

//-------------------------------------------------------------------------------------

Game::~Game(void) {
	delete ball;
	delete court;
	delete player;
	delete computer;
	delete target1;
	delete target2;
	delete target3;
	std::cout << "========= Debug: Game Deleted =========" << std::endl;
}

//-------------------------------------------------------------------------------------

void Game::reset(void) {

	gameState.gameStarted = false;
	physicsEngine.setGravity(0, EARTH_G, 0);
	gameState.combo = false;
	gameState.comboBonus = 0;
	target2->resetScore();
	target3->resetScore();
	target1->resetScore();

	delete ball;
	ball = new Ball(graphicsEngine, physicsEngine, court, Ogre::Vector3(5, -75, COURT_LENGTH/2 - 310));

	player->setPosition(Ogre::Vector3(5, -125, COURT_LENGTH/2 - 300));  
	player->resetState(); 
	computer->setPosition(Ogre::Vector3(5, -125, -COURT_LENGTH/2 + 300));
	computer->getNode()->yaw(Ogre::Degree(180));
	computer->resetState();
}

//-------------------------------------------------------------------------------------

void Game::runAI(void) {
	Ogre::Vector3 distance = ball->getNode()->getPosition() - computer->getNode()->getPosition();
	if (distance.x > 0)  computer->playerState.movingLeft = true;
	else  computer->playerState.movingLeft = false;
	if (distance.x < 0)  computer->playerState.movingRight = true;
	else  computer->playerState.movingRight = false;
}


//-------------------------------------------------------------------------------------

void Game::runNextFrame(const Ogre::FrameEvent& evt) {
	if(gameState.paused) return;
	if (!gameState.gameStarted) return;

	player->move(evt);
	runAI();
	computer->move(evt);
	physicsEngine.stepSimulation(evt.timeSinceLastFrame*5);
	if(ball->hitBy(player))	soundHandler->play_sound(ball_hit);
	if(ball->hitBy(computer)) soundHandler->play_sound(ball_hit);

	BallCollisionEvent be = ball->collidesWith(court, player);
	if (be == HIT_FLOOR) {
		gameState.combo = false;
		gameState.comboBonus = 0;
	}
	if (be == HIT_PLAYER)
		std::cout << "play sound ball hit player"<<std::endl;
	if (be == HIT_FLOOR)
		std::cout << "play sound ball hit floor"<<std::endl;		//soundHandler->play_sound(point_down);
		
	BallCollisionEvent te = ball->hitTarget(target1, target2, target3);

	if(te == HIT_TARGET_1 && target1->handleHit(gameState.comboBonus)) {
		gameState.comboBonus++;
		gameState.combo = true;
		soundHandler->play_sound(point_up);
	}else {
		target1->setNormalTexture();
	}
	if(te == HIT_TARGET_2 && target2->handleHit(gameState.comboBonus)) {
		gameState.comboBonus++;
		gameState.combo = true;
		soundHandler->play_sound(point_up);
	}else {
		target2->setNormalTexture();
	}
	if(te == HIT_TARGET_3 && target3->handleHit(gameState.comboBonus)) {
		gameState.comboBonus++;
		gameState.combo = true;
		soundHandler->play_sound(point_up);
	}else {
		target3->setNormalTexture();
	}
	
	ball->updateGraphicsScene();
}

//-------------------------------------------------------------------------------------

void Game::toggleCamera(void) {
	Ogre::Node* oldCamParent = cameraNode->getParent();
	oldCamParent->removeChild(cameraNode);
	Ogre::SceneNode* newCamParent;
	if(gameState.camMode == ABOVE_CAM) {
		gameState.camMode = THIRD_PERSON_CAM;
		newCamParent = player->getNode();
		newCamParent->addChild(cameraNode);
		cameraNode->setPosition(0, 80, 50);
		cameraNode->resetOrientation();
        	cameraNode->pitch(Ogre::Degree(-10));
	} else if (gameState.camMode == THIRD_PERSON_CAM){
		gameState.camMode = FIRST_PERSON_CAM;
		newCamParent = player->getNode();
		newCamParent->addChild(cameraNode);
		cameraNode->setPosition(0, 60, 0);
		cameraNode->resetOrientation();
        	cameraNode->pitch(Ogre::Degree(-15));
	} else {
		gameState.camMode = ABOVE_CAM;
		newCamParent = graphicsEngine->getRootSceneNode();
		newCamParent->addChild(cameraNode);
		cameraNode->setPosition(0, 50, 700);
		cameraNode->resetOrientation();
		player->getNode()->resetOrientation();
        	cameraNode->pitch(Ogre::Degree(-10));
	}
}

//-------------------------------------------------------------------------------------

void Game::handleKeyboardEvent(enum KeyboardEvent evt) {
	switch (evt) {
	case GO_LEFT:
		player->playerState.movingLeft = true; break;
	case GO_RIGHT:
		player->playerState.movingRight = true; break;
	case GO_FORWARD:
		player->playerState.movingForward = true; break;
	case GO_BACKWARD:
		player->playerState.movingBackward = true; break;
	case STOP_LEFT:
		player->playerState.movingLeft = false; break;
	case STOP_RIGHT:
		player->playerState.movingRight = false; break;
	case STOP_FORWARD:
		player->playerState.movingForward = false; break;
	case STOP_BACKWARD:
		player->playerState.movingBackward = false; break;
	case RUN:
	        player->playerState.step = RUN_STEP; break;
	case STOP_RUN:
	        player->playerState.step = NORMAL_STEP; break;
	case USE_WEAK_HIT:
        	player->playerState.strength = WEAK_HIT; break;
	case USE_NORMAL_HIT:
	        player->playerState.strength = NORMAL_HIT; break;
	case USE_STRONG_HIT:
	        player->playerState.strength = STRONG_HIT; break;
	case TOGGLE_CAMERA:
		toggleCamera(); 
		break;
	case RESTART:
		reset(); break;
	case PAUSE:
		gameState.paused = !gameState.paused; break;
	default: break;
	}
}

void Game::handleMouseMove(Ogre::Real dx, Ogre::Real dy) {
	PlayerState* playerState = &(player->playerState);
	if (playerState->hitting) {
		if (dx > 5 && playerState->shotDirection.x == 0)
			playerState->shotDirection.x = BACKHAND;
		else if (dx < -5 && playerState->shotDirection.x == 0)
                        playerState->shotDirection.x = FOREHAND;
		if (dy > 5 && playerState->shotDirection.y == 0) 
			playerState->shotDirection.y = VOLLEY;
		else if (dy < -5 && playerState->shotDirection.y == 0) 
			playerState->shotDirection.y = SMASH;
	}
	if (gameState.camMode != ABOVE_CAM){
		player->getNode()->yaw(Ogre::Degree(-0.15*dx), Ogre::Node::TS_LOCAL);
	}
	if (gameState.camMode == FIRST_PERSON_CAM){
		cameraNode->pitch(Ogre::Degree(-0.15*dy), Ogre::Node::TS_LOCAL);
	} 
}

void Game::handleMouseClick(enum MouseEvent evt) {
	switch (evt) {
	case HIT_START:
		player->playerState.hitting = true;
		gameState.gameStarted = true;
		break;
	case HIT_STOP:
		player->playerState.hitting = false;
		player->playerState.shotDirection = 0; 
		break;
	default: break;
	}
}


