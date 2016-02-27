#include "BreakoutScene.h"
#include "SimpleAudioEngine.h"

using namespace std;
using namespace CocosDenshion;
USING_NS_CC;

Scene* BreakoutScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::createWithPhysics();
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	
	// 'layer' is an autorelease object
	auto layer = BreakoutScene::create();
	layer->SetPhysicsWorld(scene->getPhysicsWorld());

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

void BreakoutScene::initEnvironment()
{
	///////////////
	// Code for creating the borders
	//
	//////////////
	auto leftWallSprite = Sprite::create("wall_vertical.png");
	leftWallSprite->setAnchorPoint(Vec2());
	leftWallSprite->setPosition(0, 100);

	auto physicsBody = PhysicsBody::createBox(Size(leftWallSprite->getContentSize().width, 
												   leftWallSprite->getContentSize().height), 
												   PhysicsMaterial(0.1f, 1.0f, 0.0f));
	physicsBody->setDynamic(false);
	physicsBody->setGravityEnable(false);
	physicsBody->setCollisionBitmask(collisionBitMap::LEFT);
	physicsBody->setContactTestBitmask(true);

	leftWallSprite->setPhysicsBody(physicsBody);
	
	this->addChild(leftWallSprite, 0);

	auto rightWallSprite = Sprite::create("wall_vertical.png");
	rightWallSprite->setAnchorPoint(Vec2());
	rightWallSprite->setPosition(880, 100);

	physicsBody = PhysicsBody::createBox(Size(rightWallSprite->getContentSize().width,
											  rightWallSprite->getContentSize().height), 
											  PhysicsMaterial(0.1f, 1.0f, 0.0f));
	physicsBody->setDynamic(false);
	physicsBody->setGravityEnable(false);
	physicsBody->setCollisionBitmask(collisionBitMap::RIGHT);
	physicsBody->setContactTestBitmask(true);

	rightWallSprite->setPhysicsBody(physicsBody);
	this->addChild(rightWallSprite, 0);

	auto topWallSprite = Sprite::create("wall_horizontal.png");
	topWallSprite->setAnchorPoint(Vec2());
	topWallSprite->setPosition(0, 630);

	physicsBody = PhysicsBody::createBox(Size(topWallSprite->getContentSize().width,
											  topWallSprite->getContentSize().height), 
											  PhysicsMaterial(0.1f, 1.0f, 0.0f));
	physicsBody->setDynamic(false);
	physicsBody->setGravityEnable(false);
	physicsBody->setCollisionBitmask(collisionBitMap::TOP);
	physicsBody->setContactTestBitmask(true);

	topWallSprite->setPhysicsBody(physicsBody);
	this->addChild(topWallSprite, 0);

	auto bottomWallSprite = Sprite::create("wall_horizontal.png");
	bottomWallSprite->setAnchorPoint(Vec2());
	bottomWallSprite->setPosition(0, 100);

	physicsBody = PhysicsBody::createBox(Size(bottomWallSprite->getContentSize().width,
											  bottomWallSprite->getContentSize().height), 
											  PhysicsMaterial(0.1f, 1.0f, 0.0f));
	physicsBody->setDynamic(false);
	physicsBody->setGravityEnable(false);
	physicsBody->setCollisionBitmask(collisionBitMap::BOTTOM);
	physicsBody->setContactTestBitmask(true);

	bottomWallSprite->setPhysicsBody(physicsBody);
	this->addChild(bottomWallSprite, 0);

	///////////////
	// code for label and positioning
	//
	//////////////
	auto lifeLabel = Label::createWithTTF("Lives: ", "fonts/Marker Felt.ttf", 24);
	lifeLabel->setAnchorPoint(Vec2());
	lifeLabel->setPosition(10, 0);

	this->addChild(lifeLabel, 1);

	lifeValue = Label::createWithTTF(std::to_string(lifeCounter), "fonts/Marker Felt.ttf", 24);
	lifeValue->setAnchorPoint(Vec2());
	lifeValue->setPosition( 60, 0);

	this->addChild(lifeValue, 2);

	auto scoreLabel = Label::createWithTTF("Score: ", "fonts/Marker Felt.ttf", 24);
	scoreLabel->setAnchorPoint(Vec2());
	scoreLabel->setPosition(500, 0);

	this->addChild(scoreLabel, 1);

	scoreValue = Label::createWithTTF(std::to_string(score), "fonts/Marker Felt.ttf", 24);
	scoreValue->setAnchorPoint(Vec2());
	scoreValue->setPosition(600, 0);

	this->addChild(scoreValue, 2);
	
	///////////////
	//The following section is for creating/initializing the blocks to break
	//
	//////////////

	int numRows = 5;
	int numColumns = 10;
	int brickWidth = 80;
	int brickLength = 25;
	int brickTag = 10; //tags that will be used when destroying the bricks
	Sprite* brick;
	
	for (int i = 0; i < numRows; i++)
	{
		for (int j = 0; j < numColumns; j++)
		{
			switch (i)
			{
				case 0:
				case 2:
					brick = Sprite::create("redbrick.png");
					break;
				case 1:
				case 4:
					brick = Sprite::create("bluebrick.png");
					break;
				case 3:
					brick = Sprite::create("orangebrick.png");
					break;
			}
			brick->setAnchorPoint(Vec2());
			brick->setPosition(20 + (5 * (j + 1)) + (j * brickWidth), 605 - (i * brickLength) - (4 * i)); //handles the spacing for the bricks' position
			brick->setTag(brickTag);

			physicsBody = PhysicsBody::createBox(Size(brick->getContentSize().width,
													  brick->getContentSize().height), 
													  PhysicsMaterial(0.1f, 1.0f, 0.0f));
			physicsBody->setDynamic(false);
			physicsBody->setGravityEnable(false);
			physicsBody->setCollisionBitmask(brickTag);
			physicsBody->setContactTestBitmask(true);

			brick->setPhysicsBody(physicsBody);
			
			this->addChild(brick, 10);
			bricksMap[brickTag] = brick;
			brickTag++;

			brickCount++;
		}
	}
}

// on "init" you need to initialize your instance
bool BreakoutScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// adds a "close" icon to exit the progress. it's an autorelease object
	auto closeItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(BreakoutScene::menuCloseCallback, this));

	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width / 2,
		origin.y + closeItem->getContentSize().height / 2));

	// create menu, it's an autorelease object
	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	//initializing the variables
	//separated the initialization of the breaker and mrBall from the rest of the assets
	score = 0;

	initEnvironment();

	breaker = Sprite::create("breaker.png");
	breaker->setPosition(450, 170);
	
	auto physicsBody = PhysicsBody::createBox(Size(breaker->getContentSize().width,
										      breaker->getContentSize().height),
											  PhysicsMaterial(0.1f, 1.0f, 0.0f));
	physicsBody->setDynamic(false);
	physicsBody->setGravityEnable(false);
	physicsBody->setCollisionBitmask(collisionBitMap::BREAKER);
	physicsBody->setContactTestBitmask(true);

	breaker->setPhysicsBody(physicsBody);
	this->addChild(breaker);

	mrBall = Sprite::create("ball.png");
	mrBall->setPosition(450, 300);
	
	physicsBody = PhysicsBody::createBox(Size(mrBall->getContentSize().width,
											  mrBall->getContentSize().height),
											  PhysicsMaterial(0.1f, 1.0f, 0.0f));
	physicsBody->setDynamic(true);
	physicsBody->setGravityEnable(false);
	physicsBody->setCollisionBitmask(collisionBitMap::BREAKER);
	physicsBody->setContactTestBitmask(true);

	mrBall->setPhysicsBody(physicsBody);
	this->addChild(mrBall);
	move_horizontal = horizontalDirection::LEFT;
	move_vertical = verticalDirection::DOWN;

	isLeftPressed = false;
	isRightPressed = false;
	isPaused = true;

	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	audio->preloadBackgroundMusic("Happy Virus.mp3");
	audio->playBackgroundMusic("Happy Virus.mp3");
	auto keyListener = EventListenerKeyboard::create();
	keyListener->onKeyPressed = [=](EventKeyboard::KeyCode code, Event *event)->void{
		Vec2 currPos = breaker->getPosition();
		switch (code)
		{
			case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
			case EventKeyboard::KeyCode::KEY_A:
				isLeftPressed = true;
				breaker->setPosition(currPos.x - 5, currPos.y);
				break;
			case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
			case EventKeyboard::KeyCode::KEY_D:
				isRightPressed = true;
				breaker->setPosition(currPos.x + 5, currPos.y);
				break;
			case EventKeyboard::KeyCode::KEY_SPACE:
				if (isPaused)
					isPaused = false;
				else
					isPaused = true;
		}
	};

	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(BreakoutScene::onCollision, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);


	keyListener->onKeyReleased = [=](EventKeyboard::KeyCode code, Event *event) {
		switch (code)
		{
			case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
			case EventKeyboard::KeyCode::KEY_A:
				isLeftPressed = false;
				break;
			case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
			case EventKeyboard::KeyCode::KEY_D:
				isRightPressed = false;
				break;
		}
	};
	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);

	this->scheduleUpdate();
	return true;

}


//Code for detecting collisions
bool BreakoutScene::onCollision(PhysicsContact &contact)
{
	bool hasCollided = false;
	PhysicsBody *a = contact.getShapeA()->getBody();
	PhysicsBody *b = contact.getShapeB()->getBody();

	if (collisionBitMap::LEFT == a->getCollisionBitmask() || collisionBitMap::LEFT == b->getCollisionBitmask() ||
		collisionBitMap::RIGHT == a->getCollisionBitmask() || collisionBitMap::RIGHT == b->getCollisionBitmask())
	{
		changeBallDirection(false);
		hasCollided = true;
	}
	else if (collisionBitMap::TOP == a->getCollisionBitmask() || collisionBitMap::TOP == b->getCollisionBitmask() ||
			 collisionBitMap::BREAKER == a->getCollisionBitmask() || collisionBitMap::BREAKER == b->getCollisionBitmask())
	{
		changeBallDirection(true);
		hasCollided = true;
	}
	else if (collisionBitMap::BOTTOM == a->getCollisionBitmask() || collisionBitMap::BOTTOM == b->getCollisionBitmask())
	{
		lifeCounter--;
		lifeValue->setString(std::to_string(lifeCounter));
		hasCollided = true;
		if (lifeCounter == 0)
		{
			gameOver = true;
		}
	}

	for (map<int, Sprite*>::iterator brickIterator = bricksMap.begin(); brickIterator != bricksMap.end(); ++brickIterator)
	{
		if (brickIterator->first == a->getCollisionBitmask() || brickIterator->first == b->getCollisionBitmask())
		{
			this->removeChildByTag(brickIterator->first);
			score += 150;
			scoreValue->setString(std::to_string(score));
			brickCount--;
		}
	}


	return hasCollided;
}

void BreakoutScene::changeBallDirection(bool changeVerticalDirection)
{
	if (changeVerticalDirection)
	{
		if (move_vertical == verticalDirection::UP)
		{
			move_vertical = verticalDirection::DOWN;
		}
		else
		{
			move_vertical = verticalDirection::UP;
		}
	}
	else
	{
		if (move_horizontal == horizontalDirection::LEFT)
		{
			move_horizontal = horizontalDirection::RIGHT;
		}
		else
		{
			move_horizontal = horizontalDirection::LEFT;
		}
	}
}

void BreakoutScene::moveBall()
{
	Vec2 currPosition = mrBall->getPosition();
	if (move_horizontal == horizontalDirection::LEFT)
	{
		if (move_vertical == verticalDirection::UP)
		{
			mrBall->setPosition(currPosition.x - BALLSTEP, currPosition.y + BALLSTEP);
		}
		else if (move_vertical == verticalDirection::DOWN)
		{
			mrBall->setPosition(currPosition.x - BALLSTEP, currPosition.y - BALLSTEP);
		}
	}
	else if (move_horizontal == horizontalDirection::RIGHT)
	{
		if (move_vertical == verticalDirection::UP)
		{
			mrBall->setPosition(currPosition.x + BALLSTEP, currPosition.y + BALLSTEP);
		}
		else if (move_vertical == verticalDirection::DOWN)
		{
			mrBall->setPosition(currPosition.x + BALLSTEP, currPosition.y - BALLSTEP);
		}
	}
}

void BreakoutScene::checkIfDead()
{
	Vec2 ballPos = mrBall->getPosition();
	if (ballPos.y < 140)
	{
		mrBall->setPosition(450, 300);
		lifeCounter--;
		lifeValue->setString(std::to_string(lifeCounter));
	}
	if (lifeCounter == 0)
	{
		gameOver = true;
	}
}

void BreakoutScene::update(float delta)
{
	auto audio = SimpleAudioEngine::getInstance();
	if (!audio->isBackgroundMusicPlaying())
	{
		audio->rewindBackgroundMusic();
	}
	if (gameOver)
	{
		Label *gameOverLabel = Label::create("Game over", "fonts/Marker Felt.ttf", 40);
		gameOverLabel->setAnchorPoint(Vec2());
		gameOverLabel->setPosition(450, 280);

		if (brickCount == 0)
		{
			gameOverLabel->setString("You Win!");
		}
		this->addChild(gameOverLabel, 20);
	}
	else if (!isPaused)
	{
		moveBall();

		Vec2 currPos = breaker->getPosition();
		if (isLeftPressed && currPos.x - 80 > 20)
		{
			breaker->setPosition(currPos.x - 5, currPos.y);
		}
		else if (isRightPressed && currPos.x + 80 < 880)
		{
			breaker->setPosition(currPos.x + 5, currPos.y);
		}
		checkIfDead();

		if (brickCount < 40 && BALLSTEP >= 25)
		{
			BALLSTEP = 3;
		}
		if (brickCount < 25 && BALLSTEP >= 15)
		{
			BALLSTEP = 4;
		}
		if (brickCount < 15)
		{
			BALLSTEP = 5;
		}
		if (brickCount == 0)
		{
			gameOver = true;
		}
	}
}

void BreakoutScene::menuCloseCallback(Ref* pSender)
{
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

BreakoutScene::BreakoutScene()
{

}


BreakoutScene::~BreakoutScene()
{
}
