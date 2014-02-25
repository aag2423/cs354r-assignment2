#include "Game.h"

#include <OgreFrameListener.h>
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <iostream>

Game::Game(Ogre::SceneManager* mSceneMgr, Ogre::SceneNode* camNode, GameMode mode) :
	graphicsEngine(mSceneMgr),
	cameraNode(camNode),
	gameMode(mode)
{

	gameState.g = EARTH_G;
	gameState.ballRestitution = 0.99;
	gameState.camMode = ABOVE_CAM;
	gameState.paused = false;
	gameState.gameStarted = false;
	gameState.combo = false;
	gameState.comboBonus = 0;
	gameState.playerScore = 0;
	gameState.computerScore = 0;
	gameState.progress = ENDED;
	gameState.result = ONGOING;


	physicsEngine.setGravity(0, EARTH_G, 0);

	court = new PlayGround(mSceneMgr, physicsEngine, FULL_COURT);
	
	target1 = new Target(mSceneMgr, physicsEngine, court, Ogre::Vector3(-150, 0, -PRACTICE_COURT_LENGTH/2), "Examples/Target");
	target2 = new Target(mSceneMgr, physicsEngine, court, Ogre::Vector3(0, -50, -PRACTICE_COURT_LENGTH/2), "Examples/Target");
	target3 = new Target(mSceneMgr, physicsEngine, court, Ogre::Vector3(150, 0, -PRACTICE_COURT_LENGTH/2), "Examples/Target");

	Ogre::Vector3 halfDim;
	court->getHalfDimension(halfDim);

	ball = new Ball(mSceneMgr, physicsEngine, court, Ogre::Vector3(5, -75, halfDim.z - 320));
	player = new Player(mSceneMgr, physicsEngine, HUMAN, court, Ogre::Vector3(0, -125, halfDim.z - 300));

	computer = new Player(mSceneMgr, physicsEngine, AI, court, Ogre::Vector3(0, -125, -FULL_COURT_LENGTH/2 + 300));
	computer->getNode()->yaw(Ogre::Degree(180));
	if (mode == FULL_GAME) {
		target1->toggleVisible();
		target2->toggleVisible();
		target3->toggleVisible();
	} else 
		computer->toggleVisible();
	
	toggleCamera();
}

//-------------------------------------------------------------------------------------

Game::~Game(void) {
	delete ball;
	delete player;
	delete computer;
	delete target1;
	delete target2;
	delete target3;
	delete court; // must be deleted last
	std::cout << "========= Debug: Game Deleted =========" << std::endl;
}

//-------------------------------------------------------------------------------------

void Game::toggleGameMode(void) {
	if (gameMode == PRACTICE) {
		gameMode = FULL_GAME;
	} else {
		gameMode = PRACTICE;
	}
	court->toggleType();
	computer->toggleVisible();
	target1->toggleVisible();
	target2->toggleVisible();
	target3->toggleVisible();
	reset();
}

void Game::rePosition(void) {
	delete ball;
	Ogre::Vector3 halfDim;
	court->getHalfDimension(halfDim);
	ball = new Ball(graphicsEngine, physicsEngine, court, Ogre::Vector3(5, -75, halfDim.z - 320));
	ball->getPhysicsObject().setRestitution(gameState.ballRestitution);
	player->setPosition(Ogre::Vector3(5, -125, halfDim.z - 300));  
	player->resetState(); 
	computer->setPosition(Ogre::Vector3(5, -125, -halfDim.z + 300));
	computer->getNode()->yaw(Ogre::Degree(180));
	computer->resetState();
}


//-------------------------------------------------------------------------------------

void Game::reset(void) {

	gameState.gameStarted = false;
	gameState.combo = false;
	gameState.comboBonus = 0;
	gameState.playerScore = 0;
	gameState.computerScore = 0;
	gameState.progress = ENDED;
	gameState.result = ONGOING;
	target2->resetScore();
	target3->resetScore();
	target1->resetScore();
	rePosition();
	
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


void Game::checkScoring(BallCollisionEvent ballEvent) {
	Ogre::Real ballPosZ = ball->getNode()->getPosition().z;
	if (gameState.progress == HIT_BY_PLAYER) {
		switch(ballEvent){
			case HIT_CEILING:
			case HIT_WALL: 
			case HIT_PLAYER:
				gameState.computerScore++; gameState.progress = ENDED; break;
			case HIT_FLOOR: 
				if (ballPosZ > 0){
					gameState.computerScore++; gameState.progress = ENDED;
				}else
					gameState.progress = HIT_BY_PLAYER_AND_FLOOR; 
				break;
			case HIT_OPPONENT:
				gameState.playerScore++; gameState.progress = ENDED; break;
			default: break;
		}
	} else if (gameState.progress == HIT_BY_PLAYER_AND_FLOOR) {
		if(ballEvent != NOTHING_HAPPENED) {
				gameState.playerScore++;
				gameState.progress = ENDED;
		}
	} else if (gameState.progress == HIT_BY_OPPONENT) {
		switch(ballEvent){
			case HIT_CEILING:
			case HIT_WALL: 
			case HIT_OPPONENT:
				gameState.playerScore++; gameState.progress = ENDED; break;
			case HIT_FLOOR: 
				if (ballPosZ < 0){
					gameState.playerScore++; gameState.progress = ENDED;
				}else
					gameState.progress = HIT_BY_OPPONENT_AND_FLOOR; 
				break;
			case HIT_PLAYER:
				gameState.computerScore++; gameState.progress = ENDED; break;
			default: break;
		}
	} else if (gameState.progress == HIT_BY_OPPONENT_AND_FLOOR) {
		if(ballEvent != NOTHING_HAPPENED) {
				gameState.computerScore++;
				gameState.progress = ENDED;
		}
	}
	if (gameState.progress == ENDED) {
		gameState.gameStarted = false;
		rePosition();
	}
}


void Game::runNextFrame(const Ogre::FrameEvent& evt) {
	if(gameState.paused) return;
	if (!gameState.gameStarted) return;
	bool isFullGame = gameMode == FULL_GAME;

	player->move(evt, isFullGame);
	if (isFullGame) {
		runAI();
		computer->move(evt);
	}
	physicsEngine.stepSimulation(evt.timeSinceLastFrame*5);
	ball->updateGraphicsScene();

	if(ball->hitBy(player))	{
		soundHandler->play_sound(ball_hit);
		if (gameState.progress == HIT_BY_PLAYER_AND_FLOOR) {
			gameState.computerScore++;
			gameState.progress = ENDED;
		} else
			gameState.progress = HIT_BY_PLAYER;
	}
	if (isFullGame && ball->hitBy(computer)) {
		soundHandler->play_sound(ball_hit);
		if (gameState.progress == HIT_BY_OPPONENT_AND_FLOOR) {
			gameState.playerScore++;
			gameState.progress = ENDED;
		} else
			gameState.progress = HIT_BY_OPPONENT;
	}

	BallCollisionEvent be = ball->collidesWith(court, player);
	


	if (be == HIT_FLOOR) {
		gameState.combo = false;
		gameState.comboBonus = 0;
		soundHandler->play_sound(ball_hit_floor);
	}
	if (be == HIT_PLAYER)
		soundHandler->play_sound(grunt);
	if (be == HIT_WALL)
		soundHandler->play_sound(ball_hit_floor);
		
	if (isFullGame) {
		checkScoring(be);
		if (getScore() > 10) {
			std::cout << "You win!"<<std::endl;
			gameState.result = WIN;
			gameState.gameStarted = false;
			soundHandler->play_sound(player_win);
		
		}
		if (getOpponentScore() > 10) {
			std::cout << "You Lose!"<<std::endl;
			gameState.result = LOSE;
			gameState.gameStarted = false;
			soundHandler->play_sound(player_lose);
		
		}
		return;
	} else {	
		if (getScore() >= 99999) {
			std::cout << "You've played too much!"<<std::endl;
			gameState.paused = true;
	
		}
	}
	
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
}

//-------------------------------------------------------------------------------------

void Game::toggleCamera(void) {
	if (gameState.paused) return;
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
	case TOGGLE_GAME_MODE:
		toggleGameMode(); 
		break;
	case RESTART:
		reset(); break;
	case PAUSE:
		gameState.paused = !gameState.paused; break;
	default: break;
	}
}

void Game::handleMouseMove(Ogre::Real dx, Ogre::Real dy) {
	if (gameState.paused || gameState.result != ONGOING) return;
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
	if (gameState.paused || gameState.result != ONGOING) return;
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


