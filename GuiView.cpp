#include "GuiView.h"
#include <iostream>

GuiView::GuiView(sf::RenderWindow& window) : window(window) {}

bool GuiView::loadFont(const std::string& fontPath) {
    if (!font.loadFromFile(fontPath)) {
        std::cerr << "Error: Could not load font from " << fontPath << std::endl;
        return false;
    }
    return true;
}

void GuiView::render(const GameModel& model, const std::optional<Wall>& previewWall, const std::vector<Point>& validPawnMoves) {
    window.clear(sf::Color(80, 80, 80)); // Dark grey background

    drawBoard();
    drawValidMoves(validPawnMoves);
    drawWalls(model);
    drawPreviewWall(previewWall);
    drawPawns(model);
    drawUI(model);
    
    window.display();
}

void GuiView::drawBoard() {
    sf::RectangleShape cell(sf::Vector2f(CELL_SIZE, CELL_SIZE));
    for (int y = 0; y < GameModel::BOARD_SIZE; ++y) {
        for (int x = 0; x < GameModel::BOARD_SIZE; ++x) {
            cell.setPosition(BOARD_OFFSET_X + x * CELL_SIZE, BOARD_OFFSET_Y + y * CELL_SIZE);
            cell.setFillColor(sf::Color(210, 180, 140)); // Tan color
            cell.setOutlineThickness(1.0f);
            cell.setOutlineColor(sf::Color::Black);
            window.draw(cell);
        }
    }
}

void GuiView::drawPawns(const GameModel& model) {
    sf::CircleShape pawn(CELL_SIZE / 2.0f - 5.0f);
    pawn.setOutlineColor(sf::Color::Black);
    pawn.setOutlineThickness(2.0f);

    // Player 1
    Point p1_pos = model.getPlayerPosition(PlayerID::PLAYER_ONE);
    pawn.setFillColor(sf::Color::Blue);
    pawn.setPosition(BOARD_OFFSET_X + p1_pos.x * CELL_SIZE + 5.0f, BOARD_OFFSET_Y + p1_pos.y * CELL_SIZE + 5.0f);
    window.draw(pawn);

    // Player 2
    Point p2_pos = model.getPlayerPosition(PlayerID::PLAYER_TWO);
    pawn.setFillColor(sf::Color::Red);
    pawn.setPosition(BOARD_OFFSET_X + p2_pos.x * CELL_SIZE + 5.0f, BOARD_OFFSET_Y + p2_pos.y * CELL_SIZE + 5.0f);
    window.draw(pawn);
}

void GuiView::drawWalls(const GameModel& model) {
    sf::RectangleShape wallShape;
    wallShape.setFillColor(sf::Color(139, 69, 19)); // Brown
    for (const auto& wall : model.getWalls()) {
        if (wall.orientation == Orientation::HORIZONTAL) {
            wallShape.setSize(sf::Vector2f(CELL_SIZE * 2 - WALL_THICKNESS, WALL_THICKNESS));
            wallShape.setPosition(
                BOARD_OFFSET_X + wall.pos.x * CELL_SIZE + WALL_THICKNESS / 2.0f,
                BOARD_OFFSET_Y + (wall.pos.y + 1) * CELL_SIZE - WALL_THICKNESS / 2.0f
            );
        } else { // VERTICAL
            wallShape.setSize(sf::Vector2f(WALL_THICKNESS, CELL_SIZE * 2 - WALL_THICKNESS));
            wallShape.setPosition(
                BOARD_OFFSET_X + (wall.pos.x + 1) * CELL_SIZE - WALL_THICKNESS / 2.0f,
                BOARD_OFFSET_Y + wall.pos.y * CELL_SIZE + WALL_THICKNESS / 2.0f
            );
        }
        window.draw(wallShape);
    }
}

void GuiView::drawPreviewWall(const std::optional<Wall>& wall) {
    if (!wall.has_value()) return;

    sf::RectangleShape wallShape;
    wallShape.setFillColor(sf::Color(139, 69, 19, 128)); // Semi-transparent brown
    if (wall->orientation == Orientation::HORIZONTAL) {
        wallShape.setSize(sf::Vector2f(CELL_SIZE * 2 - WALL_THICKNESS, WALL_THICKNESS));
        wallShape.setPosition(
            BOARD_OFFSET_X + wall->pos.x * CELL_SIZE + WALL_THICKNESS / 2.0f,
            BOARD_OFFSET_Y + (wall->pos.y + 1) * CELL_SIZE - WALL_THICKNESS / 2.0f
        );
    } else { // VERTICAL
        wallShape.setSize(sf::Vector2f(WALL_THICKNESS, CELL_SIZE * 2 - WALL_THICKNESS));
        wallShape.setPosition(
            BOARD_OFFSET_X + (wall->pos.x + 1) * CELL_SIZE - WALL_THICKNESS / 2.0f,
            BOARD_OFFSET_Y + wall->pos.y * CELL_SIZE + WALL_THICKNESS / 2.0f
        );
    }
    window.draw(wallShape);
}

void GuiView::drawValidMoves(const std::vector<Point>& moves) {
    sf::CircleShape highlight(CELL_SIZE / 4.0f);
    highlight.setFillColor(sf::Color(0, 255, 0, 128)); // Semi-transparent green
    for(const auto& move : moves) {
        highlight.setPosition(
            BOARD_OFFSET_X + move.x * CELL_SIZE + CELL_SIZE / 4.0f,
            BOARD_OFFSET_Y + move.y * CELL_SIZE + CELL_SIZE / 4.0f
        );
        window.draw(highlight);
    }
}

void GuiView::drawUI(const GameModel& model) {
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::White);

    std::string playerTurn = "Turn: Player " + std::string(model.getCurrentPlayer() == PlayerID::PLAYER_ONE ? "1 (Blue)" : "2 (Red)");
    text.setString(playerTurn);
    text.setPosition(BOARD_OFFSET_X, 10.0f);
    window.draw(text);
    
    std::string p1Walls = "P1 Walls: " + std::to_string(model.getPlayerWallCount(PlayerID::PLAYER_ONE));
    text.setString(p1Walls);
    text.setPosition(BOARD_OFFSET_X + 400, 10.0f);
    window.draw(text);

    std::string p2Walls = "P2 Walls: " + std::to_string(model.getPlayerWallCount(PlayerID::PLAYER_TWO));
    text.setString(p2Walls);
    text.setPosition(BOARD_OFFSET_X + 400, 35.0f);
    window.draw(text);
    
    auto winner = model.getWinner();
    if (winner.has_value()) {
        text.setString("Player " + std::string(*winner == PlayerID::PLAYER_ONE ? "1" : "2") + " WINS!");
        text.setCharacterSize(60);
        text.setFillColor(sf::Color::Yellow);
        text.setStyle(sf::Text::Bold);
        // Center text
        sf::FloatRect textRect = text.getLocalBounds();
        text.setOrigin(textRect.left + textRect.width/2.0f, textRect.top + textRect.height/2.0f);
        text.setPosition(sf::Vector2f(window.getSize().x/2.0f, window.getSize().y/2.0f));
        window.draw(text);
    }
}
