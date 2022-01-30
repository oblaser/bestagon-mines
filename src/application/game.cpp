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
}



Game::Game()
{
    sAppName = prj::appName;
}

Game::~Game()
{}

bool Game::OnUserCreate()
{
    loadSprites();

    return true;
}

bool Game::OnUserUpdate(float tElapsed)
{
    Clear(olc::Pixel(0xab, 0xab, 0xab));

    //static bool a_or_b = true;
    //if (GetKey(olc::F3).bReleased)
    //{
    //    a_or_b = !a_or_b;
    //    delete spr_hexagon;
    //    if (a_or_b) spr_hexagon = xpm_to_sprite_(26, 30, colors, hexagon_v1_variantA);
    //    else spr_hexagon = xpm_to_sprite_(26, 30, colors, hexagon_v1_variantB);
    //}
    //if (a_or_b) DrawString(1, 1, "A");
    //else DrawString(1, 1, "B");
    //
    //if (GetKey(olc::K0).bReleased) { a_or_b = true; delete spr_hexagon; spr_hexagon = xpm_to_sprite_(26, 30, colors, hexagon_v1_variantA); }
    //if (GetKey(olc::K1).bReleased) { a_or_b = true; delete spr_hexagon; spr_hexagon = xpm_to_sprite_(26, 30, colors, hexagon_v1_variantA_1); }
    //if (GetKey(olc::K2).bReleased) { a_or_b = true; delete spr_hexagon; spr_hexagon = xpm_to_sprite_(26, 30, colors, hexagon_v1_variantA_2); }
    //if (GetKey(olc::K3).bReleased) { a_or_b = true; delete spr_hexagon; spr_hexagon = xpm_to_sprite_(26, 30, colors, hexagon_v1_variantA_3); }
    //if (GetKey(olc::K4).bReleased) { a_or_b = true; delete spr_hexagon; spr_hexagon = xpm_to_sprite_(26, 30, colors, hexagon_v1_variantA_4); }
    //if (GetKey(olc::K5).bReleased) { a_or_b = true; delete spr_hexagon; spr_hexagon = xpm_to_sprite_(26, 30, colors, hexagon_v1_variantA_5); }
    //if (GetKey(olc::K6).bReleased) { a_or_b = true; delete spr_hexagon; spr_hexagon = xpm_to_sprite_(26, 30, colors, hexagon_v1_variantA_6); }

#ifdef PRJ_DEBUG
    if (GetKey(olc::ESCAPE).bReleased) olc_Terminate();
#endif

    const auto pm = GetPixelMode();
    SetPixelMode(olc::Pixel::ALPHA);

    const vi2d fieldOrig(10, 10);
    DrawSprite(fieldOrig + vi2d(24 * 0, 0), spr_closed);
    DrawSprite(fieldOrig + vi2d(24 * 1, 0), spr_closed);
    DrawSprite(fieldOrig + vi2d(24 * 2, 0), spr_closed);

    DrawSprite(fieldOrig + vi2d(12 + 24 * 0, 20), spr_closed);

    static bool tmp = false;
    if (GetKey(olc::F4).bReleased) tmp = !tmp;
    if (tmp)
    {
        DrawSprite(fieldOrig + vi2d(24 * 3, 0), spr_closed);
        DrawSprite(fieldOrig + vi2d(24 * 4, 0), spr_tiles[1]);
        DrawSprite(fieldOrig + vi2d(24 * 5, 0), spr_tiles[2]);
        DrawSprite(fieldOrig + vi2d(24 * 6, 0), spr_tiles[0]);

        DrawSprite(fieldOrig + vi2d(12 + 24 * 1, 20), spr_tiles[3]);
        DrawSprite(fieldOrig + vi2d(12 + 24 * 2, 20), spr_tiles[4]);
        DrawSprite(fieldOrig + vi2d(12 + 24 * 3, 20), spr_tiles[5]);
        DrawSprite(fieldOrig + vi2d(12 + 24 * 4, 20), spr_tiles[6]);
        DrawSprite(fieldOrig + vi2d(12 + 24 * 5, 20), spr_tiles[0]);

        DrawSprite(fieldOrig + vi2d(24 * 0, 20 * 2), spr_tiles[0]);
        DrawSprite(fieldOrig + vi2d(24 * 1, 20 * 2), spr_tiles[0]);
        DrawSprite(fieldOrig + vi2d(24 * 2, 20 * 2), spr_tiles[0]);
        DrawSprite(fieldOrig + vi2d(24 * 3, 20 * 2), spr_tiles[0]);
        DrawSprite(fieldOrig + vi2d(24 * 4, 20 * 2), spr_tiles[0]);
        DrawSprite(fieldOrig + vi2d(24 * 5, 20 * 2), spr_tiles[0]);
        DrawSprite(fieldOrig + vi2d(24 * 6, 20 * 2), spr_closed);

        DrawSprite(fieldOrig + vi2d(12 + 24 * 0, 20 * 3), spr_mine);
        DrawSprite(fieldOrig + vi2d(12 + 24 * 1, 20 * 3), spr_flag);
        DrawSprite(fieldOrig + vi2d(12 + 24 * 2, 20 * 3), spr_closed);
        DrawSprite(fieldOrig + vi2d(12 + 24 * 3, 20 * 3), spr_closed);
        DrawSprite(fieldOrig + vi2d(12 + 24 * 4, 20 * 3), spr_closed);
        DrawSprite(fieldOrig + vi2d(12 + 24 * 5, 20 * 3), spr_closed);
    }

    SetPixelMode(pm);



    const vi2d mousePos = GetMousePos();
    Draw(mousePos, olc::RED);

    return true;
}

bool Game::OnUserDestroy()
{
    return true;
}
