#include "GuiController.h"
#include <iostream>

GuiController::GuiController(GameModel& model)
    : model(model),
      window(sf::VideoMode(700, 700), "C++ Quoridor"),
      view(window),
      currentMode(Mode::PAWN_MOVE),
      wallPlacementOrientation(Orientation::HORIZONTAL) {
    window.setFramerateLimit(60);
    if (!view.loadFont("DejaVuSans.ttf")) {
        // Handle error, maybe close the window
        window.close();
    }
}

void GuiController::run() {
    while (window.isOpen()) {
        handleInput();
        if (!model.getWinner().has_value()) {
            update();
        }
        render();
    }
}

void GuiController::handleInput() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        // --- Keyboard Input ---
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::M) {
                currentMode = (currentMode == Mode::PAWN_MOVE) ? Mode::WALL_PLACE : Mode::PAWN_MOVE;
            }
            if (event.key.code == sf::Keyboard::R && currentMode == Mode::WALL_PLACE) {
                wallPlacementOrientation = (wallPlacementOrientation == Orientation::HORIZONTAL) ? Orientation::VERTICAL : Orientation::HORIZONTAL;
            }
        }
        
        // --- Mouse Click Input ---
        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                if(model.getWinner().has_value()) continue;

                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                if (currentMode == Mode::PAWN_MOVE) {
                    Point target = pixelToBoardCoords(mousePos);
                    Point current = model.getPlayerPosition(model.getCurrentPlayer());
                    model.movePawn(target.x - current.x, target.y - current.y);
                } else { // WALL_PLACE mode
                    Point wallPos = pixelToWallCoords(mousePos);
                    Wall newWall = {wallPos, wallPlacementOrientation};
                    model.placeWall(newWall);
                }
            }
        }
    }
}

void GuiController::update() {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);

    if (currentMode == Mode::WALL_PLACE) {
        validPawnMoves.clear();
        Point wallPos = pixelToWallCoords(mousePos);
        previewWall = Wall{wallPos, wallPlacementOrientation};
    } else { // PAWN_MOVE mode
        previewWall.reset();
        calculateValidPawnMoves();
    }
}

void GuiController::render() {
    view.render(model, previewWall, validPawnMoves);
}

// --- Helper Functions ---

Point GuiController::pixelToBoardCoords(const sf::Vector2i& mousePos) {
    int x = static_cast<int>((mousePos.x - GuiView::BOARD_OFFSET_X) / GuiView::CELL_SIZE);
    int y = static_cast<int>((mousePos.y - GuiView::BOARD_OFFSET_Y) / GuiView::CELL_SIZE);
    return {x, y};
}

Point GuiController::pixelToWallCoords(const sf::Vector2i& mousePos) {
    // Walls are placed on intersections, so we find the nearest one
    int x = static_cast<int>((mousePos.x - GuiView::BOARD_OFFSET_X - GuiView::CELL_SIZE / 2.0f) / GuiView::CELL_SIZE);
    int y = static_cast<int>((mousePos.y - GuiView::BOARD_OFFSET_Y - GuiView::CELL_SIZE / 2.0f) / GuiView::CELL_SIZE);
    return {x, y};
}

void GuiController::calculateValidPawnMoves() {
    validPawnMoves.clear();
    Point current = model.getPlayerPosition(model.getCurrentPlayer());
    int dx[] = {0, 0, 1, -1};
    int dy[] = {1, -1, 0, 0};

    for(int i = 0; i < 4; ++i) {
        Point target = {current.x + dx[i], current.y + dy[i]};
        // We can't use the model's movePawn as it switches players.
        // We'd need a dedicated validator, or just live without highlighting.
        // For simplicity, let's just highlight adjacent squares for now.
        // A more robust solution would add a `isMoveValid` to the model.
         if (target.x >= 0 && target.x < GameModel::BOARD_SIZE && target.y >= 0 && target.y < GameModel::BOARD_SIZE) {
            // A full implementation would call a const validation method on the model here
            validPawnMoves.push_back(target);
        }
    }
}
