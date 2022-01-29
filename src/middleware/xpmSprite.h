/*
author          Oliver Blaser
date            29.01.2022
copyright       OLC-3 - Copyright (c) 2022 Oliver Blaser
*/

/*

It's planned to create a complete XPM parser. But for now the
function `newSprite()` reads only the data field.

*/

#ifndef IG_XPMSPRITE_H
#define IG_XPMSPRITE_H

#include <cstddef>
#include <cstdint>
#include <unordered_map>

#include "olcPixelGameEngine.h"

// !!! new operator !!!
olc::Sprite* xpm_to_sprite_(int32_t w, int32_t h, const std::unordered_map<char, olc::Pixel>& colors, const char* const* xpmData);


#endif // IG_XPMSPRITE_H
