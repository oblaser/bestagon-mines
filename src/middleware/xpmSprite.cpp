/*
author          Oliver Blaser
date            29.01.2022
copyright       OLC-3 - Copyright (c) 2022 Oliver Blaser
*/

#include <cstddef>
#include <cstdint>
#include <unordered_map>

#include "xpmSprite.h"

#include "olcPixelGameEngine.h"
#include "xpmSprite.h"



olc::Sprite* xpm_to_sprite_(int32_t w, int32_t h, const std::unordered_map<char, olc::Pixel>& colors, const char* const* xpmData)
{
    olc::Sprite* spr = new olc::Sprite(w, h);

    for (int32_t y = 0; y < h; ++y)
    {
        const char* const rowData = *(xpmData + y);

        for (int32_t x = 0; x < w; ++x)
        {
            spr->SetPixel(x, y, colors.at(*(rowData + x)));
        }
    }

    return spr;
}
