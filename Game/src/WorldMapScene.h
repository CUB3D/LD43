//
// Created by cub3d on 01/12/18.
//

#ifndef LD43_LD43SCENE_H
#define LD43_LD43SCENE_H

#include <UK.h>

class WorldMapScene : public UK::Scene {
public:
    WorldMapScene();
    virtual void update();
    virtual void render() const;
};


#endif //LD43_LD43SCENE_H
