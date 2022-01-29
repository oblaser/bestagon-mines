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
#include "middleware/xpmSprite.h"
#include "project.h"

#include "olcPixelGameEngine.h"


using olc::vf2d;
using olc::vi2d;

namespace
{
}



Game::Game()
{
    sAppName = prj::appName;
}

Game::~Game()
{
    delete m_spr;
}

bool Game::OnUserCreate()
{
    std::unordered_map<char, olc::Pixel> colors;
    colors.insert_or_assign(' ', olc::Pixel(255, 255, 255, 127));
    colors.insert_or_assign('b', olc::Pixel(0, 123, 255));
    colors.insert_or_assign('c', olc::Pixel(0, 92, 191));
    colors.insert_or_assign('.', olc::Pixel(79, 164, 255));

    const char* xpmData[] =
    {
        "            b""b            ",
        "          bbb""bbb          ",
        "        bbbbb""bbbbb        ",
        "       bbbb  ""  bbbb       ",
        "      bbb    ""    bbb      ",
        "     bbb     ""     bbb     ",
        "   bbbb      ""      bbbb   ",
        " bbbbb       ""       bbbbb ",
        "bbbb         ""         bbbb",
        "bb           ""           bb",
        "bb           ""           bb",
        "bb           ""           bb",
        "bb           ""           bb",
        "bb           ""           bb",
        "bb           ""           bb",
        "bb           ""           bb",
        "bb           ""           bb",
        "bb           ""           bb",
        "bb           ""           bb",
        "bb           ""           bb",
        "bb           ""           bb",
        "             ""         bbbb",
        "             ""       bbbbb ",
        "             ""      bbbb   ",
        "             ""     bbb     ",
        "             ""    bbb      ",
        "             ""  bbbb       ",
        "             ""bbbbb        ",
        "            b""bbb          ",
        "            b""b            "
    };

    m_spr = xpm_to_sprite_(26, 30, colors, xpmData);
    
    return true;
}

bool Game::OnUserUpdate(float tElapsed)
{
    Clear(olc::BLANK);

#ifdef PRJ_DEBUG
    if (GetKey(olc::ESCAPE).bReleased) olc_Terminate();
#endif

    DrawSprite(10, 10, m_spr);

    const vi2d mousePos = GetMousePos();
    DrawRect(mousePos - vi2d(1, 1), vi2d(3, 3));

    return true;
}

bool Game::OnUserDestroy()
{
    return true;
}
