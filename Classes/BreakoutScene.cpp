#include "BreakoutScene.h"

USING_NS_CC;

Scene* BreakoutScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = BreakoutScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

Label* scoreValue;

void BreakoutScene::initEnvironment()
{
	
	// Code for creating the borders
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

	auto lifeLabel = Label::createWithTTF("Lives: ", "fonts/Marker Felt.ttf", 24);
	lifeLabel->setAnchorPoint(Vec2());
	lifeLabel->setPosition(10, 0);

	this->addChild(lifeLabel, 1);

	auto scoreLabel = Label::createWithTTF("Score: ", "fonts/Marker Felt.ttf", 24);
	scoreLabel->setAnchorPoint(Vec2());
	scoreLabel->setPosition(500, 0);

	this->addChild(scoreLabel, 1);

	scoreValue = Label::createWithTTF("0", "fonts/Marker Felt.ttf", 24);
	scoreValue->setAnchorPoint(Vec2());
	scoreValue->setPosition(600, 0);

	this->addChild(scoreValue, 2);
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

	/////////////////////////////
	// 2. add a menu item with "X" image, which is clicked to quit the program
	//    you may modify it.

	// add a "close" icon to exit the progress. it's an autorelease object
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

	/*
	/////////////////////////////
	// 3. add your codes below...

	// add a label shows "Hello World"
	// create and initialize a label

	auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);

	// position the label on the center of the screen
	label->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height - label->getContentSize().height));

	// add the label as a child to this layer
	this->addChild(label, 1);

	// add "HelloWorld" splash screen"
	auto sprite = Sprite::create("HelloWorld.png");

	// position the sprite on the center of the screen
	sprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

	// add the sprite as a child to this layer
	this->addChild(sprite, 0);
	*/

	initEnvironment();

	return true;

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
