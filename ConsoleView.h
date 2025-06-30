#ifndef CONSOLEVIEW_H
#define CONSOLEVIEW_H

#include "GameModel.h"

class ConsoleView {
public:
    void render(const GameModel& model);

private:
    void clearScreen();
};

#endif // CONSOLEVIEW_H
