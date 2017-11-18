#include "MainMenuScene.h"
#include "GameScene.h"

Scene* MainMenu::createScene(){
	return MainMenu::create();
}

bool MainMenu::init(){
	if (!Scene::init())
		return false;

	//get size
	Vec2 visibleSize = Director::getInstance()->getVisibleSize();

	//add background
	auto bg = Sprite::createWithSpriteFrameName("bg_day.png");
	bg->setPosition(visibleSize.x / 2, visibleSize.y / 2);
	this->addChild(bg, -1);

	//add bird
	auto bird = Sprite::createWithSpriteFrameName("bird0_1.png");
	bird->setPosition(visibleSize.x / 2, visibleSize.y / 1.5);
	auto swing = Animate::create(AnimationCache::getInstance()->getAnimation("birdAnimation0"));
	auto up = MoveBy::create(0.5f, Vec2(0, 9));
	auto down = up->reverse();
	auto updown = Sequence::create(up, down, nullptr);
	bird->runAction(RepeatForever::create(swing));
	bird->runAction(RepeatForever::create(updown));
	this->addChild(bird);

	//add title
	auto title = Sprite::createWithSpriteFrameName("title.png");
	title->setPosition(visibleSize.x / 2, visibleSize.y / 1.2);
	this->addChild(title);

	//add land
	land1 = Sprite::createWithSpriteFrameName("land.png");
	land2 = Sprite::createWithSpriteFrameName("land.png");
	land1->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
	land2->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
	this->addChild(land1);
	this->addChild(land2);

	//add start button
	auto button_start = Sprite::createWithSpriteFrameName("button_play.png");
	auto button_start_touched = Sprite::createWithSpriteFrameName("button_play.png");
	button_start_touched->setPosition(0, - 5);
	auto menu_start = MenuItemSprite::create(button_start, button_start_touched, CC_CALLBACK_1(MainMenu::startCallBack, this));
	auto menu = Menu::create(menu_start, nullptr);
	menu->setPosition(visibleSize.x / 2, visibleSize.y / 2);
	this->addChild(menu);


	this->schedule(schedule_selector(MainMenu::scrollLand));

	return true;
}

void MainMenu::scrollLand(float delta){
	land1->setPositionX(land1->getPositionX() - 2.0f);
	land2->setPositionX(land1->getPositionX() + land1->getContentSize().width);
	if (land2->getPositionX() <= 0)
		land1->setPositionX(0.0f);
}

void MainMenu::startCallBack(Ref* pSender){
	SimpleAudioEngine::getInstance()->playEffect("swooshing.wav");
	auto scene = Game::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f, scene));
}