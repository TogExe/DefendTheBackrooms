//
// Created by TogExe on 05/04/2025.
//

#ifndef TILES_H
#define TILES_H

// Those are the tiles I will be using
const char tile_floor[8][8] = {
    {'l','l','l','l','l','l','l','l'},
    {'f','f','f','f','f','f','f','f'},
    {'f','f','f','f','f','f','f','f'},
    {'f','f','f','f','f','f','f','f'},
    {'f','f','f','f','f','f','f','f'},
    {'f','f','f','f','f','f','f','f'},
    {'f','f','f','f','f','f','f','f'},
    {'f','f','f','f','f','f','f','f'},
};
const char tile_wall_h[8][8] = {
    {'w','w','w','w','w','w','w','w'},
    {'w','w','w','w','w','w','w','w'},
    {'f','f','f','f','f','f','f','f'},
    {'f','f','f','f','f','f','f','f'},
    {'f','f','f','f','f','f','f','f'},
    {'f','f','f','f','f','f','f','f'},
    {'w','w','w','w','w','w','w','w'},
    {'w','w','w','w','w','w','w','w'},
};
const char tile_wall_v[8][8] = {
    {'w','w','f','f','f','f','w','w'},
    {'w','w','f','f','f','f','w','w'},
    {'w','w','f','f','f','f','w','w'},
    {'w','w','f','f','f','f','w','w'},
    {'w','w','f','f','f','f','w','w'},
    {'w','w','f','f','f','f','w','w'},
    {'w','w','f','f','f','f','w','w'},
    {'w','w','f','f','f','f','w','w'},
};
const char tile_corner[8][8] = {
    {'w','w','w','w','w','w','w','w'},
    {'w','c','c','c','c','c','c','c'},
    {'w','c','f','f','f','f','f','f'},
    {'w','c','f','f','f','f','f','f'},
    {'w','c','f','f','f','f','f','f'},
    {'w','c','f','f','f','f','f','f'},
    {'w','c','f','f','f','f','f','f'},
    {'w','c','f','f','f','f','f','f'},
};
const char tile_doorway[8][8] = {
    {'w','w','w','w','w','w','w','w'},
    {'w','f','f','f','f','f','f','w'},
    {'w','f','f','f','f','f','f','w'},
    {'w','f','d','d','d','d','f','w'},
    {'w','f','d','d','d','d','f','w'},
    {'w','f','f','f','f','f','f','w'},
    {'w','f','f','f','f','f','f','w'},
    {'w','w','w','w','w','w','w','w'},
};
const char tile_0_open[8][8] = {
    {'l','l','l','l','l','l','l','l'},
    {'f','f','f','f','f','f','f','f'},
    {'f','f','f','f','f','f','f','f'},
    {'f','f','f','f','f','f','f','f'},
    {'f','f','f','f','f','f','f','f'},
    {'f','f','f','f','f','f','f','f'},
    {'f','f','f','f','f','f','f','f'},
    {'f','f','f','f','f','f','f','f'},
};

const char tile_1_wall_top[8][8] = {
    {'w','w','w','w','w','w','w','w'},
    {'w','w','w','w','w','w','w','w'},
    {'f','f','f','f','f','f','f','f'},
    {'f','f','f','f','f','f','f','f'},
    {'f','f','f','f','f','f','f','f'},
    {'f','f','f','f','f','f','f','f'},
    {'f','f','f','f','f','f','f','f'},
    {'f','f','f','f','f','f','f','f'},
};

const char tile_2_wall_bottom[8][8] = {
    {'f','f','f','f','f','f','f','f'},
    {'f','f','f','f','f','f','f','f'},
    {'f','f','f','f','f','f','f','f'},
    {'f','f','f','f','f','f','f','f'},
    {'f','f','f','f','f','f','f','f'},
    {'f','f','f','f','f','f','f','f'},
    {'w','w','w','w','w','w','w','w'},
    {'w','w','w','w','w','w','w','w'},
};

const char tile_3_wall_left[8][8] = {
    {'w','w','f','f','f','f','f','f'},
    {'w','w','f','f','f','f','f','f'},
    {'w','w','f','f','f','f','f','f'},
    {'w','w','f','f','f','f','f','f'},
    {'w','w','f','f','f','f','f','f'},
    {'w','w','f','f','f','f','f','f'},
    {'w','w','f','f','f','f','f','f'},
    {'w','w','f','f','f','f','f','f'},
};

const char tile_4_wall_right[8][8] = {
    {'f','f','f','f','f','f','w','w'},
    {'f','f','f','f','f','f','w','w'},
    {'f','f','f','f','f','f','w','w'},
    {'f','f','f','f','f','f','w','w'},
    {'f','f','f','f','f','f','w','w'},
    {'f','f','f','f','f','f','w','w'},
    {'f','f','f','f','f','f','w','w'},
    {'f','f','f','f','f','f','w','w'},
};

const char tile_5_cross[8][8] = {
    {'f','f','f','f','f','f','f','f'},
    {'f','f','f','f','f','f','f','f'},
    {'f','f','f','f','f','f','f','f'},
    {'f','f','f','f','f','f','f','f'},
    {'f','f','f','f','f','f','f','f'},
    {'f','f','f','f','f','f','f','f'},
    {'f','f','f','f','f','f','f','f'},
    {'f','f','f','f','f','f','f','f'},
};

const char tile_6_dead_end[8][8] = {
    {'w','w','w','w','w','w','w','w'},
    {'w','f','f','f','f','f','f','w'},
    {'w','f','f','f','f','f','f','w'},
    {'w','f','f','f','f','f','f','w'},
    {'w','f','f','f','f','f','f','w'},
    {'w','f','f','f','f','f','f','w'},
    {'w','f','f','f','f','f','f','w'},
    {'w','w','w','w','w','w','w','w'},
};

const char tile_7_corner_tl[8][8] = {
    {'w','w','w','w','w','w','w','w'},
    {'w','c','c','c','c','c','c','c'},
    {'w','c','f','f','f','f','f','f'},
    {'w','c','f','f','f','f','f','f'},
    {'w','c','f','f','f','f','f','f'},
    {'w','c','f','f','f','f','f','f'},
    {'w','c','f','f','f','f','f','f'},
    {'w','c','f','f','f','f','f','f'},
};

const char tile_8_corner_tr[8][8] = {
    {'w','w','w','w','w','w','w','w'},
    {'c','c','c','c','c','c','c','w'},
    {'f','f','f','f','f','f','c','w'},
    {'f','f','f','f','f','f','c','w'},
    {'f','f','f','f','f','f','c','w'},
    {'f','f','f','f','f','f','c','w'},
    {'f','f','f','f','f','f','c','w'},
    {'f','f','f','f','f','f','c','w'},
};

const char tile_9_corner_bl[8][8] = {
    {'w','c','f','f','f','f','f','f'},
    {'w','c','f','f','f','f','f','f'},
    {'w','c','f','f','f','f','f','f'},
    {'w','c','f','f','f','f','f','f'},
    {'w','c','f','f','f','f','f','f'},
    {'w','c','f','f','f','f','f','f'},
    {'w','c','c','c','c','c','c','c'},
    {'w','w','w','w','w','w','w','w'},
};

const char tile_10_corner_br[8][8] = {
    {'f','f','f','f','f','f','c','w'},
    {'f','f','f','f','f','f','c','w'},
    {'f','f','f','f','f','f','c','w'},
    {'f','f','f','f','f','f','c','w'},
    {'f','f','f','f','f','f','c','w'},
    {'f','f','f','f','f','f','c','w'},
    {'c','c','c','c','c','c','c','w'},
    {'w','w','w','w','w','w','w','w'},
};

const char tile_11_doorway_top[8][8] = {
    {'w','w','w','w','w','w','w','w'},
    {'w','d','d','d','d','d','d','w'},
    {'f','f','f','f','f','f','f','f'},
    {'f','f','f','f','f','f','f','f'},
    {'f','f','f','f','f','f','f','f'},
    {'f','f','f','f','f','f','f','f'},
    {'f','f','f','f','f','f','f','f'},
    {'f','f','f','f','f','f','f','f'},
};

const char tile_12_lightless[8][8] = {
    {'f','f','f','f','f','f','f','f'},
    {'f','f','f','f','f','f','f','f'},
    {'f','f','f','f','f','f','f','f'},
    {'f','f','f','f','f','f','f','f'},
    {'f','f','f','f','f','f','f','f'},
    {'f','f','f','f','f','f','f','f'},
    {'f','f','f','f','f','f','f','f'},
    {'f','f','f','f','f','f','f','f'},
};

const char tile_13_void[8][8] = {
    {'x','x','x','x','x','x','x','x'},
    {'x','x','x','x','x','x','x','x'},
    {'x','x','x','x','x','x','x','x'},
    {'x','x','x','x','x','x','x','x'},
    {'x','x','x','x','x','x','x','x'},
    {'x','x','x','x','x','x','x','x'},
    {'x','x','x','x','x','x','x','x'},
    {'x','x','x','x','x','x','x','x'},
};

const char tile_14_pillar[8][8] = {
    {'f','f','f','f','f','f','f','f'},
    {'f','c','c','f','f','c','c','f'},
    {'f','c','c','f','f','c','c','f'},
    {'f','f','f','f','f','f','f','f'},
    {'f','f','f','f','f','f','f','f'},
    {'f','c','c','f','f','c','c','f'},
    {'f','c','c','f','f','c','c','f'},
    {'f','f','f','f','f','f','f','f'},
};

const char tile_15_flicker[8][8] = {
    {'l','f','l','f','l','f','l','f'},
    {'f','f','f','f','f','f','f','f'},
    {'f','f','f','f','f','f','f','f'},
    {'f','f','f','f','f','f','f','f'},
    {'f','f','f','f','f','f','f','f'},
    {'f','f','f','f','f','f','f','f'},
    {'f','f','f','f','f','f','f','f'},
    {'f','f','f','f','f','f','f','f'},
};

const char* tiles[16] = {
    (const char*)tile_0_open,
    (const char*)tile_1_wall_top,
    (const char*)tile_2_wall_bottom,
    (const char*)tile_3_wall_left,
    (const char*)tile_4_wall_right,
    (const char*)tile_5_cross,
    (const char*)tile_6_dead_end,
    (const char*)tile_7_corner_tl,
    (const char*)tile_8_corner_tr,
    (const char*)tile_9_corner_bl,
    (const char*)tile_10_corner_br,
    (const char*)tile_11_doorway_top,
    (const char*)tile_12_lightless,
    (const char*)tile_13_void,
    (const char*)tile_14_pillar,
    (const char*)tile_15_flicker
};


#endif //TILES_H
