//
// Created by cub3d on 01/12/18.
//

#include "WorldMapScene.h"
#include "HouseComponent.h"
#include "HouseScene.h"
#include <Font/TTFont.h>
#include "Data.h"
#include <algorithm>
#include "Timer.h"

#define MOVE_SPEED 25

//TODO: web build for final rel

//TODO: complete win scene
//TODO: people walk back and forth, if caught => loose, more suspicion => they move faster / are smarterdf
// TODO: add cupboards to hide in, one person per level other than ground

UK::Graphics::TTFont fnt("Assets/Fonts/Arimo-Regular.ttf", 16, UK::BLACK);

UK::Graphics::Image groundTexture("Assets/Ground.png");
UK::Graphics::Image playerImage("Assets/Player.png");
UK::Graphics::Image house("Assets/HouseComponent.png");

UK::KeyBind upK(SDLK_UP, "UP");
UK::KeyBind downK(SDLK_DOWN, "DOWN");
UK::KeyBind leftKy(SDLK_LEFT, "LEFT");
UK::KeyBind rightKy(SDLK_RIGHT, "RIGHT");

bool renderinit = false;

std::vector<HouseComponent> houses;

WorldMapScene::WorldMapScene() : Scene("WorldMap") {
    this->addObject(UK_LOAD_ENTITY_AT("Assets/Entity/Player.json", 10, 10));
    // Gravity is into screen
    this->world.SetGravity(b2Vec2(0, 0));

    // Gen houses
    for(int i = 0; i < 5; i++) {
        auto house = UK_LOAD_ENTITY("Assets/Entity/House.json");
        house->setPosition(static_cast<int>(RANDINT(2, 60)), static_cast<int>(RANDINT(2, 50)));
        house->getComponent<HouseComponent>()->houseID = i;
        this->addObject(house);

        UK_ADD_SCENE(std::make_shared<HouseScene>(i));

        this->contactManager.addListener(house, getObject<UK::Entity>("Player"), [i](auto ents, bool touching) {
            if(touching && (std::find(Data::getData().visitedHouses.begin(), Data::getData().visitedHouses.end(), i) == Data::getData().visitedHouses.end())) {
                // open new house
                std::string name = "House_" + std::to_string(ents.first->template getComponent<HouseComponent>()->houseID);

                Data::getData().visitedHouses.push_back(i);
                UK::getUnknown().globalSceneManager.loadScene(name);
            }
        });
    }
}

void WorldMapScene::update() {
    if(Data::getData().visitedHouses.size() == 5) {
        UK_LOAD_SCENE("End");
    }

    Scene::update();

    auto ent = getObject<UK::Entity>("Player");
    auto phys = ent->getComponent<UK::PhysicsBodyComponent>();

    if(upK.pressed()) {
        phys->applyForce(UK::Vector(0, -MOVE_SPEED));
    }

    if(downK.pressed()) {
        phys->applyForce(UK::Vector(0, MOVE_SPEED));
    }

    if(leftKy.pressed()) {
        phys->applyForce(UK::Vector(-MOVE_SPEED, 0));
    }

    if(rightKy.pressed()) {
        phys->applyForce(UK::Vector(MOVE_SPEED, 0));
    }
}

void WorldMapScene::render() const {
    if(!renderinit) {
        groundTexture.setDimentions(*UK::getUnknown().screenSize);
        renderinit = true;
    }
    groundTexture.render(0, 0);

    Scene::render();

    for(auto& i : Data::getData().visitedHouses) {
        for(auto& ii : getObjects<UK::Entity>("House")) {
            if(ii->getComponent<HouseComponent>()->houseID == i) {
                fnt.drawString("Already visited", ii->position.x * 16 - 50, ii->position.y * 16 + 40);
            }
        }
    }

    Timer::getTimer().render();
}
