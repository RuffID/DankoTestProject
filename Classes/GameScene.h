#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include <cocos2d.h>
#include "Grid.h"

USING_NS_CC;

class GameScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

	virtual bool init();
	virtual void onEnter();

private:
    Grid* grid = nullptr;
    bool isBusy = false;
    cocos2d::Label* scoreLabel = nullptr;
    cocos2d::Label* gameOverLabel = nullptr;
    int currentScore = 0;

    GameScene() = default;
    ~GameScene() = default;
    GameScene(GameScene&) = default;

    void newGame(cocos2d::Ref* ref);
    int checkForMatches(Block* pressedBlock, std::vector<Block*>& matches);
    void removeAllMatchesBlock(Block* block, std::vector<Block*> matches);
    void addScore(int score);
    void lockTouches(float time);
    void unlockTouches(float dt);
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
};

#endif // __GAME_SCENE_H__
