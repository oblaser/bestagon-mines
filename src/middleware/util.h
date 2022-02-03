/*
author          Oliver Blaser
date            02.02.2022
copyright       OLC-3 - Copyright (c) 2022 Oliver Blaser
*/

#ifndef IG_UTIL_H
#define IG_UTIL_H

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <string>
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

    void openUrl(const std::string& url);
}





//
// from github.com/oblaser/omw
// adapted to work here
//
#define OMW__FILENAME__     (OMWi_file_to_filename(__FILE__))
constexpr const char* OMWi_file_to_filename(const char* p)
{
    const char* fn = p;
    while (*p)
    {
#if defined(_WIN32)
        if (*p++ == '\\') fn = p;
#else
        if (*p++ == '/') fn = p;
#endif
    }
    return fn;
}



#endif // IG_UTIL_H
