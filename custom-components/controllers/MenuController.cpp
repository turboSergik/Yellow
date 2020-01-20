//
// Created by Олег Бобров on 20.01.2020.
//

#include "MenuController.hpp"

void MenuController::start() {
    Network::onGamesResponse.addListener<MenuController, &MenuController::onGames>(this);
    Network::send(Action::GAMES);
    //TODO: instantiate UI here
}

void MenuController::onDestroy() {
    Network::onGamesResponse.removeListener<MenuController, &MenuController::onGames>(this);
    for (auto & child : *transform) {
        child->gameObject->destroy();
    }
}

void MenuController::onGames(const nlohmann::json & json) {
    //std::cout << json.dump(4) << std::endl;
    Game::current->setState(GameState::INIT);
}
