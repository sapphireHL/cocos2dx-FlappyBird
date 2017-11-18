#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "ScoreLayer.h"

USING_NS_CC;
using namespace CocosDenshion;
using namespace std;

const int BIRD_RADIUS = 15;
const Size LAND_SIZE = Size(366, 122);
const Size PIPE_SIZE = Size(52, 320);
const int WAIT_DISTANCE = 100;
const int PIPE_DISTANCE = 120;
const int PIPE_INTERVALS = 196;
const int ANIMATION_UPDOWN = 100;
const int ANIMATION_SWING = 101;
const int GROUP_PIPE = 200;
const int GROUP_LAND = 201;
const int GROUP_BIRD = 202;
const int PIPE_NEW = 300;
const int PIPE_PASS = 301;
enum GameState
{
	STATE_READY,
	STATE_PLAYING,
	STATE_END
};

class Game : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(Game);

	void birdInit();
	void pipeInit();
	void landInit();
	void backgroundInit();
	Node* createPipeRandom(int x);
	void scrollLand(float);
	void update(float) override;
	void rotateBird();
	void updateScore();
	void gameOver();
	void restartCallBack(Ref* pSender);
	void birdFadeOut();
	void slidePanelAndButton();

private:
	Vec2 visibleSize;
	Sprite* land1, *land2;
	Sprite* bird;
	vector<Node*> pipes;
	Sprite* tutorial, *ready;
	GameState gameState;
	int score;
	int bestScore;
	ScoreLayer* scoreLayer;
};

#endif 