//
// Created by cub3d on 01/12/18.
//

#include "HouseBlock.h"


HouseBlock::HouseBlock() : Entity(UK::Loader::loadEntityPrototype("Assets/Entity/HouseBlock.json")), bg("Assets/HouseBlock1.png") {
}

void HouseBlock::render(double Xoffset, double Yoffset) const {
    bg.render(this->position.x - Xoffset, position.y - Yoffset);
}
