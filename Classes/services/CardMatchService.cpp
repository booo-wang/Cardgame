#include "CardMatchService.h"
#include <cstdlib>

bool CardMatchService::canMatch(CardFaceType face1, CardFaceType face2) {
    return std::abs(static_cast<int>(face1) - static_cast<int>(face2)) == 1;
}

bool CardMatchService::canMatchCards(const CardModel* card1, const CardModel* card2) {
    if (card1 == nullptr || card2 == nullptr) {
        return false;
    }
    return canMatch(card1->getFace(), card2->getFace());
}
