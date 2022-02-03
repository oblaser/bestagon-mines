/*
author          Oliver Blaser
date            02.02.2022
copyright       OLC-3 - Copyright (c) 2022 Oliver Blaser
*/

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <vector>

#include "util.h"

#include "olcPixelGameEngine.h"
#ifdef OLC_PLATFORM_WINAPI
#include <Windows.h>
#endif



olc::vi2d util::vround(const olc::vf2d& v)
{
    return olc::vi2d(std::lround(v.x), std::lround(v.y));
}



olc::vf2d util::rotScaleTransl(const olc::vf2d& point, float angle, float scale, const olc::vf2d& translate)
{
    olc::vf2d r;

    r.x = point.x * std::cos(angle) - point.y * std::sin(angle);
    r.y = point.x * std::sin(angle) + point.y * std::cos(angle);

    r *= scale;

    r += translate;

    return r;
}

std::vector<olc::vf2d> util::rotScaleTransl(const std::vector<olc::vf2d>& points, float angle, float scale, const olc::vf2d& translate)
{
    const size_t n = points.size();
    std::vector<olc::vf2d> r(n);

    for (size_t i = 0; i < n; ++i)
    {
        r[i].x = points[i].x * std::cos(angle) - points[i].y * std::sin(angle);
        r[i].y = points[i].x * std::sin(angle) + points[i].y * std::cos(angle);
    }

    for (size_t i = 0; i < n; ++i)
    {
        r[i] *= scale;
    }

    for (size_t i = 0; i < n; ++i)
    {
        r[i] += translate;
    }

    r.shrink_to_fit();

    return r;
}



void util::openUrl(const std::string& url)
{
#if defined(OLC_PLATFORM_WINAPI)
    ShellExecuteA(nullptr, "open", url.c_str(), nullptr, nullptr, SW_SHOWNORMAL);
#elif defined(OLC_PLATFORM_X11)
    system(("xdg-open " + url + " > /dev/null 2>&1 &").c_str());
#endif

    // maybe in the future:
    // to generate a return code see
    // - https://docs.microsoft.com/en-us/windows/win32/api/shellapi/nf-shellapi-shellexecutea
    // - https://man7.org/linux/man-pages/man3/system.3.html
}
