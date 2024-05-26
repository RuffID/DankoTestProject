#include "GameScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* GameScene::createScene()
{
    auto scene = Scene::create();
    auto gameScene = new GameScene();

    gameScene->autorelease();

    if (gameScene->init())
    {
        scene->addChild(gameScene);
        return scene;
    }
    else
        CC_SAFE_DELETE(scene);

    return nullptr;
}

bool GameScene::init()
{
    if ( !Layer::init() )
        return false;

    isBusy = false;
    srand(utils::gettime());
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("blocks.plist");
    
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(Grid::onTouchBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

    return true;
}

void GameScene::newGame(cocos2d::Ref* ref)
{
    grid->removeAllBlocks();
    grid->generateRandomBlocks();

    addScore(-currentScore);
    gameOverLabel->setString("");
}

void GameScene::onEnter()
{
    Layer::onEnter();

    Size visibleSize = Director::getInstance()->getVisibleSize();
    auto background = cocos2d::LayerColor::create(Color4B(55, 76, 118, 255));
    addChild(background);

    grid = Grid::createGrid(MAX_ROWS, MAX_COLS);

    grid->setPosition(visibleSize.width * 0.5f - grid->getContentSize().width * 0.5f,
        visibleSize.height * 0.5f - grid->getContentSize().height * 0.5f + 95);
    
    addChild(grid);


    auto newGameButton = MenuItemSprite::create(Sprite::createWithSpriteFrameName("button_pressed.png"),
        Sprite::createWithSpriteFrameName("button.png"));
    newGameButton->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    newGameButton->setPosition(160, visibleSize.height + 130);

    auto newGameLabel = Label::createWithSystemFont(u8"Старт", FONT, 32);
    newGameLabel->enableShadow();
    newGameLabel->setPosition(newGameButton->getContentSize().width * 0.5f, newGameButton->getContentSize().height * 0.5f + 5);
    newGameButton->addChild(newGameLabel);
    newGameButton->setCallback(CC_CALLBACK_1(GameScene::newGame, this));

    auto menu = Menu::create(newGameButton, nullptr);
    menu->setPosition(Vec2::ZERO);
    addChild(menu);

    scoreLabel = Label::createWithSystemFont("", FONT, 32);
    scoreLabel->setColor(Color3B::WHITE);
    scoreLabel->enableShadow();
    scoreLabel->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    scoreLabel->setPosition(visibleSize.width - 20, visibleSize.height + 120);
    addChild(scoreLabel);

    gameOverLabel = Label::createWithSystemFont("", FONT, 32);
    gameOverLabel->enableShadow();
    gameOverLabel->setPosition(newGameButton->getPositionX() + 500, visibleSize.height + 100);
    addChild(gameOverLabel);

    newGame(this);
}

bool GameScene::onTouchBegan(Touch* touch, Event* event)
{
    Vec2 point = touch->getLocation();

    // Проверяет нет ли активных действий в game scene или в grid, а так же есть ли блок на нажатом месте
    if (isBusy || grid->isBusy() || grid->getBoundingBox().containsPoint(point) == false)
        return false;

    auto block = grid->getBlockForPosition(point);

    if (block == nullptr)
        return true;

    std::vector<Block*> matches;
    matches.push_back(block);
    int num_matches = checkForMatches(block, matches);

    // Если найдено больше требуемого количество блока для удаления, то удаляет их
    if (num_matches >= MATCHES_REQUIRED)
    {
        addScore(num_matches);
        removeAllMatchesBlock(block, matches);
    }

    // В конце хода проверяет остались ли ещё ходы, если нет - выводит на экран о завершении игры
    if (!grid->findAvalailableMoves())
        gameOverLabel->setString(u8"Игра окончена");

    return true;
}

int GameScene::checkForMatches(Block* pressedBlock, std::vector<Block*>& matches)
{
    return grid->findMatches(pressedBlock, matches);
}

void GameScene::removeAllMatchesBlock(Block* block, std::vector<Block*> matches)
{
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("pop.wav");

	for (auto match : matches)
		grid->removeBlockAt(match->gridPosition);

    // Небольшая задержка и блокировка нажатий чтобы не было коллизий
    lockTouches(0.7f);
    // Анимация падения блоков
	grid->fallingBlocks();
}

void GameScene::lockTouches(float time)
{
    unschedule(CC_SCHEDULE_SELECTOR(GameScene::unlockTouches));
    scheduleOnce(CC_SCHEDULE_SELECTOR(GameScene::unlockTouches), time);
    isBusy = true;
}

void GameScene::unlockTouches(float dt)
{
    isBusy = false;
}

void GameScene::addScore(int score)
{
    currentScore += score;
    char score_string[512];
    snprintf(score_string, 512, u8"Очки: %d", currentScore);
    scoreLabel->setString(score_string);
}