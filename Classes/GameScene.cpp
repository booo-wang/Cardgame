#include "GameScene.h"
#include "controllers/GameController.h"

USING_NS_CC;

Scene* GameScene::createScene()
{
    return GameScene::create();
}

bool GameScene::init()
{
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    // 天蓝色渐变背景（上深下浅）
    auto skyGradient = LayerGradient::create(
        Color4B(40, 120, 200, 255),   // 上方：深蓝
        Color4B(135, 200, 240, 255),  // 下方：浅蓝
        Vec2(0, -1)
    );
    skyGradient->setContentSize(visibleSize);
    this->addChild(skyGradient, -10);

    // 底部绿色草地区域
    auto grass = LayerColor::create(Color4B(60, 140, 50, 255),
        visibleSize.width, visibleSize.height * 0.28f);
    grass->setPosition(origin);
    this->addChild(grass, -9);

    // 牌区深棕色背景
    auto cardAreaBg = LayerColor::create(Color4B(50, 35, 25, 200),
        visibleSize.width, visibleSize.height * 0.68f);
    cardAreaBg->setPosition(Vec2(origin.x, origin.y + visibleSize.height * 0.28f));
    this->addChild(cardAreaBg, -8);

    // 底部操作栏半透明深色背景
    auto bottomBar = LayerColor::create(Color4B(35, 25, 18, 220),
        visibleSize.width, visibleSize.height * 0.28f);
    bottomBar->setPosition(origin);
    this->addChild(bottomBar, -7);

    _gameController = new GameController();
    _gameController->init(this);

    return true;
}

GameScene::~GameScene()
{
    delete _gameController;
    _gameController = nullptr;
}
