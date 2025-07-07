#ifndef GUIVIEW_H
#define GUIVIEW_H

#include <SFML/Graphics.hpp>
#include "GameModel.h"
#include <optional>
#include <vector>

class GuiView {
public:
    GuiView(sf::RenderWindow& window);
    bool loadFont(const std::string& fontPath);

    void render(const GameModel& model, 
                const std::optional<Wall>& previewWall,
                const std::vector<Point>& validPawnMoves);

    // Drawing constants
    static constexpr float CELL_SIZE = 60.0f;
    static constexpr float WALL_THICKNESS = 10.0f;
    static constexpr float BOARD_OFFSET_X = 50.0f;
    static constexpr float BOARD_OFFSET_Y = 50.0f;

private:
    void drawBoard();
    void drawPawns(const GameModel& model);
    void drawWalls(const GameModel& model);
    void drawUI(const GameModel& model);
    void drawPreviewWall(const std::optional<Wall>& wall);
    void drawValidMoves(const std::vector<Point>& moves);

    sf::RenderWindow& window;
    sf::Font font;

};

#endif // GUIVIEW_H
