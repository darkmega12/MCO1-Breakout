#include "BreakoutScene.h"

USING_NS_CC;

Scene* BreakoutScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::createWithPhysics();

	// 'layer' is an autorelease object
	auto layer = BreakoutScene::create();

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

	this->addChild(leftWallSprite, 0);

	auto rightWallSprite = Sprite::create("wall_vertical.png");
	rightWallSprite->setAnchorPoint(Vec2());
	rightWallSprite->setPosition(880, 100);

	this->addChild(rightWallSprite, 0);

	auto topWallSprite = Sprite::create("wall_horizontal.png");
	topWallSprite->setAnchorPoint(Vec2());
	topWallSprite->setPosition(0, 630);

	this->addChild(topWallSprite, 0);

	auto bottomWallSprite = Sprite::create("wall_horizontal.png");
	bottomWallSprite->setAnchorPoint(Vec2());
	bottomWallSprite->setPosition(0, 100);

	this->addChild(bottomWallSprite, 0);

	///////////////
	// code for label and positioning
	//
	//////////////
	auto lifeLabel = Label::createWithTTF("Lives: ", "fonts/Marker Felt.ttf", 24);
	lifeLabel->setAnchorPoint(Vec2());
	lifeLabel->setPosition(10, 0);

	this->addChild(lifeLabel, 1);

	auto scoreLabel = Label::createWithTTF("Score: ", "fonts/Marker Felt.ttf", 24);
	scoreLabel->setAnchorPoint(Vec2());
	scoreLabel->setPosition(500, 0);

	this->addChild(scoreLabel, 1);

	scoreValue = Label::createWithTTF(std::to_string(score), "fonts/Marker Felt.ttf", 24);
	scoreValue->setAnchorPoint(Vec2());
	scoreValue->setPosition(600, 0);

	this->addChild(scoreValue, 2);
	
	pLives = CCArray::create();
	Sprite *life = Sprite::create("ball.png");
	life->setAnchorPoint(Vec2());
	life->setPosition(80, 0);
	this->addChild(life);
	pLives->addObject(life);

	life = Sprite::create("ball.png");
	life->setAnchorPoint(Vec2());
	life->setPosition(120, 0);
	this->addChild(life);
	pLives->addObject(life);

	life = Sprite::create("ball.png");
	life->setAnchorPoint(Vec2());
	life->setPosition(160, 0);
	this->addChild(life);
	pLives->addObject(life);

	///////////////
	//The following section is for creating/initializing the blocks to break
	//
	//////////////

	int numRows = 5;
	int numColumns = 10;
	int brickWidth = 80;
	int brickLength = 25;
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
			_bricks->addObject(brick);
			this->addChild(brick, 10);
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
	_bricks = CCArray::create();
	score = 0;

	initEnvironment();

	breaker = Sprite::create("breaker.png");
	breaker->setPosition(450, 170);
	this->addChild(breaker, 10);

	mrBall = Sprite::create("ball.png");
	mrBall->setPosition(450, 300);
	this->addChild(mrBall, 10);
	move_horizontal = horizontalDirection::LEFT;
	move_vertical = verticalDirection::DOWN;

	isLeftPressed = false;
	isRightPressed = false;
	isPaused = true;

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
	if (ballPos.y < 150)
	{
		pLives->removeLastObject();
		mrBall->setPosition(450, 300);
	}
	if (pLives->count() == 0)
	{
		gameOver = true;
	}
}

void BreakoutScene::update(float delta)
{
	if (gameOver)
	{
		Label *gameOverLabel = Label::create("Game over", "fonts/Marker Felt.ttf", 30);
		gameOverLabel->setAnchorPoint(Vec2());
		gameOverLabel->setPosition(450, 310);

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

		//checkIfDead();
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
