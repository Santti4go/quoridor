#include "ConsoleView.h"
#include <iostream>
#include <vector>
#include <string>

void ConsoleView::clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void ConsoleView::render(const GameModel& model) {
    clearScreen();

    const int RENDER_SIZE = GameModel::BOARD_SIZE * 2;
    std::vector<std::string> grid(RENDER_SIZE, std::string(RENDER_SIZE, ' '));

    // 1. Draw grid spaces
    for (int y = 0; y < GameModel::BOARD_SIZE; ++y) {
        for (int x = 0; x < GameModel::BOARD_SIZE; ++x) {
            grid[y * 2][x * 2] = '.';
        }
    }

    // 2. Draw walls
    for (const auto& wall : model.getWalls()) {
        if (wall.orientation == Orientation::HORIZONTAL) {
            grid[wall.pos.y * 2 + 1][wall.pos.x * 2] = '=';
            grid[wall.pos.y * 2 + 1][wall.pos.x * 2 + 1] = '=';
            grid[wall.pos.y * 2 + 1][wall.pos.x * 2 + 2] = '=';
        } else { // VERTICAL
            grid[wall.pos.y * 2][wall.pos.x * 2 + 1] = '|';
            grid[wall.pos.y * 2 + 1][wall.pos.x * 2 + 1] = '|';
            grid[wall.pos.y * 2 + 2][wall.pos.x * 2 + 1] = '|';
        }
    }

    // 3. Draw pawns
    Point p1_pos = model.getPlayerPosition(PlayerID::PLAYER_ONE);
    Point p2_pos = model.getPlayerPosition(PlayerID::PLAYER_TWO);
    grid[p1_pos.y * 2][p1_pos.x * 2] = '1';
    grid[p2_pos.y * 2][p2_pos.x * 2] = '2';

    // 4. Print everything
    std::cout << "--- C++ Quoridor ---\n\n";
    
    // Print coordinate headers
    std::cout << "  ";
    for(int x=0; x < GameModel::BOARD_SIZE; ++x) std::cout << x << " ";
    std::cout << "\n";

    for (int y = 0; y < RENDER_SIZE - 1; ++y) {
        if(y % 2 == 0) std::cout << y/2 << " "; else std::cout << "  ";
        std::cout << grid[y] << "\n";
    }
    std::cout << "\n";
    
    // Print Player Info
    std::cout << "Player 1 (starts bottom, goal is row 0): " << model.getPlayerWallCount(PlayerID::PLAYER_ONE) << " walls left.\n";
    std::cout << "Player 2 (starts top,   goal is row 8): " << model.getPlayerWallCount(PlayerID::PLAYER_TWO) << " walls left.\n\n";

    // Print Turn Info
    auto winner = model.getWinner();
    if (winner.has_value()) {
        std::cout << ">>> GAME OVER! Player " << (*winner == PlayerID::PLAYER_ONE ? "1" : "2") << " wins! <<<\n";
    } else {
        std::cout << "Current Turn: Player " << (model.getCurrentPlayer() == PlayerID::PLAYER_ONE ? "1" : "2") << "\n";
    }
}
