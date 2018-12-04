//
// Created by cub3d on 01/12/18.
//

#ifndef LD43_HOUSESCENE_H
#define LD43_HOUSESCENE_H

#include <UK.h>
#include <Graphics/FrameBuffer.h>
#include "HouseBlock.h"


class HouseScene : public UK::Scene {
public:
    int houseID;
    UK::FrameBuffer fb;
    bool escape;
    double suspicion;

    HouseScene(int houseID);
    virtual void update();
    virtual void render() const;
};


#endif //LD43_HOUSESCENE_H
