#ifndef __SCORE_LAYER_H__
#define __SCORE_LAYER_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include <stack>

USING_NS_CC;
using namespace CocosDenshion;
using namespace std;

class ScoreLayer : public Layer
{
public:
	
	virtual bool init();
	CREATE_FUNC(ScoreLayer);

	void updateScore(int);
	void hideScores();
	void showPanel();
	void showScoreOnPanel(int);
	void showBestScoreOnPanel(int);


private:
	Vec2 visibleSize;
	vector<Sprite*> scores;
};

#endif 
