#include "views/CardView.h"
#include "configs/models/CardResConfig.h"

USING_NS_CC;

CardView* CardView::create(CardModel* model)
{
    CardView* ret = new (std::nothrow) CardView();
    if (ret && ret->init(model))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool CardView::init(CardModel* model)
{
    if (!Node::init())
    {
        return false;
    }

    _cardModel = model;
    _isClickable = false;
    _onClickCallback = nullptr;

    std::string basePath = CardResConfig::getCardBasePath();
    _cardBase = Sprite::create(basePath);
    if (!_cardBase)
    {
        return false;
    }
    _cardBase->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _cardBase->setPosition(Vec2::ZERO);
    this->addChild(_cardBase, 0);

    Size cardSize = _cardBase->getContentSize();
    this->setContentSize(cardSize);

    // 大数字居中
    CardFaceType face = _cardModel->getFace();
    CardSuitType suit = _cardModel->getSuit();

    std::string bigNumPath = CardResConfig::getBigNumberPath(face, suit);
    _bigNumber = Sprite::create(bigNumPath);
    if (_bigNumber)
    {
        _bigNumber->setPosition(Vec2(cardSize.width * 0.5f, cardSize.height * 0.5f));
        this->addChild(_bigNumber, 1);
    }

    // 左上角小数字
    std::string smallNumPath = CardResConfig::getSmallNumberPath(face, suit);
    _smallNumber = Sprite::create(smallNumPath);
    if (_smallNumber)
    {
        _smallNumber->setPosition(Vec2(15.0f, cardSize.height - 30.0f));
        this->addChild(_smallNumber, 1);
    }

    // 花色图标（右上角）
    std::string suitPath = CardResConfig::getSuitPath(suit);
    _suitIcon = Sprite::create(suitPath);
    if (_suitIcon)
    {
        _suitIcon->setPosition(Vec2(cardSize.width - 20.0f, cardSize.height - 30.0f));
        this->addChild(_suitIcon, 1);
    }

    // 牌背面：蓝色花纹遮罩
    _cardBack = LayerColor::create(Color4B(30, 80, 140, 230), cardSize.width, cardSize.height);
    _cardBack->setPosition(Vec2::ZERO);
    this->addChild(_cardBack, 2);

    // 背面中心装饰菱形图案
    auto backDeco = DrawNode::create();
    float cx = cardSize.width * 0.5f;
    float cy = cardSize.height * 0.5f;
    float rx = cardSize.width * 0.25f;
    float ry = cardSize.height * 0.25f;
    Vec2 diamond[4] = {
        Vec2(cx, cy + ry), Vec2(cx + rx, cy),
        Vec2(cx, cy - ry), Vec2(cx - rx, cy)
    };
    backDeco->drawPolygon(diamond, 4, Color4F(0.15f, 0.4f, 0.7f, 1.0f), 2.0f, Color4F(0.8f, 0.85f, 0.9f, 0.8f));
    _cardBack->addChild(backDeco, 1);

    updateDisplay();

    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);

    listener->onTouchBegan = [this](Touch* touch, Event* event) -> bool {
        if (!_isClickable)
        {
            return false;
        }

        Vec2 locationInNode = this->convertToNodeSpace(touch->getLocation());
        Size size = this->getContentSize();
        Rect rect(0, 0, size.width, size.height);

        if (rect.containsPoint(locationInNode))
        {
            if (_onClickCallback && _cardModel)
            {
                _onClickCallback(_cardModel->getId());
            }
            return true;
        }
        return false;
    };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

void CardView::updateDisplay()
{
    if (!_cardModel)
    {
        return;
    }

    if (_cardModel->isFaceUp())
    {
        showFront();
    }
    else
    {
        showBack();
    }
}

void CardView::showFront()
{
    if (_bigNumber)   _bigNumber->setVisible(true);
    if (_smallNumber) _smallNumber->setVisible(true);
    if (_suitIcon)    _suitIcon->setVisible(true);
    if (_cardBack)    _cardBack->setVisible(false);
}

void CardView::showBack()
{
    if (_bigNumber)   _bigNumber->setVisible(false);
    if (_smallNumber) _smallNumber->setVisible(false);
    if (_suitIcon)    _suitIcon->setVisible(false);
    if (_cardBack)    _cardBack->setVisible(true);
}

void CardView::setClickable(bool clickable)
{
    _isClickable = clickable;
}

bool CardView::isClickable() const
{
    return _isClickable;
}

void CardView::setOnClickCallback(std::function<void(int cardId)> callback)
{
    _onClickCallback = callback;
}

CardModel* CardView::getCardModel() const
{
    return _cardModel;
}

void CardView::playFlipAnimation(std::function<void()> callback)
{
    // scaleX缩到0模拟翻转
    auto scaleDown = ScaleTo::create(0.15f, 0.0f, 1.0f);

    auto switchFace = CallFunc::create([this]() {
        if (_cardModel)
        {
            if (_cardModel->isFaceUp())
            {
                showFront();
            }
            else
            {
                showBack();
            }
        }
    });

    auto scaleUp = ScaleTo::create(0.15f, 1.0f, 1.0f);

    auto onComplete = CallFunc::create([callback]() {
        if (callback)
        {
            callback();
        }
    });

    auto sequence = Sequence::create(scaleDown, switchFace, scaleUp, onComplete, nullptr);
    this->runAction(sequence);
}
