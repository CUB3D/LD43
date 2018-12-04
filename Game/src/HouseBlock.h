//
// Created by cub3d on 01/12/18.
//

#ifndef LD43_HOUSEBLOCK_H
#define LD43_HOUSEBLOCK_H

#include <UK.h>

class HouseBlock : public UK::Entity {
public:
    UK::Graphics::Image bg;

    HouseBlock();

    virtual void render(double Xoffset, double Yoffset) const override;
};


#endif //LD43_HOUSEBLOCK_H
