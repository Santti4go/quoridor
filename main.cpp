#include "GameModel.h"
#include "ConsoleView.h"
#include "GameController.h"

int main() {
    GameModel model;
    ConsoleView view;
    GameController controller(model, view);

    controller.run();

    return 0;
}
