#pragma once

#include "cocos2d.h"

USING_NS_CC;

class BreakoutScene : public cocos2d::Layer
{
private:
	CCArray *pLives;
	int move_horizontal;
	int move_vertical;
	int score;
	Label* scoreValue;
	Sprite *mrBall;
	Sprite *breaker;

	int BALLSTEP = 2;

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
protected:
	cocos2d::CCArray *_bricks;
	cocos2d::CCArray *_ball;
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

	void checkIfDead();

	BreakoutScene();
	~BreakoutScene();
};

