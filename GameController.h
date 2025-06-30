#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include "GameModel.h"
#include "ConsoleView.h"

class GameController {
public:
    GameController(GameModel& model, ConsoleView& view);
    void run();

private:
    GameModel& model;
    ConsoleView& view;

    void processInput();
};

#endif // GAMECONTROLLER_H
