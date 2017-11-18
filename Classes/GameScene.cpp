#include "GameScene.h"

Scene* Game::createScene(){
	auto scene = Scene::createWithPhysics();
	scene->getPhysicsWorld()->setGravity(Vec2(0, -900));
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	auto layer = Game::create();
	scene->addChild(layer);
	return scene;
}

bool Game::init(){
	if (!Scene::init())
		return false;

	//ready state
	gameState = STATE_READY;

	//get size
	visibleSize = Director::getInstance()->getVisibleSize();

	backgroundInit();
	birdInit();
	pipeInit();
	landInit();

	//show score
	score = 0;
	bestScore = UserDefault::getInstance()->getIntegerForKey("best");
	scoreLayer = ScoreLayer::create();
	this->addChild(scoreLayer);

	//touch listener
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = [=](Touch*  touch, Event* event){
		if (gameState == STATE_END)
			return false;
		if (gameState == STATE_READY){
			gameState = STATE_PLAYING;
			auto fadeout = FadeOut::create(0.3f);
			tutorial->runAction(fadeout);
			ready->runAction(fadeout->clone());
			bird->stopActionByTag(ANIMATION_UPDOWN);
			bird->getPhysicsBody()->setGravityEnable(true);
		}
		else{
			//SimpleAudioEngine::getInstance()->playEffect("wing.wav");
			bird->getPhysicsBody()->setVelocity(Vec2(0, 260));
		}
		return true;
	};

	//contact listener
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = [=](PhysicsContact& contact){
		/*if (gameState == STATE_END)
			return false;*/
		if (gameState == STATE_PLAYING){
			auto shapea = contact.getShapeA(), shapeb = contact.getShapeB();
			if (shapea->getGroup() != shapeb->getGroup()){
				if (shapea->getGroup() == GROUP_BIRD || shapeb->getGroup() == GROUP_BIRD){
					gameOver();
				}
			}
		}
		return true;
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

	this->schedule(schedule_selector(Game::scrollLand));
	this->scheduleUpdate();

	return true;
}

void Game::scrollLand(float delta){
	if (gameState == STATE_END)
		return;
	//move land
	land1->setPositionX(land1->getPositionX() - 3.0f);
	land2->setPositionX(land1->getPositionX() + land1->getContentSize().width);
	if (land2->getPositionX() <= 0)
		land1->setPositionX(0.0f);

	//move pipes
	if (gameState == STATE_PLAYING){
		for (int i = 0; i < 2; i++){
			if (pipes[i]->getPositionX() <= -PIPE_SIZE.width){
				pipes[i]->setPosition(visibleSize.x + PIPE_SIZE.width, random(0, 200));
				pipes[i]->setTag(PIPE_NEW);
			}
			else
				pipes[i]->setPositionX(pipes[i]->getPositionX() - 3.0f);
		}
	}
}

void Game::update(float delta){
	if (gameState == STATE_PLAYING){
		rotateBird();
		updateScore();
	}
}

void Game::birdInit(){
	//add bird
	bird = Sprite::createWithSpriteFrameName("bird0_1.png");
	bird->setPosition(visibleSize.x / 4, visibleSize.y / 2);
	//add animation
	auto swing = Animate::create(AnimationCache::getInstance()->getAnimation("birdAnimation0"));
	auto up = MoveBy::create(0.5f, Vec2(0, 9));
	auto down = up->reverse();
	auto updown = Sequence::create(up, down, nullptr);
	auto updown_forever = RepeatForever::create(updown);
	updown_forever->setTag(ANIMATION_UPDOWN);
	bird->runAction(RepeatForever::create(swing));
	bird->runAction(updown_forever);
	//add physics
	auto birdBody = PhysicsBody::createCircle(BIRD_RADIUS);
	birdBody->setDynamic(true);
	birdBody->setGravityEnable(false);
	birdBody->setContactTestBitmask(1);
	birdBody->setGroup(GROUP_BIRD);
	bird->setPhysicsBody(birdBody);
	this->addChild(bird);
}

void Game::pipeInit(){
	//add pipes
	for (int i = 0; i < 2; i++){
		Node* pipe = createPipeRandom(PIPE_INTERVALS*i + WAIT_DISTANCE + visibleSize.x);
		this->addChild(pipe);
		pipes.push_back(pipe);
	}
}

void Game::landInit(){
	//add land
	land1 = Sprite::createWithSpriteFrameName("land.png");
	land2 = Sprite::createWithSpriteFrameName("land.png");
	land1->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
	land2->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
	auto landBody1 = PhysicsBody::createBox(LAND_SIZE);
	landBody1->setDynamic(false);
	landBody1->setGroup(GROUP_LAND);
	landBody1->setContactTestBitmask(2);
	auto landBody2 = PhysicsBody::createBox(LAND_SIZE);
	landBody2->setDynamic(false);
	landBody2->setGroup(GROUP_LAND);
	landBody2->setContactTestBitmask(2);
	land1->setPhysicsBody(landBody1);
	land2->setPhysicsBody(landBody2);
	this->addChild(land1);
	this->addChild(land2);
}

void Game::backgroundInit(){
	//add background
	auto bg = Sprite::createWithSpriteFrameName("bg_day.png");
	bg->setPosition(visibleSize.x / 2, visibleSize.y / 2);
	this->addChild(bg, -1);

	//add tutorial
	tutorial = Sprite::createWithSpriteFrameName("tutorial.png");
	tutorial->setPosition(visibleSize.x / 2, visibleSize.y / 2);
	tutorial->setVisible(true);
	this->addChild(tutorial);

	//add ready tag
	ready = Sprite::createWithSpriteFrameName("text_ready.png");
	ready->setPosition(visibleSize.x / 2, visibleSize.y / 1.5);
	ready->setVisible(true);
	this->addChild(ready);
}

Node* Game::createPipeRandom(int x){
	int randomHeight = random(0, 200);
	auto pipeup = Sprite::createWithSpriteFrameName("pipe_down.png");
	auto pipedown = Sprite::createWithSpriteFrameName("pipe_up.png");
	auto pipeupBody = PhysicsBody::createBox(PIPE_SIZE);
	pipeupBody->setDynamic(false);
	pipeupBody->setContactTestBitmask(4);
	pipeupBody->setGroup(GROUP_PIPE);
	pipeup->setPhysicsBody(pipeupBody);
	auto pipedownBody = PhysicsBody::createBox(PIPE_SIZE);
	pipedownBody->setDynamic(false);
	pipedownBody->setContactTestBitmask(4);
	pipedownBody->setGroup(GROUP_PIPE);
	pipedown->setPhysicsBody(pipedownBody);
	pipeup->setPositionY(PIPE_DISTANCE + PIPE_SIZE.height);
	//bind 2 pipes
	auto singlePipe = Node::create();
	singlePipe->addChild(pipedown, 0, "pipeup");
	singlePipe->addChild(pipeup, 0, "pipedown");
	singlePipe->setPosition(x, randomHeight);
	singlePipe->setTag(PIPE_NEW);
	return singlePipe;
}

void Game::gameOver(){
	SimpleAudioEngine::getInstance()->playEffect("hit.wav");
	SimpleAudioEngine::getInstance()->playEffect("die.wav");
	bestScore = max(bestScore, score);
	UserDefault::getInstance()->setIntegerForKey("best", bestScore);
	//show over text
	auto overLabel = Sprite::createWithSpriteFrameName("text_game_over.png");
	overLabel->setOpacity(0);
	auto fadeto = FadeTo::create(1.0f, 255);
	overLabel->runAction(fadeto);
	overLabel->setPosition(visibleSize.x / 2, visibleSize.y / 1.5);
	this->addChild(overLabel);
	//hide scores
	scoreLayer->hideScores();

	birdFadeOut();
	slidePanelAndButton();
	gameState = STATE_END;
}

void Game::rotateBird(){
	float verticalSpeed = bird->getPhysicsBody()->getVelocity().y;
	bird->setRotation(min(max(-30, (int)(-0.2*verticalSpeed - 40)), 45));
}

void Game::updateScore(){
	for (int i = 0; i < 2; i++){
		if (pipes[i]->getTag() == PIPE_NEW){
			if (bird->getPositionX() > pipes[i]->getPositionX()){
				//SimpleAudioEngine::getInstance()->playEffect("point.wav");
				score++;
				scoreLayer->updateScore(score);
				pipes[i]->setTag(PIPE_PASS);
			}
		}
	}
}

void Game::restartCallBack(Ref* pSender){
	SimpleAudioEngine::getInstance()->playEffect("swooshing.wav");
	auto newScene = Game::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f, newScene));
}

void Game::birdFadeOut(){
	bird->stopAllActions();
	auto fadeout = FadeOut::create(2.0f);
	auto rotate = RotateTo::create(0.01f, 90);
	bird->runAction(fadeout);
	bird->runAction(rotate);
}


void Game::slidePanelAndButton(){
	//show score on panel
	scoreLayer->showPanel();
	scoreLayer->showScoreOnPanel(score);
	scoreLayer->showBestScoreOnPanel(bestScore);
	//slide button
	auto restartButton = Sprite::createWithSpriteFrameName("button_play.png");
	auto restartButtonDown = Sprite::createWithSpriteFrameName("button_play.png");
	restartButtonDown->setPositionY(-5);
	auto restartItem = MenuItemSprite::create(restartButton, restartButtonDown, CC_CALLBACK_1(Game::restartCallBack, this));
	auto restartMenu = Menu::createWithItem(restartItem);
	restartMenu->setPosition(visibleSize.x / 2, -50);
	auto delay = DelayTime::create(1.5f);
	auto slidebutton = MoveTo::create(1.0f, Vec2(visibleSize.x / 2, visibleSize.y / 3.3));
	auto slide = EaseExponentialInOut::create(slidebutton);
	restartMenu->runAction(Sequence::create(delay, slide, nullptr));
	this->addChild(restartMenu);
}