#include "views/StackView.h"
#include "configs/models/CardResConfig.h"

USING_NS_CC;

StackView* StackView::create(int count)
{
    StackView* ret = new (std::nothrow) StackView();
    if (ret && ret->init(count))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool StackView::init(int count)
{
    if (!Node::init())
    {
        return false;
    }

    _remainingCount = count;
    _onClickCallback = nullptr;

    std::string basePath = CardResConfig::getCardBasePath();
    _stackSprite = Sprite::create(basePath);
    if (_stackSprite)
    {
        _stackSprite->setPosition(Vec2::ZERO);
        this->addChild(_stackSprite, 0);

        // 蓝色牌背
        _stackSprite->setColor(Color3B(30, 80, 140));

        this->setContentSize(_stackSprite->getContentSize());
    }

    _countLabel = Label::createWithSystemFont(std::to_string(_remainingCount), "Arial", 36);
    if (_countLabel)
    {
        _countLabel->setPosition(Vec2::ZERO);
        _countLabel->setTextColor(Color4B::WHITE);
        _countLabel->enableOutline(Color4B(0, 0, 0, 200), 2);
        this->addChild(_countLabel, 1);
    }

    updateCount(_remainingCount);

    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);

    listener->onTouchBegan = [this](Touch* touch, Event* event) -> bool {
        if (!_stackSprite || !_stackSprite->isVisible())
        {
            return false;
        }

        Vec2 locationInNode = _stackSprite->convertToNodeSpace(touch->getLocation());
        Size size = _stackSprite->getContentSize();
        Rect rect(0, 0, size.width, size.height);

        if (rect.containsPoint(locationInNode))
        {
            if (_onClickCallback)
            {
                _onClickCallback();
            }
            return true;
        }
        return false;
    };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

void StackView::updateCount(int count)
{
    _remainingCount = count;

    if (_countLabel)
    {
        _countLabel->setString(std::to_string(_remainingCount));
    }

    // 没牌了就隐藏
    bool hasCards = (_remainingCount > 0);
    if (_stackSprite)
    {
        _stackSprite->setVisible(hasCards);
    }
    if (_countLabel)
    {
        _countLabel->setVisible(hasCards);
    }
}

void StackView::setOnClickCallback(std::function<void()> callback)
{
    _onClickCallback = callback;
}
