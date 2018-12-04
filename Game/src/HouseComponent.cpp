//
// Created by cub3d on 01/12/18.
//

#include "HouseComponent.h"

HouseComponent::HouseComponent() : Component(), houseID(0) {}

RTTR_REGISTRATION {
    rttr::registration::class_<HouseComponent>("HouseComponent")
        .constructor<>();
};
