//
// Created by cub3d on 03/12/18.
//

#ifndef LD43_DATA_H
#define LD43_DATA_H

#include <vector>

class Data
{
public:
    std::vector<int> visitedHouses;
    int food;
    bool caught;

    Data();

    static Data& getData() {
        static Data d;
        return d;
    }
};


#endif //LD43_DATA_H
