#include "GameModel.h"
#include <queue>
#include <algorithm>

GameModel::GameModel()
    : player1Pos{BOARD_SIZE / 2, BOARD_SIZE - 1},
      player2Pos{BOARD_SIZE / 2, 0},
      player1Walls(10),
      player2Walls(10),
      currentPlayer(PlayerID::PLAYER_ONE),
      winner(std::nullopt) {}

PlayerID GameModel::getCurrentPlayer() const { return currentPlayer; }
Point GameModel::getPlayerPosition(PlayerID player) const { return (player == PlayerID::PLAYER_ONE) ? player1Pos : player2Pos; }
int GameModel::getPlayerWallCount(PlayerID player) const { return (player == PlayerID::PLAYER_ONE) ? player1Walls : player2Walls; }
const std::vector<Wall>& GameModel::getWalls() const { return walls; }
std::optional<PlayerID> GameModel::getWinner() const { return winner; }

void GameModel::switchPlayer() {
    currentPlayer = (currentPlayer == PlayerID::PLAYER_ONE) ? PlayerID::PLAYER_TWO : PlayerID::PLAYER_ONE;
}

void GameModel::checkForWinner() {
    if (player1Pos.y == 0) {
        winner = PlayerID::PLAYER_ONE;
    } else if (player2Pos.y == BOARD_SIZE - 1) {
        winner = PlayerID::PLAYER_TWO;
    }
}

bool GameModel::movePawn(int dx, int dy) {
    if (winner.has_value()) return false;

    Point& currentPos = (currentPlayer == PlayerID::PLAYER_ONE) ? player1Pos : player2Pos;
    Point targetPos = {currentPos.x + dx, currentPos.y + dy};

    if (isPawnMoveValid(currentPos, targetPos)) {
        currentPos = targetPos;
        checkForWinner();
        switchPlayer();
        return true;
    }
    return false;
}

bool GameModel::placeWall(const Wall& wall) {
    if (winner.has_value()) return false;

    int& currentWalls = (currentPlayer == PlayerID::PLAYER_ONE) ? player1Walls : player2Walls;
    if (currentWalls <= 0) return false;

    if (isWallPlacementValid(wall)) {
        walls.push_back(wall);
        currentWalls--;
        switchPlayer();
        return true;
    }
    return false;
}

bool GameModel::isPawnMoveValid(const Point& from, const Point& to) const {
    if (to.x < 0 || to.x >= BOARD_SIZE || to.y < 0 || to.y >= BOARD_SIZE) return false;
    if (abs(from.x - to.x) + abs(from.y - to.y) != 1) return false;

    Point opponentPos = (currentPlayer == PlayerID::PLAYER_ONE) ? player2Pos : player1Pos;
    if (to == opponentPos) return false;

    if (crossesWall(from, to)) return false;

    return true;
}

// *** MODIFIED FUNCTION ***
bool GameModel::isWallPlacementValid(const Wall& newWall) const {
    // Boundary check
    if (newWall.pos.x < 0 || newWall.pos.x >= BOARD_SIZE - 1 || newWall.pos.y < 0 || newWall.pos.y >= BOARD_SIZE - 1) {
        return false;
    }

    // Overlap check
    for (const auto& existingWall : walls) {
        if (existingWall == newWall || existingWall.pos == newWall.pos) return false;
    }

    // "Golden Rule" check: Does it block all paths?
    // Create a temporary copy of the walls vector for the simulation.
    std::vector<Wall> simulatedWalls = this->walls;
    simulatedWalls.push_back(newWall);

    // Check paths using the simulated wall set.
    bool p1Path = pathExists(PlayerID::PLAYER_ONE, simulatedWalls);
    bool p2Path = pathExists(PlayerID::PLAYER_TWO, simulatedWalls);

    return p1Path && p2Path;
}

// *** NEW Public-facing wrappers ***
bool GameModel::pathExists(PlayerID player) const {
    return pathExists(player, this->walls);
}

bool GameModel::crossesWall(const Point& from, const Point& to) const {
    return crossesWall(from, to, this->walls);
}


// *** MODIFIED Helper with vector parameter ***
bool GameModel::pathExists(PlayerID player, const std::vector<Wall>& currentWalls) const {
    Point startPos = getPlayerPosition(player);
    int goalY = (player == PlayerID::PLAYER_ONE) ? 0 : BOARD_SIZE - 1;

    std::queue<Point> q;
    q.push(startPos);
    std::vector<std::vector<bool>> visited(BOARD_SIZE, std::vector<bool>(BOARD_SIZE, false));
    visited[startPos.y][startPos.x] = true;

    while (!q.empty()) {
        Point current = q.front();
        q.pop();

        if (current.y == goalY) return true;

        int dx[] = {0, 0, 1, -1};
        int dy[] = {1, -1, 0, 0};

        for (int i = 0; i < 4; ++i) {
            Point next = {current.x + dx[i], current.y + dy[i]};
            if (next.x >= 0 && next.x < BOARD_SIZE && next.y >= 0 && next.y < BOARD_SIZE &&
                !visited[next.y][next.x] && !crossesWall(current, next, currentWalls)) {
                visited[next.y][next.x] = true;
                q.push(next);
            }
        }
    }
    return false;
}

// *** MODIFIED Helper with vector parameter ***
bool GameModel::crossesWall(const Point& from, const Point& to, const std::vector<Wall>& currentWalls) const {
    for (const auto& wall : currentWalls) {
        if (wall.orientation == Orientation::HORIZONTAL) {
            if ((from.y == wall.pos.y && to.y == wall.pos.y + 1) || (from.y == wall.pos.y + 1 && to.y == wall.pos.y)) {
                if (from.x == wall.pos.x || from.x == wall.pos.x + 1) return true;
            }
        } else { // VERTICAL
            if ((from.x == wall.pos.x && to.x == wall.pos.x + 1) || (from.x == wall.pos.x + 1 && to.x == wall.pos.x)) {
                if (from.y == wall.pos.y || from.y == wall.pos.y + 1) return true;
            }
        }
    }
    return false;
}
