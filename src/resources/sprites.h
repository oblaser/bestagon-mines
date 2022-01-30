/*
author          Oliver Blaser
date            30.01.2022
copyright       OLC-3 - Copyright (c) 2022 Oliver Blaser
*/

#ifndef IG_RES_SPRITES_H
#define IG_RES_SPRITES_H

#include <cstddef>
#include <cstdint>
#include <unordered_map>
#include <vector>

#include "project.h"

#include "olcPixelGameEngine.h"


class SpriteOwner
{
public:
    SpriteOwner();
    virtual ~SpriteOwner();

    void loadSprites();

protected:
    std::unordered_map<char, olc::Pixel> colors;

    olc::Sprite* spr_closed = nullptr;
    olc::Sprite* spr_error = nullptr;
    olc::Sprite* spr_flag = nullptr;
    olc::Sprite* spr_mine = nullptr;
    olc::Sprite* spr_num[7];

private:
    std::vector<olc::Sprite*> m_sprites; // copies of the sprite pointers, to delete in a loop...

    void deleteSprites();
};


#endif // IG_RES_SPRITES_H
