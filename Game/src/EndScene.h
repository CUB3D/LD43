//
// Created by cub3d on 04/12/18.
//

#ifndef LD43_ENDSCENE_H
#define LD43_ENDSCENE_H

#include <UK.h>

class EndScene : public UK::Scene {
public:
    EndScene();

    virtual void render() const override;
};


#endif //LD43_ENDSCENE_H
