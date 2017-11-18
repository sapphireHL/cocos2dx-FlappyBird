#ifndef __LOADING_SCENE_H__
#define __LOADING_SCENE_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "AudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;
using namespace experimental;

class Loading : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(Loading);

	void loadingCallBack(Texture2D*);
};

#endif 
