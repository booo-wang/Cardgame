#include "views/TrayView.h"
#include "configs/models/CardResConfig.h"

USING_NS_CC;

TrayView* TrayView::create()
{
    TrayView* ret = new (std::nothrow) TrayView();
    if (ret && ret->init())
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool TrayView::init()
{
    if (!Node::init())
    {
        return false;
    }

    _topCardView = nullptr;

    // 空位占位符，用牌底图半透明显示
    std::string basePath = CardResConfig::getCardBasePath();
    _placeholder = Sprite::create(basePath);
    if (_placeholder)
    {
        _placeholder->setOpacity(80);
        _placeholder->setPosition(Vec2::ZERO);
        this->addChild(_placeholder, 0);

        this->setContentSize(_placeholder->getContentSize());
    }

    return true;
}

void TrayView::setTopCard(CardModel* card)
{
    if (_topCardView)
    {
        _topCardView->removeFromParent();
        _topCardView = nullptr;
    }

    if (!card)
    {
        if (_placeholder)
        {
            _placeholder->setVisible(true);
        }
        return;
    }

    if (_placeholder)
    {
        _placeholder->setVisible(false);
    }

    _topCardView = CardView::create(card);
    if (_topCardView)
    {
        // 居中对齐到tray锚点
        Size cardSize = _topCardView->getContentSize();
        _topCardView->setPosition(Vec2(-cardSize.width * 0.5f, -cardSize.height * 0.5f));
        _topCardView->showFront();
        this->addChild(_topCardView, 1);
    }
}

CardView* TrayView::getTopCardView() const
{
    return _topCardView;
}

cocos2d::Vec2 TrayView::getTopCardPosition() const
{
    return this->getPosition();
}
