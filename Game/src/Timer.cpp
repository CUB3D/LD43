//
// Created by cub3d on 04/12/18.
//

#include "Timer.h"
#include <Font/TTFont.h>
#include "Data.h"

UK::Graphics::TTFont fnt4("Assets/Fonts/Arimo-Regular.ttf", 16, UK::BLACK);
UK::Graphics::TTFont fnt5("Assets/Fonts/Arimo-Regular.ttf", 16, UK::RED);

Timer::Timer() : startTime(SDL_GetTicks()) {
    endtime = (60 + 15) * 1000;
}

void Timer::render() const {
    long tss = (endtime - (SDL_GetTicks() - startTime)) / 1000;

    if(tss < 30) {
        fnt5.drawString("Time remaining: " + std::to_string(tss), 10, 35);
    } else {
        fnt4.drawString("Time remaining: " + std::to_string(tss), 10, 35);
    }

    if(tss <= 0) {
        Data::getData().caught = true;
        UK_LOAD_SCENE("End");
    }
}
