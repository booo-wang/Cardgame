#include "CardUtils.h"

std::string CardUtils::faceToString(CardFaceType face) {
    switch (face) {
        case CFT_ACE:   return "A";
        case CFT_TWO:   return "2";
        case CFT_THREE: return "3";
        case CFT_FOUR:  return "4";
        case CFT_FIVE:  return "5";
        case CFT_SIX:   return "6";
        case CFT_SEVEN: return "7";
        case CFT_EIGHT: return "8";
        case CFT_NINE:  return "9";
        case CFT_TEN:   return "10";
        case CFT_JACK:  return "J";
        case CFT_QUEEN: return "Q";
        case CFT_KING:  return "K";
        default:        return "?";
    }
}

std::string CardUtils::suitToString(CardSuitType suit) {
    switch (suit) {
        case CST_CLUBS:    return "\xe2\x99\xa3"; // ♣
        case CST_DIAMONDS: return "\xe2\x99\xa6"; // ♦
        case CST_HEARTS:   return "\xe2\x99\xa5"; // ♥
        case CST_SPADES:   return "\xe2\x99\xa0"; // ♠
        default:           return "?";
    }
}

std::string CardUtils::cardToString(const CardModel* card) {
    if (card == nullptr) {
        return "?";
    }
    return suitToString(card->getSuit()) + faceToString(card->getFace());
}
