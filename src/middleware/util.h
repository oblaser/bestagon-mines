/*
author          Oliver Blaser
date            28.01.2022
copyright       OLC-3 - Copyright (c) 2022 Oliver Blaser
*/

#ifndef IG_UTIL_H
#define IG_UTIL_H

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <vector>

#include "olcPixelGameEngine.h"


namespace util
{
    constexpr float pi = 3.14159265359f;

    // deg to rad angle
    constexpr float deg_rad(float deg) { return deg * util::pi / 180.0f; }

    olc::vi2d vround(const olc::vf2d& v);

    olc::vf2d rotScaleTransl(const olc::vf2d& point, float angle, float scale = 1.0f, const olc::vf2d& translate = olc::vf2d(0.0f, 0.0f));
    std::vector<olc::vf2d> rotScaleTransl(const std::vector<olc::vf2d>& points, float angle, float scale = 1.0f, const olc::vf2d& translate = olc::vf2d(0.0f, 0.0f));
}


#endif // IG_UTIL_H
