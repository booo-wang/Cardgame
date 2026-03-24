#include "CardModel.h"
#include <algorithm>

int CardModel::_nextId = 0;

CardModel::CardModel()
    : _id(_nextId++)
    , _face(CFT_NONE)
    , _suit(CST_NONE)
    , _faceUp(false)
    , _position(cocos2d::Vec2::ZERO)
{
}

CardModel::CardModel(CardFaceType face, CardSuitType suit)
    : _id(_nextId++)
    , _face(face)
    , _suit(suit)
    , _faceUp(false)
    , _position(cocos2d::Vec2::ZERO)
{
}

CardModel::~CardModel()
{
}

int CardModel::getId() const
{
    return _id;
}

CardFaceType CardModel::getFace() const
{
    return _face;
}

CardSuitType CardModel::getSuit() const
{
    return _suit;
}

bool CardModel::isFaceUp() const
{
    return _faceUp;
}

cocos2d::Vec2 CardModel::getPosition() const
{
    return _position;
}

const std::vector<int>& CardModel::getCoveredByIds() const
{
    return _coveredByIds;
}

void CardModel::setFace(CardFaceType face)
{
    _face = face;
}

void CardModel::setSuit(CardSuitType suit)
{
    _suit = suit;
}

void CardModel::setFaceUp(bool faceUp)
{
    _faceUp = faceUp;
}

void CardModel::setPosition(const cocos2d::Vec2& position)
{
    _position = position;
}

void CardModel::addCoveredBy(int id)
{
    auto it = std::find(_coveredByIds.begin(), _coveredByIds.end(), id);
    if (it == _coveredByIds.end())
    {
        _coveredByIds.push_back(id);
    }
}

void CardModel::removeCoveredBy(int id)
{
    auto it = std::find(_coveredByIds.begin(), _coveredByIds.end(), id);
    if (it != _coveredByIds.end())
    {
        _coveredByIds.erase(it);
    }
}

bool CardModel::isCovered() const
{
    return !_coveredByIds.empty();
}

rapidjson::Value CardModel::serialize(rapidjson::Document::AllocatorType& allocator) const
{
    rapidjson::Value obj(rapidjson::kObjectType);

    obj.AddMember("id", _id, allocator);
    obj.AddMember("face", static_cast<int>(_face), allocator);
    obj.AddMember("suit", static_cast<int>(_suit), allocator);
    obj.AddMember("faceUp", _faceUp, allocator);

    rapidjson::Value posObj(rapidjson::kObjectType);
    posObj.AddMember("x", _position.x, allocator);
    posObj.AddMember("y", _position.y, allocator);
    obj.AddMember("position", posObj, allocator);

    rapidjson::Value coveredArr(rapidjson::kArrayType);
    for (int covId : _coveredByIds)
    {
        coveredArr.PushBack(covId, allocator);
    }
    obj.AddMember("coveredByIds", coveredArr, allocator);

    return obj;
}

CardModel* CardModel::deserialize(rapidjson::Value& value)
{
    CardModel* card = new CardModel();
    // 用存档里的id覆盖自动分配的
    card->_id = value["id"].GetInt();
    card->_face = static_cast<CardFaceType>(value["face"].GetInt());
    card->_suit = static_cast<CardSuitType>(value["suit"].GetInt());
    card->_faceUp = value["faceUp"].GetBool();

    if (value.HasMember("position"))
    {
        rapidjson::Value& posVal = value["position"];
        card->_position.x = posVal["x"].GetFloat();
        card->_position.y = posVal["y"].GetFloat();
    }

    if (value.HasMember("coveredByIds"))
    {
        rapidjson::Value& covArr = value["coveredByIds"];
        for (rapidjson::SizeType i = 0; i < covArr.Size(); i++)
        {
            card->_coveredByIds.push_back(covArr[i].GetInt());
        }
    }

    return card;
}

void CardModel::resetIdCounter()
{
    _nextId = 0;
}
