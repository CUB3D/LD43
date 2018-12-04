//
// Created by cub3d on 01/12/18.
//

#include "HouseScene.h"
#include <Font/TTFont.h>
#include "Data.h"
#include "Timer.h"

FileShader fs("Assets/Shader/FBO_vert.glsl", "Assets/Shader/FBO_Frag.glsl");
UK::Graphics::Image torchMap("Assets/TorchMap.png");
UK::Graphics::Image dark("Assets/LightMapDark.png");
UK::Graphics::TTFont fnt2("Assets/Fonts/Arimo-Regular.ttf", 16, UK::BLACK);

#define MOVE_SPEED 5 * 9.8

#define SUSPICION_WALKING 0.001
#define SUSPICION_JUMP_LAND 1
#define SUSPICION_PASSIVE_DECAY 0.0001

UK::KeyBind jmp(SDLK_UP, "UP");
UK::KeyBind leftK(SDLK_LEFT, "LEFT");
UK::KeyBind rightK(SDLK_RIGHT, "RIGHT");

UK::Timer t(0.2f);
UK::KeyBind interact(SDLK_SPACE, "interact");

HouseScene::HouseScene(int houseID) : Scene("House_"+std::to_string(houseID)), houseID(houseID), fb(*UK::getUnknown().screenSize), escape(false), suspicion(0) {
    t.isTickComplete();
    dark.setDimentions(*UK::getUnknown().screenSize);

    int w = 3,h = 4;

    for(int y = 0; y < h; y++) {
        for(int x = 0; x < w; x++) {
            auto blk = std::make_shared<HouseBlock>();
            blk->setPosition(x * 512, 760 - 512 * y);
            this->addObject(blk);
        }

        this->addObject(UK_LOAD_ENTITY_AT("Assets/Entity/HouseGround.json", 0, 45.5 - 32 * y));
    }

    this->addObject(UK_LOAD_ENTITY_AT("Assets/Entity/Door.json", 10, 36.5));

    auto player = UK_LOAD_ENTITY_AT("Assets/Entity/Player.json", 40, 40);
    this->addObject(player);


    for(auto& g : getObjects<UK::Entity>("Ground")) {
        this->contactManager.addListener(player, g, [this](auto pair, bool onGround) {
            if (onGround) {
                jmp.enabled = true;
                suspicion += SUSPICION_JUMP_LAND;
            }
        });
    }

    for(auto& a : this->getObjects<UK::Entity>("HouseBlock")) {
        if(RANDINT(0, 1)) {
            auto lamp = UK_LOAD_ENTITY_AT("Assets/Entity/Lamp.json", (RANDINT(0, 500) + a->position.x) / 16, (a->position.y - 64 - 16) / 16);

            this->contactManager.addListener(lamp, player, [&](auto pair, bool firstTouch) {
                if(RANDINT(0, 10) < 3) { // 30% chance
                    suspicion += 0.3;
                }
            });

            this->addObject(lamp);
        }
    }

    for(int y = 0; y < h-2; y++) {
        int x = 1;

        for(auto& a : this->getObjects<UK::Entity>("HouseBlock")) {
            if(a->position.x / 512 == x && a->position.y == 760 - 512 * y) {
                auto stair = UK_LOAD_ENTITY_AT("Assets/Entity/Stair.json", a->position.x / 16,
                                               a->position.y / 16 - 11.5);
                this->addObject(stair);
            }
        }
    }

    this->addObject(UK_LOAD_ENTITY_AT("Assets/Entity/Other.json", RANDINT(55, 85), 11));
}

void showDialog(const std::string& str) {
    fnt2.drawString(str, UK::getUnknown().screenSize->width / 2, 100);
}

void HouseScene::update() {
    if(!fb.created) {
        fb.createFBO();
        fs.compile();
        fs.bind();
        fs.setFloat("fboTexture", 0);
        fs.setFloat("lightMap", 1);
        fs.unbind();
    }

    Scene::update();

    auto ent = getObject<UK::Entity>("Player");
    auto phys = ent->getComponent<UK::PhysicsBodyComponent>();

    if(jmp.pressed()) {
        phys->applyForce(UK::Vector(0, -MOVE_SPEED * 10));
        jmp.enabled = false;
    }

    if(leftK.pressed()) {
        phys->applyForce(UK::Vector(-MOVE_SPEED, 0));
        suspicion += SUSPICION_WALKING;
    }

    if(rightK.pressed()) {
        phys->applyForce(UK::Vector(MOVE_SPEED, 0));
        suspicion += SUSPICION_WALKING;
    }

    if(suspicion > 0)
        suspicion -= SUSPICION_PASSIVE_DECAY;

    if(ent->position.x <= 0.5)
        ent->setPosition(0.5, ent->position.y);

    if(ent->position.x >= 90)
        ent->setPosition(90, ent->position.y);

    cam.setPosition(ent->position.x * 16 - cam.area.w / 2, ent->position.y * 16 - cam.area.h / 2, cam.area.w, cam.area.h);

    if(interact.pressed() && t.isTickComplete()) {
        for(auto& a : getObjects<UK::Entity>("Stair")) {
            auto b = b2TestOverlap(phys->fixture->GetAABB(0), a->getComponent<UK::PhysicsBodyComponent>()->fixture->GetAABB(0));
            if(b) {
                bool above = (ent->position.y < a->position.y);

                getObject<UK::Entity>("Player")->move(0, above ? 32 : -35);
                return;
            }
        }

        for(auto& a : getObjects<UK::Entity>("Door")) {
            auto b = b2TestOverlap(phys->fixture->GetAABB(0), a->getComponent<UK::PhysicsBodyComponent>()->fixture->GetAABB(0));
            if(b && !getObject<UK::Entity>("Other")->enabled) {
                UK_LOAD_SCENE("WorldMap");
                return;
            }
        }

        for(auto& a : getObjects<UK::Entity>("Other")) {
            if (a->enabled) {
                auto b = b2TestOverlap(phys->fixture->GetAABB(0),
                                       a->getComponent<UK::PhysicsBodyComponent>()->fixture->GetAABB(0));
                if (b) {
                    a->queueDisable();
                    Data::getData().food += 1000;
                    escape = true;
                    return;
                }
            }
        }
    }
}

void HouseScene::render() const {
    fb.bind();
    glClear(GL_COLOR_BUFFER_BIT);

    Scene::render();

    auto player = getObject<UK::Entity>("Player");
    auto phys = player->getComponent<UK::PhysicsBodyComponent>();

    for(auto& a : getObjects<UK::Entity>("Stair")) {
        auto b = b2TestOverlap(phys->fixture->GetAABB(0), a->getComponent<UK::PhysicsBodyComponent>()->fixture->GetAABB(0));
        if(b) {
            showDialog("Press space to change floor");
            break;
        }
    }

    for(auto& a : getObjects<UK::Entity>("Door")) {
        auto b = b2TestOverlap(phys->fixture->GetAABB(0), a->getComponent<UK::PhysicsBodyComponent>()->fixture->GetAABB(0));
        if(b) {
            if(getObject<UK::Entity>("Other")->enabled) {
                showDialog("You are too hungry to leave, find a snack first");
            } else {
                showDialog("Press space to exit");
            }
            break;
        }
    }

    for(auto& a : getObjects<UK::Entity>("Other")) {
        if(a->enabled) {
            auto b = b2TestOverlap(phys->fixture->GetAABB(0),
                                   a->getComponent<UK::PhysicsBodyComponent>()->fixture->GetAABB(0));
            if (b) {
                showDialog("Press space to attack");
                break;
            }
        }
    }

    fnt2.drawString("Score: " + std::to_string(Data::getData().food), 10, 10);

    if(escape)
        fnt2.drawString("Quick, escape!", 10, 20);

    Timer::getTimer().render();


//    ImGui::Begin("Dbg");
//    ImGui::Text("Suspicion: %lf\n", suspicion);
//    ImGui::Text("Pos: %lf,%lf\n", player->position.x, player->position.y);
//    ImGui::End();

//    glEnable(GL_BLEND);
//    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
//    dark.render(0, 0);
//
//    glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR);
//    glBlendEquation(GL_FUNC_SUBTRACT);
//    torchMap.render(0,0);
//    glDisable(GL_BLEND);


    fb.unbind();


    fs.bind();
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, torchMap.getTextureInfo().pointer);
    glClear(GL_COLOR_BUFFER_BIT);
    fb.render(&fs);
}
