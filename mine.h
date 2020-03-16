#ifndef MINE_H
#define MINE_H

#include "config.h"

class Mine
{
public:
    Mine();

    int map[GRID_X][GRID_Y];
    int x;
    int y;
    int nMine;

};

#endif // MINE_H
