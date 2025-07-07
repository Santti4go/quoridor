#include "GameModel.h"

#define GUI_VERSION

#ifndef GUI_VERSION     /* CLI version */
#include "ConsoleView.h"
#include "GameController.h"
#endif

#ifdef GUI_VERSION      /* GUI version */
#include "GuiView.h"
#include "GuiController.h"
#endif

int main() {
    GameModel model;

    #ifndef GUI_VERSION
    // Only required for CLI version
    ConsoleView view;
    GameController controller(model, view);
    #endif

    #ifdef GUI_VERSION
    GuiController controller(model);
    #endif

    controller.run();

    return 0;
}
