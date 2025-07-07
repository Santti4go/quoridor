#ifndef GUICONTROLLER_H
#define GUICONTROLLER_H

#include "GameModel.h"
#include "GuiView.h"
#include <SFML/Graphics.hpp>
#include <optional>

class GuiController {
public:
    GuiController(GameModel& model);
    void run();

private:
    void handleInput();
    void update(); // For continuous logic like hover effects
    void render();

    // Input processing helpers
    Point pixelToBoardCoords(const sf::Vector2i& mousePos);
    Point pixelToWallCoords(const sf::Vector2i& mousePos);
    void calculateValidPawnMoves();

    GameModel& model;
    sf::RenderWindow window;
    GuiView view;

    // Controller state
    enum class Mode { PAWN_MOVE, WALL_PLACE };
    Mode currentMode;
    Orientation wallPlacementOrientation;
    
    // Visual feedback state
    std::optional<Wall> previewWall;
    std::vector<Point> validPawnMoves;
};

#endif // GUICONTROLLER_H
