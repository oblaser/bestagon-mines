/*
author          Oliver Blaser
date            29.01.2022
copyright       OLC-3 - Copyright (c) 2022 Oliver Blaser
*/

#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

#include "game.h"
#include "middleware/util.h"
#include "project.h"

#include "olcPixelGameEngine.h"


using olc::vf2d;
using olc::vi2d;

namespace
{
    const olc::Pixel bg(31, 10, 84);
    const olc::Pixel fg(255, 119, 0);

    void drawHexagon(olc::PixelGameEngine* pge, const vi2d& pos, float r, float a_deg)
    {
        std::vector<vf2d> points =
        {
            vf2d(r, util::deg_rad(-30)).cart(),
            vf2d(r, util::deg_rad(-90)).cart(),
            vf2d(r, util::deg_rad(-150)).cart(),
            vf2d(r, util::deg_rad(150)).cart(),
            vf2d(r, util::deg_rad(90)).cart(),
            vf2d(r, util::deg_rad(30)).cart()
        };

        points = util::rotScaleTransl(points, util::deg_rad(a_deg), 1.0f, pos);

        const size_t n = points.size();
        for (size_t i = 0; i < n; ++i)
        {
            pge->DrawLine(util::vround(points[i]), util::vround(points[(i + 1) % n]), fg);
        }
    }
}



Game::Game()
{
    sAppName = prj::appName;
}

bool Game::OnUserCreate()
{
    m_spr = std::make_unique<olc::Sprite>("frog.spr");
    return true;
}

bool Game::OnUserUpdate(float tElapsed)
{
    Clear(bg);

#ifdef PRJ_DEBUG
    if (GetKey(olc::ESCAPE).bReleased) olc_Terminate();
#endif

    drawHexagon(this, util::vround(vf2d(10, 10) * 2.5), 14, 0.1);
    drawHexagon(this, util::vround(vf2d(20, 10) * 2.5), 14, 0.1);
    drawHexagon(this, util::vround(vf2d(30, 10) * 2.5), 14, 0.1);
    drawHexagon(this, util::vround(vf2d(15, 19) * 2.5), 14, 0.1);
    drawHexagon(this, util::vround(vf2d(25, 19) * 2.5), 14, 0.1);
    drawHexagon(this, util::vround(vf2d(10, 28) * 2.5), 14, 0.1);
    drawHexagon(this, util::vround(vf2d(20, 28) * 2.5), 14, 0.1);
    drawHexagon(this, util::vround(vf2d(30, 28) * 2.5), 14, 0.1);

    const vi2d mousePos = GetMousePos();
    DrawRect(mousePos - vi2d(1, 1), vi2d(3, 3));

    DrawSprite(40, 40, m_spr.get());

    return true;
}

bool Game::OnUserDestroy()
{
    return true;
}
