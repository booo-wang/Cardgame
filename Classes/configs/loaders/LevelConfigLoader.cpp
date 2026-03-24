#include "LevelConfigLoader.h"
#include "json/document.h"

LevelConfig* LevelConfigLoader::loadFromFile(const std::string& filePath)
{
    std::string fullPath = cocos2d::FileUtils::getInstance()->fullPathForFilename(filePath);
    std::string fileContent = cocos2d::FileUtils::getInstance()->getStringFromFile(fullPath);

    if (fileContent.empty())
    {
        cocos2d::log("LevelConfigLoader: Failed to read file: %s", filePath.c_str());
        return nullptr;
    }

    rapidjson::Document doc;
    doc.Parse(fileContent.c_str());

    if (doc.HasParseError())
    {
        cocos2d::log("LevelConfigLoader: JSON parse error in file: %s", filePath.c_str());
        return nullptr;
    }

    LevelConfig* config = new LevelConfig();

    // 解析playfield
    if (doc.HasMember("Playfield") && doc["Playfield"].IsArray())
    {
        const rapidjson::Value& playfieldArr = doc["Playfield"];
        for (rapidjson::SizeType i = 0; i < playfieldArr.Size(); i++)
        {
            const rapidjson::Value& cardObj = playfieldArr[i];

            CardConfig cardConfig;
            cardConfig.face = static_cast<CardFaceType>(cardObj["CardFace"].GetInt());
            cardConfig.suit = static_cast<CardSuitType>(cardObj["CardSuit"].GetInt());

            if (cardObj.HasMember("Position") && cardObj["Position"].IsObject())
            {
                const rapidjson::Value& posObj = cardObj["Position"];
                cardConfig.position.x = posObj["x"].GetFloat();
                cardConfig.position.y = posObj["y"].GetFloat();
            }
            else
            {
                cardConfig.position = cocos2d::Vec2::ZERO;
            }

            config->playfieldCards.push_back(cardConfig);
        }
    }

    // 解析stack
    if (doc.HasMember("Stack") && doc["Stack"].IsArray())
    {
        const rapidjson::Value& stackArr = doc["Stack"];
        for (rapidjson::SizeType i = 0; i < stackArr.Size(); i++)
        {
            const rapidjson::Value& cardObj = stackArr[i];

            CardConfig cardConfig;
            cardConfig.face = static_cast<CardFaceType>(cardObj["CardFace"].GetInt());
            cardConfig.suit = static_cast<CardSuitType>(cardObj["CardSuit"].GetInt());

            if (cardObj.HasMember("Position") && cardObj["Position"].IsObject())
            {
                const rapidjson::Value& posObj = cardObj["Position"];
                cardConfig.position.x = posObj["x"].GetFloat();
                cardConfig.position.y = posObj["y"].GetFloat();
            }
            else
            {
                cardConfig.position = cocos2d::Vec2::ZERO;
            }

            config->stackCards.push_back(cardConfig);
        }
    }

    return config;
}
