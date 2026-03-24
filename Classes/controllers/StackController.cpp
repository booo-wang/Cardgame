#include "StackController.h"

StackController::StackController()
    : _gameModel(nullptr)
    , _stackView(nullptr)
    , _onStackDrawCallback(nullptr) {
}

StackController::~StackController() {
}

void StackController::init(GameModel* model, StackView* view) {
    _gameModel = model;
    _stackView = view;
}

void StackController::setOnStackDrawCallback(std::function<void()> callback) {
    _onStackDrawCallback = callback;
}

bool StackController::handleStackClick() {
    if (_gameModel == nullptr) {
        return false;
    }

    if (_gameModel->getStackSize() <= 0) {
        return false;
    }

    if (_onStackDrawCallback) {
        _onStackDrawCallback();
    }
    return true;
}
