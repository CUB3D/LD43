//
// Created by cub3d on 02/12/18.
//

#include "EventTriggerComponent.h"

EventTriggerComponent::EventTriggerComponent() {

}

RTTR_REGISTRATION {
    rttr::registration::class_<EventTriggerComponent>("EventTriggerComponent")
        .constructor<>();
};
