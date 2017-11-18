#include "ScoreLayer.h"

bool ScoreLayer::init(){
	if (!Layer::init())
		return false;

	visibleSize = Director::getInstance()->getVisibleSize();

	//add score
	auto score = Sprite::createWithSpriteFrameName("font_048.png");
	score->setPosition(visibleSize.x / 2, visibleSize.y / 1.2);
	scores.push_back(score);
	this->addChild(scores[0]);
	return true;
}

void ScoreLayer::updateScore(int score){
	stack<int> bit;
	while (score != 0){
		bit.push(score % 10);
		score /= 10;
	}

	int i = 0;
	while (!bit.empty()){
		int curScore = bit.top();
		bit.pop();
		string scoreFileName = "font_0" + to_string(48 + curScore) + ".png";
		if (i >= (int)scores.size()){
			auto newscore = Sprite::createWithSpriteFrameName(scoreFileName);
			scores.push_back(newscore);
			scores[i]->setPosition(scores[i - 1]->getPosition().x + 24, scores[i - 1]->getPosition().y);
			this->addChild(scores[i]);
		}
		else{
			Vec2 pos = scores[i]->getPosition();
			this->removeChild(scores[i]);
			scores[i] = Sprite::createWithSpriteFrameName(scoreFileName);
			scores[i]->setPosition(pos);
			this->addChild(scores[i]);
		}
		i++;
	}
}

void ScoreLayer::hideScores(){
	for (int i = 0; i < (int)scores.size(); i++){
		scores[i]->setVisible(false);
	}
}

void ScoreLayer::showScoreOnPanel(int score){
	int i = 0;
	do
	{
		string scoreFileName = "number_score_0" + to_string(score % 10) + ".png";
		auto scoresNumber = Sprite::createWithSpriteFrameName(scoreFileName);
		scoresNumber->setPosition(visibleSize.x / 1.3 - 16 * i, visibleSize.y / 2 + 15);
		scoresNumber->setOpacity(0);
		auto fadeto = FadeTo::create(0.1f, 255);
		auto delay = DelayTime::create(2.0f);
		scoresNumber->runAction(Sequence::create(delay, fadeto, nullptr));
		this->addChild(scoresNumber);
		score /= 10;
		i++;
	} while (score != 0);
}

void ScoreLayer::showBestScoreOnPanel(int score){
	int i = 0;
	do
	{
		string scoreFileName = "number_score_0" + to_string(score % 10) + ".png";
		auto scoresNumber = Sprite::createWithSpriteFrameName(scoreFileName);
		scoresNumber->setPosition(visibleSize.x / 1.3 - 16 * i, -112);
		auto slidescore = MoveTo::create(1.0f, Vec2(visibleSize.x / 1.3 - 16 * i, visibleSize.y / 2 - 22));
		auto slide = EaseExponentialOut::create(slidescore);
		auto delay = DelayTime::create(1.0f);
		scoresNumber->runAction(Sequence::create(delay, slide, nullptr));
		this->addChild(scoresNumber);
		score /= 10;
		i++;
	} while (score != 0);
}

void ScoreLayer::showPanel(){
	//slide panel
	auto panel = Sprite::createWithSpriteFrameName("score_panel.png");
	auto medal = Sprite::createWithSpriteFrameName("medals_2.png");
	medal->setPosition(54, 58);
	panel->addChild(medal);	
	panel->setPosition(visibleSize.x / 2, -90);
	auto slidepanel = MoveTo::create(1.0f, Vec2(visibleSize.x / 2, visibleSize.y / 2));
	auto slide = EaseExponentialOut::create(slidepanel);
	auto delay = DelayTime::create(1.0f);
	panel->runAction(Sequence::create(delay, slide, nullptr));
	this->addChild(panel);
}