#include <vector>
#include <iostream>

#include "map.hpp"


class Game {
public:
    enum class State {
        MENU,
        WANDERING,
        BATTLE
    };

    Game(Map& gameMap) : map(gameMap), gameState(State::MENU) {}

    void Draw(const std::vector<std::vector<char>>& grid) {
        switch (gameState) {
        case State::MENU:
            drawMenu();
            break;
        case State::WANDERING:
            drawWandering(grid);
            break;
        case State::BATTLE:
            drawBattle();
            break;
        }
    }

private:
    Map& map;
    State gameState;

    void drawMenu() {
        std::cout << "=== MENU ===\n";
    }

    void drawWandering(const std::vector<std::vector<char>>& grid) {
        for (const auto& row : grid) {
            for (char cell : row) {
                std::cout << cell << ' ';
            }
            std::cout << '\n';
        }
    }

    void drawBattle() {
        std::cout << "=== BATTLE ===\n";
    }
};
