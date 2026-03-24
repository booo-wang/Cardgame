#include "PlayFieldController.h"

PlayFieldController::PlayFieldController()
    : _gameModel(nullptr)
    , _playFieldView(nullptr)
    , _onCardMatchCallback(nullptr) {
}

PlayFieldController::~PlayFieldController() {
    // 不持有model和view，不需要delete
}

void PlayFieldController::init(GameModel* model, PlayFieldView* view) {
    _gameModel = model;
    _playFieldView = view;
}

void PlayFieldController::setOnCardMatchCallback(std::function<void(int)> callback) {
    _onCardMatchCallback = callback;
}

bool PlayFieldController::handleCardClick(int cardId) {
    if (_gameModel == nullptr) {
        return false;
    }

    CardModel* card = _gameModel->findPlayfieldCard(cardId);
    if (card == nullptr) {
        return false;
    }

    if (!card->isFaceUp()) {
        return false;
    }

    if (card->isCovered()) {
        return false;
    }

    if (_onCardMatchCallback) {
        _onCardMatchCallback(cardId);
    }
    return true;
}
