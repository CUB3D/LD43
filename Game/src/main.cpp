#include <iostream>
#include <UK.h>

#include "WorldMapScene.h"
#include "EndScene.h"

int main() {
    UK_CREATE_WINDOW();

    UK_ADD_SCENE(std::make_shared<WorldMapScene>());
    UK_ADD_SCENE(std::make_shared<EndScene>());
    UK_LOAD_SCENE("WorldMap");

    UK_INIT_GAME();
    return 0;
}