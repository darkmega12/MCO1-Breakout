#pragma once

#include "cocos2d.h"

USING_NS_CC;

class BreakoutScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	//initialize the assets in their positions
	void initEnvironment();

	// a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender);

	// implement the "static create()" method manually
	CREATE_FUNC(BreakoutScene);

	BreakoutScene();
	~BreakoutScene();
};

