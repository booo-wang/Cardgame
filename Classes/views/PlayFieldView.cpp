#include "views/PlayFieldView.h"

USING_NS_CC;

PlayFieldView* PlayFieldView::create(const std::vector<CardModel*>& cards)
{
    PlayFieldView* ret = new (std::nothrow) PlayFieldView();
    if (ret && ret->init(cards))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool PlayFieldView::init(const std::vector<CardModel*>& cards)
{
    if (!Node::init())
    {
        return false;
    }

    _onCardClickCallback = nullptr;

    for (auto* cardModel : cards)
    {
        if (!cardModel)
        {
            continue;
        }

        CardView* cardView = CardView::create(cardModel);
        if (!cardView)
        {
            continue;
        }

        cardView->setPosition(cardModel->getPosition());

        // 转发点击事件
        cardView->setOnClickCallback([this](int cardId) {
            if (_onCardClickCallback)
            {
                _onCardClickCallback(cardId);
            }
        });

        // 正面朝上且没被盖住才能点
        if (cardModel->isFaceUp() && !cardModel->isCovered())
        {
            cardView->setClickable(true);
        }
        else
        {
            cardView->setClickable(false);
        }

        this->addChild(cardView);
        _cardViews[cardModel->getId()] = cardView;
    }

    return true;
}

void PlayFieldView::setOnCardClickCallback(std::function<void(int cardId)> callback)
{
    _onCardClickCallback = callback;

    for (auto& pair : _cardViews)
    {
        CardView* cardView = pair.second;
        if (cardView)
        {
            cardView->setOnClickCallback([this](int cardId) {
                if (_onCardClickCallback)
                {
                    _onCardClickCallback(cardId);
                }
            });
        }
    }
}

CardView* PlayFieldView::getCardView(int cardId)
{
    auto it = _cardViews.find(cardId);
    if (it != _cardViews.end())
    {
        return it->second;
    }
    return nullptr;
}

void PlayFieldView::removeCardView(int cardId)
{
    auto it = _cardViews.find(cardId);
    if (it != _cardViews.end())
    {
        CardView* cardView = it->second;
        if (cardView)
        {
            cardView->removeFromParent();
        }
        _cardViews.erase(it);
    }
}

void PlayFieldView::detachCardView(int cardId)
{
    _cardViews.erase(cardId);
}

void PlayFieldView::flipCard(int cardId)
{
    auto it = _cardViews.find(cardId);
    if (it != _cardViews.end())
    {
        CardView* cardView = it->second;
        if (cardView)
        {
            CardModel* model = cardView->getCardModel();
            bool shouldBeClickable = model && model->isFaceUp() && !model->isCovered();
            cardView->playFlipAnimation([cardView, shouldBeClickable]() {
                if (cardView->getCardModel() && cardView->getCardModel()->isFaceUp())
                {
                    cardView->showFront();
                }
                else
                {
                    cardView->showBack();
                }
                cardView->setClickable(shouldBeClickable);
            });
        }
    }
}

void PlayFieldView::addCardView(int cardId, CardView* cardView)
{
    if (cardView)
    {
        _cardViews[cardId] = cardView;
    }
}
