#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include <vector>
#include <optional>

// --- Data Structures ---

struct Point {
    int x, y;
    bool operator==(const Point& other) const { return x == other.x && y == other.y; }
};

enum class Orientation { HORIZONTAL, VERTICAL };
enum class PlayerID { PLAYER_ONE, PLAYER_TWO };

struct Wall {
    Point pos;
    Orientation orientation;
    bool operator==(const Wall& other) const { return pos == other.pos && orientation == other.orientation; }
};

class GameModel {
public:
    static constexpr int BOARD_SIZE = 9;

    GameModel();

    // --- Getters for View ---
    PlayerID getCurrentPlayer() const;
    Point getPlayerPosition(PlayerID player) const;
    int getPlayerWallCount(PlayerID player) const;
    const std::vector<Wall>& getWalls() const;
    std::optional<PlayerID> getWinner() const;

    // --- Game Actions (called by Controller) ---
    bool movePawn(int dx, int dy);
    bool placeWall(const Wall& wall);

private:
    // --- Internal State ---
    Point player1Pos;
    Point player2Pos;
    int player1Walls;
    int player2Walls;
    PlayerID currentPlayer;
    std::vector<Wall> walls;
    std::optional<PlayerID> winner;

    // --- Game Logic and Validation ---
    void switchPlayer();
    bool isPawnMoveValid(const Point& from, const Point& to) const;
    bool isWallPlacementValid(const Wall& newWall) const;
    void checkForWinner();

    // --- Pathfinding helpers ---
    // Public-facing versions use the model's state
    bool pathExists(PlayerID player) const;
    bool crossesWall(const Point& from, const Point& to) const;
    
    // Private overloads for simulation with a hypothetical set of walls
    bool pathExists(PlayerID player, const std::vector<Wall>& currentWalls) const;
    bool crossesWall(const Point& from, const Point& to, const std::vector<Wall>& currentWalls) const;
};

#endif // GAMEMODEL_H
