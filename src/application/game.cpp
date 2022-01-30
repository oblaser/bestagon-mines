/*
author          Oliver Blaser
date            30.01.2022
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
    constexpr size_t fieldW = 30;
    constexpr size_t fieldH = 21;
    constexpr size_t fieldSize = fieldW * fieldH;
}



Game::Game()
{
    sAppName = prj::appName;

    reset();
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
#pragma region input
    const vi2d mousePos = GetMousePos();

    if (GetMouse(olc::Mouse::LEFT).bPressed)
    {
        //m_mouseDnIdx = ;
    }
    if (GetMouse(olc::Mouse::LEFT).bReleased)
    {
    }

#ifdef PRJ_DEBUG
    if (GetKey(olc::ESCAPE).bReleased) olc_Terminate();
#endif
#pragma endregion

#pragma region draw
    Clear(olc::Pixel(0xf0, 0xf0, 0xf0));

    const auto pm = GetPixelMode();
    SetPixelMode(olc::Pixel::ALPHA);

    const vi2d fieldOrig(20, 20);

    constexpr int32_t xOffset = 12;

    for (int32_t y = 0; y < fieldH; ++y)
    {
        int32_t xoff;
        //int32_t n_x;

        if (y & 1)
        {
            xoff = xOffset;
            //n_x = 19;
        }
        else
        {
            xoff = 0;
            //n_x = 20;
        }

        for (int32_t x = 0; x < fieldW/*n_x*/; ++x)
        {
            const auto fieldVal = m_field[y * fieldW + x];

            if (fieldVal != T_NOSHOW)
            {
                olc::Sprite* spr;

                if (fieldVal >= T_OPEN && fieldVal <= T_6) spr = spr_num[fieldVal];
                else if (fieldVal == T_CLOSED) spr = spr_closed;
                else if (fieldVal == T_MINE) spr = spr_mine;
                else if (fieldVal == T_EXPLODED) spr = spr_error;
                else if (fieldVal == T_FLAG) spr = spr_flag;
                else spr = spr_error;

                DrawSprite(fieldOrig.x + xoff + 24 * x, fieldOrig.y + 20 * y, spr);
            }
        }
    }

    SetPixelMode(pm);
#pragma endregion

    int32_t fieldIdxX = (mousePos.x - fieldOrig.x) / 24;
    int32_t fieldIdxY = (mousePos.y - fieldOrig.y) / 20;

    DrawRect(fieldOrig.x + /*xoff*/ +24 * fieldIdxX, fieldOrig.y + 20 * fieldIdxY, 26, 30, olc::RED);



#if 0
    DrawSprite(fieldOrig + vi2d(24 * 0, 0), spr_closed);
    DrawSprite(fieldOrig + vi2d(24 * 1, 0), spr_closed);
    DrawSprite(fieldOrig + vi2d(24 * 2, 0), spr_closed);

    DrawSprite(fieldOrig + vi2d(12 + 24 * 0, 20), spr_closed);

    static bool tmp = true;
    if (GetKey(olc::F4).bReleased) tmp = !tmp;
    if (tmp)
    {
        DrawSprite(fieldOrig + vi2d(24 * 3, 0), spr_closed);
        DrawSprite(fieldOrig + vi2d(24 * 4, 0), spr_tiles[1]);
        DrawSprite(fieldOrig + vi2d(24 * 5, 0), spr_tiles[2]);
        DrawSprite(fieldOrig + vi2d(24 * 6, 0), spr_tiles[0]);
        DrawSprite(fieldOrig + vi2d(24 * 7, 0), spr_closed);
        DrawSprite(fieldOrig + vi2d(24 * 8, 0), spr_mine);
        DrawSprite(fieldOrig + vi2d(24 * 9, 0), spr_closed);

        DrawSprite(fieldOrig + vi2d(12 + 24 * 1, 20 * 1), spr_tiles[3]);
        DrawSprite(fieldOrig + vi2d(12 + 24 * 2, 20 * 1), spr_tiles[4]);
        DrawSprite(fieldOrig + vi2d(12 + 24 * 3, 20 * 1), spr_tiles[5]);
        DrawSprite(fieldOrig + vi2d(12 + 24 * 4, 20 * 1), spr_tiles[0]);
        DrawSprite(fieldOrig + vi2d(12 + 24 * 5, 20 * 1), spr_tiles[0]);
        DrawSprite(fieldOrig + vi2d(12 + 24 * 6, 20 * 1), spr_closed);
        DrawSprite(fieldOrig + vi2d(12 + 24 * 7, 20 * 1), spr_closed);
        DrawSprite(fieldOrig + vi2d(12 + 24 * 8, 20 * 1), spr_closed);

        DrawSprite(fieldOrig + vi2d(24 * 0, 20 * 2), spr_tiles[0]);
        DrawSprite(fieldOrig + vi2d(24 * 1, 20 * 2), spr_tiles[0]);
        DrawSprite(fieldOrig + vi2d(24 * 2, 20 * 2), spr_tiles[0]);
        DrawSprite(fieldOrig + vi2d(24 * 3, 20 * 2), spr_tiles[0]);
        DrawSprite(fieldOrig + vi2d(24 * 4, 20 * 2), spr_tiles[0]);
        DrawSprite(fieldOrig + vi2d(24 * 5, 20 * 2), spr_tiles[6]);
        DrawSprite(fieldOrig + vi2d(24 * 6, 20 * 2), spr_closed);
        DrawSprite(fieldOrig + vi2d(24 * 7, 20 * 2), spr_closed);
        DrawSprite(fieldOrig + vi2d(24 * 8, 20 * 2), spr_closed);
        DrawSprite(fieldOrig + vi2d(24 * 9, 20 * 2), spr_closed);

        DrawSprite(fieldOrig + vi2d(12 + 24 * 0, 20 * 3), spr_mine);
        DrawSprite(fieldOrig + vi2d(12 + 24 * 1, 20 * 3), spr_flag);
        DrawSprite(fieldOrig + vi2d(12 + 24 * 2, 20 * 3), spr_closed);
        DrawSprite(fieldOrig + vi2d(12 + 24 * 3, 20 * 3), spr_closed);
        DrawSprite(fieldOrig + vi2d(12 + 24 * 4, 20 * 3), spr_closed);
        DrawSprite(fieldOrig + vi2d(12 + 24 * 5, 20 * 3), spr_closed);
        DrawSprite(fieldOrig + vi2d(12 + 24 * 6, 20 * 3), spr_closed);
        DrawSprite(fieldOrig + vi2d(12 + 24 * 7, 20 * 3), spr_closed);
        DrawSprite(fieldOrig + vi2d(12 + 24 * 8, 20 * 3), spr_closed);

        DrawSprite(fieldOrig + vi2d(24 * 0, 20 * 4), spr_closed);
        DrawSprite(fieldOrig + vi2d(24 * 1, 20 * 4), spr_closed);
        DrawSprite(fieldOrig + vi2d(24 * 2, 20 * 4), spr_closed);
        DrawSprite(fieldOrig + vi2d(24 * 3, 20 * 4), spr_closed);
        DrawSprite(fieldOrig + vi2d(24 * 4, 20 * 4), spr_closed);
        DrawSprite(fieldOrig + vi2d(24 * 5, 20 * 4), spr_closed);
        DrawSprite(fieldOrig + vi2d(24 * 6, 20 * 4), spr_closed);
        DrawSprite(fieldOrig + vi2d(24 * 7, 20 * 4), spr_flag);
        DrawSprite(fieldOrig + vi2d(24 * 8, 20 * 4), spr_closed);
        DrawSprite(fieldOrig + vi2d(24 * 9, 20 * 4), spr_closed);

        DrawSprite(fieldOrig + vi2d(12 + 24 * 0, 20 * 5), spr_closed);
        DrawSprite(fieldOrig + vi2d(12 + 24 * 1, 20 * 5), spr_closed);
        DrawSprite(fieldOrig + vi2d(12 + 24 * 2, 20 * 5), spr_closed);
        DrawSprite(fieldOrig + vi2d(12 + 24 * 3, 20 * 5), spr_closed);
        DrawSprite(fieldOrig + vi2d(12 + 24 * 4, 20 * 5), spr_closed);
        DrawSprite(fieldOrig + vi2d(12 + 24 * 5, 20 * 5), spr_closed);
        DrawSprite(fieldOrig + vi2d(12 + 24 * 6, 20 * 5), spr_closed);
        DrawSprite(fieldOrig + vi2d(12 + 24 * 7, 20 * 5), spr_closed);
        DrawSprite(fieldOrig + vi2d(12 + 24 * 8, 20 * 5), spr_closed);
    }
#endif

    return true;
}

bool Game::OnUserDestroy()
{
    return true;
}

void Game::createField()
{
    m_field.assign(fieldSize, T_CLOSED);
    m_mines.assign(fieldSize, false);

    for (int32_t y = 0; y < fieldH; ++y)
    {
        if (y & 1) m_field[y * fieldW + fieldW - 1] = T_NOSHOW;
    }

    m_field.shrink_to_fit();
    m_mines.shrink_to_fit();
}

void Game::reset()
{
    createField();
}
