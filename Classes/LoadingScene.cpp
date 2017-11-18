#include "LoadingScene.h"
#include "MainMenuScene.h"

Scene* Loading::createScene(){
	return Loading::create();
}

bool Loading::init(){
	if (!Scene::init())
		return false;

	//get size
	Vec2 visibleSize = Director::getInstance()->getVisibleSize();

	//add background
	auto bg = Sprite::create("splash.png");
	bg->setPosition(visibleSize.x / 2, visibleSize.y / 2);
	this->addChild(bg);

	//preload texture
	TextureCache::getInstance()->addImageAsync("data.png", CC_CALLBACK_1(Loading::loadingCallBack, this));

	return true;
}

void Loading::loadingCallBack(Texture2D* texture){
	//preload spriteframe
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("data.plist", texture);

	//preload animation
	auto animation0 = Animation::create();
	animation0->setDelayPerUnit(0.1f);
	animation0->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("bird0_0.png"));
	animation0->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("bird0_1.png"));
	animation0->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("bird0_2.png"));
	auto animation1 = Animation::create();
	animation1->setDelayPerUnit(0.1f);
	animation1->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("bird1_0.png"));
	animation1->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("bird1_1.png"));
	animation1->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("bird1_2.png"));
	auto animation2 = Animation::create();
	animation2->setDelayPerUnit(0.1f);
	animation2->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("bird2_0.png"));
	animation2->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("bird2_1.png"));
	animation2->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("bird2_2.png"));
	AnimationCache::getInstance()->addAnimation(animation0, "birdAnimation0");
	AnimationCache::getInstance()->addAnimation(animation1, "birdAnimation1");
	AnimationCache::getInstance()->addAnimation(animation2, "birdAnimation2");

	//preload music
	SimpleAudioEngine::getInstance()->preloadEffect("die.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("hit.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("point.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("swooshing.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("wing.wav");

	//preload complete, show next scene
	auto scene = MainMenu::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f, scene));
	
}