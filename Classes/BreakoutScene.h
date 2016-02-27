#pragma once

#include "cocos2d.h"

USING_NS_CC;

class BreakoutScene : public cocos2d::Layer
{
private:
	int lifeCounter = 3;
	int move_horizontal;
	int move_vertical;
	int score = 0;
	int brickCount = 0;
	int BALLSTEP = 2;
	
	Label* scoreValue;
	Label* lifeValue;
	Sprite *mrBall;
	Sprite *breaker;

	std::map<int, Sprite*> bricksMap;

	bool isLeftPressed;
	bool isRightPressed;
	bool isPaused;
	bool gameOver = false;

	struct horizontalDirection
	{
		enum type{ LEFT = 0, RIGHT = 1};
	};
	struct verticalDirection
	{
		enum type{ UP = 0, DOWN = 1};
	};
	struct collisionBitMap
	{
		enum type{ LEFT = 0, RIGHT = 1, TOP = 2, BOTTOM = 3, BREAKER = 4, BALL = 5};
	};

	PhysicsWorld *sceneWorld;
	void SetPhysicsWorld(PhysicsWorld *world) { sceneWorld = world; };
public:
	static cocos2d::Scene* createScene();

	virtual bool init();


	//initialize the assets in their positions
	void initEnvironment();

	// a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender);

	// implement the "static create()" method manually
	CREATE_FUNC(BreakoutScene);

	void update(float) override;

	void moveBall();

	void collide();

	void checkIfDead();

	void changeBallDirection(bool changeVerticalDirection); //used vertical just to distinguish vertical and horizontal

	bool onCollision(PhysicsContact &contact);

	BreakoutScene();
	~BreakoutScene();
};

