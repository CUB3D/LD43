//
// Created by cub3d on 04/12/18.
//

#include "EndScene.h"
#include "Data.h"
#include <Font/TTFont.h>

UK::Graphics::TTFont fnt3("Assets/Fonts/Arimo-Regular.ttf", 16, UK::BLACK);
UK::Graphics::Image bk("Assets/Back.png");

EndScene::EndScene() : Scene("End") {

}

void EndScene::render() const {
    Scene::render();

    UK::getUnknown().globalSceneManager.scenes["WorldMap"]->render();
    //bk.render(0, 0);

    fnt3.drawString("You scored: " + std::to_string(Data::getData().food), 100, 100);
    if(Data::getData().caught) {
        fnt3.drawString("However, you failed to escape in time, better luck next time", 100, 200);
    } else {
        fnt3.drawString("And managed to escape without being seen", 100, 200);
    }
}
