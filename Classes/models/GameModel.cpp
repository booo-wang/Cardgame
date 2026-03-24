#include "GameModel.h"
#include <algorithm>

GameModel::GameModel()
{
}

GameModel::~GameModel()
{
    for (auto* card : _playfieldCards)
    {
        delete card;
    }
    _playfieldCards.clear();

    for (auto* card : _stackCards)
    {
        delete card;
    }
    _stackCards.clear();

    for (auto* card : _trayCards)
    {
        delete card;
    }
    _trayCards.clear();
}

const std::vector<CardModel*>& GameModel::getPlayfieldCards() const
{
    return _playfieldCards;
}

const std::vector<CardModel*>& GameModel::getStackCards() const
{
    return _stackCards;
}

const std::vector<CardModel*>& GameModel::getTrayCards() const
{
    return _trayCards;
}

CardModel* GameModel::getTrayTopCard() const
{
    if (_trayCards.empty())
    {
        return nullptr;
    }
    return _trayCards.back();
}

void GameModel::addTrayCard(CardModel* card)
{
    if (card != nullptr)
    {
        _trayCards.push_back(card);
    }
}

CardModel* GameModel::popTrayTopCard()
{
    if (_trayCards.empty())
    {
        return nullptr;
    }
    CardModel* card = _trayCards.back();
    _trayCards.pop_back();
    return card;
}

CardModel* GameModel::findPlayfieldCard(int id) const
{
    for (auto* card : _playfieldCards)
    {
        if (card->getId() == id)
        {
            return card;
        }
    }
    return nullptr;
}

void GameModel::removePlayfieldCard(int id)
{
    auto it = std::find_if(_playfieldCards.begin(), _playfieldCards.end(),
        [id](const CardModel* card) {
            return card->getId() == id;
        });

    if (it != _playfieldCards.end())
    {
        _playfieldCards.erase(it);
    }
}

bool GameModel::isPlayfieldEmpty() const
{
    return _playfieldCards.empty();
}

CardModel* GameModel::popStackTopCard()
{
    if (_stackCards.empty())
    {
        return nullptr;
    }
    CardModel* card = _stackCards.back();
    _stackCards.pop_back();
    return card;
}

int GameModel::getStackSize() const
{
    return static_cast<int>(_stackCards.size());
}

std::vector<CardModel*>& GameModel::playfieldCards()
{
    return _playfieldCards;
}

std::vector<CardModel*>& GameModel::stackCards()
{
    return _stackCards;
}

std::vector<CardModel*>& GameModel::trayCards()
{
    return _trayCards;
}

rapidjson::Value GameModel::serialize(rapidjson::Document::AllocatorType& allocator) const
{
    rapidjson::Value obj(rapidjson::kObjectType);

    rapidjson::Value playfieldArr(rapidjson::kArrayType);
    for (const auto* card : _playfieldCards)
    {
        playfieldArr.PushBack(card->serialize(allocator), allocator);
    }
    obj.AddMember("playfieldCards", playfieldArr, allocator);

    rapidjson::Value stackArr(rapidjson::kArrayType);
    for (const auto* card : _stackCards)
    {
        stackArr.PushBack(card->serialize(allocator), allocator);
    }
    obj.AddMember("stackCards", stackArr, allocator);

    rapidjson::Value trayArr(rapidjson::kArrayType);
    for (const auto* card : _trayCards)
    {
        trayArr.PushBack(card->serialize(allocator), allocator);
    }
    obj.AddMember("trayCards", trayArr, allocator);

    return obj;
}

GameModel* GameModel::deserialize(rapidjson::Value& value)
{
    GameModel* model = new GameModel();

    if (value.HasMember("playfieldCards"))
    {
        rapidjson::Value& arr = value["playfieldCards"];
        for (rapidjson::SizeType i = 0; i < arr.Size(); i++)
        {
            model->_playfieldCards.push_back(CardModel::deserialize(arr[i]));
        }
    }

    if (value.HasMember("stackCards"))
    {
        rapidjson::Value& arr = value["stackCards"];
        for (rapidjson::SizeType i = 0; i < arr.Size(); i++)
        {
            model->_stackCards.push_back(CardModel::deserialize(arr[i]));
        }
    }

    if (value.HasMember("trayCards"))
    {
        rapidjson::Value& arr = value["trayCards"];
        for (rapidjson::SizeType i = 0; i < arr.Size(); i++)
        {
            model->_trayCards.push_back(CardModel::deserialize(arr[i]));
        }
    }

    return model;
}
