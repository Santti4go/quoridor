#include "GameController.h"
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

GameController::GameController(GameModel& model, ConsoleView& view)
    : model(model), view(view) {}

void GameController::run() {
    while (!model.getWinner().has_value()) {
        view.render(model);
        processInput();
    }
    // Final render to show the winning state
    view.render(model);
}

void GameController::processInput() {
    bool moveSuccess = false;
    while (!moveSuccess) {
        std::cout << "Enter command (e.g., 'move u', 'wall 3 4 h'): ";
        std::string line;
        std::getline(std::cin, line);

        std::stringstream ss(line);
        std::string command;
        ss >> command;

        if (command == "move") {
            char dir;
            ss >> dir;
            int dx = 0, dy = 0;
            if (dir == 'u' || dir == 'w') dy = -1;
            else if (dir == 'd' || dir == 's') dy = 1;
            else if (dir == 'l' || dir == 'a') dx = -1;
            else if (dir == 'r' || dir == 'd') dx = 1;
            else {
                std::cout << "Invalid move direction. Use u(p), d(own), l(eft), r(ight) or w,a,s,d.\n";
                continue;
            }
            moveSuccess = model.movePawn(dx, dy);
            if (!moveSuccess) std::cout << "Invalid move. Try again.\n";

        } else if (command == "wall") {
            int x, y;
            char orientationChar;
            ss >> x >> y >> orientationChar;

            if (ss.fail()) {
                std::cout << "Invalid wall format. Use 'wall x y [h|v]'.\n";
                continue;
            }

            Wall wall;
            wall.pos = {x, y};
            if (orientationChar == 'h') {
                wall.orientation = Orientation::HORIZONTAL;
            } else if (orientationChar == 'v') {
                wall.orientation = Orientation::VERTICAL;
            } else {
                std::cout << "Invalid wall orientation. Use 'h' or 'v'.\n";
                continue;
            }

            moveSuccess = model.placeWall(wall);
            if (!moveSuccess) std::cout << "Invalid wall placement. Try again.\n";

        } else {
            std::cout << "Unknown command.\n";
        }
    }
}
