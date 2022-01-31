/*
author          Oliver Blaser
date            31.01.2022
copyright       OLC-3 - Copyright (c) 2022 Oliver Blaser
*/

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <ctime>
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
    constexpr int32_t fieldPixelW = fieldW * SPR_XOFF;
    constexpr int32_t fieldPixelH = fieldH * SPR_YOFF;
    constexpr double relNMines = 0.16;

    constexpr size_t fieldCoordToFieldIdx(int32_t x, int32_t y) { return static_cast<size_t>(y) * fieldW + static_cast<size_t>(x); }
    constexpr size_t fieldCoordToFieldIdx(size_t x, size_t y) { return y * fieldW + x; }
    constexpr void fieldIdxToFieldCoord(size_t idx, int32_t& x, int32_t& y)
    {
        x = idx % fieldW;
        y = idx / fieldW;
    }

#ifdef PRJ_DEBUG
    bool bkpt = false;
#endif
}



Game::Game()
    : m_mouseDnFieldIdx((size_t)-1), m_mouseRDnFieldIdx((size_t)-1), m_firstClick(true)
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
    const vi2d fieldOrig(20, 20);

#pragma region input
    const vi2d mousePos = GetMousePos();
    const size_t mouseFieldIdx = mousePosToFieldIdx(mousePos, fieldOrig);

    if (GetMouse(olc::Mouse::RIGHT).bPressed) m_mouseRDnFieldIdx = mouseFieldIdx;
    if (GetMouse(olc::Mouse::RIGHT).bReleased && (mouseFieldIdx == m_mouseRDnFieldIdx) && (mouseFieldIdx < m_field.size()) && (m_field[mouseFieldIdx] == T_CLOSED))
    {
        m_field[mouseFieldIdx] = T_FLAG;
    }

    if (GetMouse(olc::Mouse::LEFT).bPressed) m_mouseDnFieldIdx = mouseFieldIdx;
    if (GetMouse(olc::Mouse::LEFT).bReleased && (mouseFieldIdx == m_mouseDnFieldIdx) && (mouseFieldIdx < m_field.size()))
    {
        if (m_field[mouseFieldIdx] == T_CLOSED)
        {
            if (m_firstClick) { m_firstClick = false; distributeField(mouseFieldIdx); }

            if (m_mines[mouseFieldIdx])
            {
                for (size_t y = 0; y < fieldH; ++y) for (size_t x = 0; x < fieldW; ++x)
                {
                    const size_t fieldIdx = y * fieldW + x;
                    if (m_mines[fieldIdx]) m_field[fieldIdx] = (fieldIdx == mouseFieldIdx ? T_EXPLODED : T_MINE);
                }
            }
            else m_field[mouseFieldIdx] = cntMinesAround(mouseFieldIdx);

            discoverField();
        }
        else if (m_field[mouseFieldIdx] == T_FLAG) m_field[mouseFieldIdx] = T_CLOSED;
    }

#ifdef PRJ_DEBUG
    if (GetKey(olc::F5).bReleased) bkpt = true;
    if (GetKey(olc::ESCAPE).bReleased) olc_Terminate();
#endif
#pragma endregion

#pragma region draw
    Clear(olc::Pixel(0xf0, 0xf0, 0xf0));

    const auto pm = GetPixelMode();
    SetPixelMode(olc::Pixel::ALPHA);

    for (int32_t y = 0; y < (int32_t)fieldH; ++y)
    {
        const int32_t xoff = (y & 1 ? SPR_XOFF_OR : 0);
        for (int32_t x = 0; x < (int32_t)fieldW; ++x)
        {
            const size_t fieldIdx = (size_t)y * fieldW + (size_t)x;
            const auto fieldVal = m_field[fieldIdx];

            if (fieldVal != T_NOSHOW)
            {
                olc::Sprite* spr;

                if ((fieldVal >= T_OPEN) && (fieldVal <= T_6)) spr = spr_num[fieldVal];
                else if (fieldVal == T_CLOSED) spr = spr_closed;
                else if (fieldVal == T_MINE) spr = spr_mine;
                else if (fieldVal == T_EXPLODED) spr = spr_exploded;
                else if (fieldVal == T_FLAG) spr = spr_flag;
                else spr = spr_error;

                DrawSprite(fieldOrig.x + xoff + SPR_XOFF * x, fieldOrig.y + SPR_YOFF * y, spr);

#ifdef PRJ_DEBUG
                if (fieldIdx == mouseFieldIdx) DrawSprite(fieldOrig.x + xoff + SPR_XOFF * x, fieldOrig.y + SPR_YOFF * y, dbg_spr_cursor);
#endif
            }
        }
    }

    SetPixelMode(pm);
#pragma endregion

#ifdef PRJ_DEBUG
    if ((mousePos.x >= fieldOrig.x) && (mousePos.x < (fieldOrig.x + fieldPixelW)) &&
        (mousePos.y >= fieldOrig.y) && (mousePos.y < (fieldOrig.y + fieldPixelH)))
    {
        const int32_t fieldIdxY = (mousePos.y - fieldOrig.y) / SPR_YOFF;
        const int32_t fieldIdxX = (mousePos.x - fieldOrig.x - (fieldIdxY & 1 ? SPR_XOFF_OR : 0)) / SPR_XOFF;
        DrawRect(fieldOrig.x + (fieldIdxY & 1 ? SPR_XOFF_OR : 0) + SPR_XOFF * fieldIdxX, fieldOrig.y + SPR_YOFF * fieldIdxY, 26, 30, olc::RED);
    }
#endif

    return true;
}

bool Game::OnUserDestroy()
{
    return true;
}

int Game::cntMinesAround(size_t fieldIdx)
{
    int32_t x, y;
    fieldIdxToFieldCoord(fieldIdx, x, y);
    return cntMinesAround(x, y);
}

int Game::cntMinesAround(int32_t x, int32_t y)
{
    int r = 0;

    if (y & 1)
    {
        if (y > 0) if (m_mines[fieldCoordToFieldIdx(x, y - 1)]) ++r;
        if ((y > 0) && (x < (fieldW - 1))) if (m_mines[fieldCoordToFieldIdx(x + 1, y - 1)]) ++r;

        if (x > 0) if (m_mines[fieldCoordToFieldIdx(x - 1, y)]) ++r;
        if (x < (fieldW - 1)) if (m_mines[fieldCoordToFieldIdx(x + 1, y)]) ++r;

        if (y < (fieldH - 1)) if (m_mines[fieldCoordToFieldIdx(x, y + 1)]) ++r;
        if ((y < (fieldH - 1)) && (x < (fieldW - 1))) if (m_mines[fieldCoordToFieldIdx(x + 1, y + 1)]) ++r;
    }
    else
    {
        if ((y > 0) && (x > 0)) if (m_mines[fieldCoordToFieldIdx(x - 1, y - 1)]) ++r;
        if (y > 0) if (m_mines[fieldCoordToFieldIdx(x, y - 1)]) ++r;

        if (x > 0) if (m_mines[fieldCoordToFieldIdx(x - 1, y)]) ++r;
        if (x < (fieldW - 1)) if (m_mines[fieldCoordToFieldIdx(x + 1, y)]) ++r;

        if ((y < (fieldH - 1)) && (x > 0)) if (m_mines[fieldCoordToFieldIdx(x - 1, y + 1)]) ++r;
        if (y < (fieldH - 1)) if (m_mines[fieldCoordToFieldIdx(x, y + 1)]) ++r;
    }

    return r;
}

void Game::discoverField()
{
    int32_t cntLoops = 0;

    bool discoveringTiles;
    do
    {
        discoveringTiles = false;

        for (int32_t x = 0; x < fieldW; ++x)
        {
            for (int32_t y = 0; y < fieldH; ++y)
            {
                if (m_field[fieldCoordToFieldIdx(x, y)] == T_OPEN)
                {
                    std::vector<vi2d> scanPos;
                    scanPos.reserve(6);

                    if (y & 1)
                    {
                        if (y > 0) scanPos.push_back(vi2d(x, y - 1));
                        if ((y > 0) && (x < (fieldW - 1))) scanPos.push_back(vi2d(x + 1, y - 1));

                        if (x > 0) scanPos.push_back(vi2d(x - 1, y));
                        if (x < (fieldW - 1)) scanPos.push_back(vi2d(x + 1, y));

                        if (y < (fieldH - 1)) scanPos.push_back(vi2d(x, y + 1));
                        if ((y < (fieldH - 1)) && (x < (fieldW - 1))) scanPos.push_back(vi2d(x + 1, y + 1));
                    }
                    else
                    {
                        if ((y > 0) && (x > 0)) scanPos.push_back(vi2d(x - 1, y - 1));
                        if (y > 0) scanPos.push_back(vi2d(x, y - 1));

                        if (x > 0) scanPos.push_back(vi2d(x - 1, y));
                        if (x < (fieldW - 1)) scanPos.push_back(vi2d(x + 1, y));

                        if ((y < (fieldH - 1)) && (x > 0)) scanPos.push_back(vi2d(x - 1, y + 1));
                        if (y < (fieldH - 1)) scanPos.push_back(vi2d(x, y + 1));
                    }

                    for (size_t i = 0; i < scanPos.size(); ++i)
                    {
                        const int32_t& tmpX = scanPos[i].x;
                        const int32_t& tmpY = scanPos[i].y;
                        const size_t tmpIdx = fieldCoordToFieldIdx(tmpX, tmpY);

                        if (m_field[tmpIdx] == T_CLOSED)
                        {
                            m_field[tmpIdx] = cntMinesAround(tmpX, tmpY);
                            discoveringTiles = true;
                        }
                    }
                }
            }
        }

        if (cntLoops >= 0) ++cntLoops;
    }
    while (discoveringTiles);
}

void Game::distributeField(size_t clickedIdx)
{
    size_t nTiles = 0;
    for (int32_t y = 0; y < (int32_t)fieldH; ++y)
    {
        for (int32_t x = 0; x < (int32_t)fieldW; ++x)
        {
            if (m_field[fieldCoordToFieldIdx(x, y)] != T_NOSHOW) ++nTiles;
        }
    }

    size_t nMines = nTiles * relNMines;

    std::srand(std::time(nullptr));

    const size_t fieldSize = (fieldW * fieldH);

    while (nMines)
    {
        size_t newMineIdx = std::rand() % fieldSize;

        if ((newMineIdx != clickedIdx) && !m_mines[newMineIdx] && (m_field[newMineIdx] != T_NOSHOW))
        {
            m_mines[newMineIdx] = true;
            --nMines;
        }
    }
}

size_t Game::mousePosToFieldIdx(const olc::vi2d& mousePos, const olc::vi2d& fieldOrig)
{
    size_t r = static_cast<size_t>(-1);

    if ((mousePos.x >= fieldOrig.x) && (mousePos.x < (fieldOrig.x + fieldPixelW)) &&
        (mousePos.y >= fieldOrig.y) && (mousePos.y < (fieldOrig.y + fieldPixelH)))
    {
        int32_t fieldCoordY = (mousePos.y - fieldOrig.y) / SPR_YOFF;
        int32_t fieldCoordX = (mousePos.x - fieldOrig.x - (fieldCoordY & 1 ? SPR_XOFF_OR : 0)) / SPR_XOFF;

        const int32_t fieldDispCoordX = fieldOrig.x + (fieldCoordY & 1 ? SPR_XOFF_OR : 0) + SPR_XOFF * fieldCoordX;
        const int32_t fieldDispCoordY = fieldOrig.y + SPR_YOFF * fieldCoordY;

        const int32_t mousePosOnSprX = mousePos.x - fieldDispCoordX;
        const int32_t mousePosOnSprY = mousePos.y - fieldDispCoordY;

        if (mousePosOnSprX >= 0)
        {
            char sprScan = scanHexagon[mousePosOnSprY][mousePosOnSprX];

#ifdef PRJ_DEBUG
            if (bkpt)
            {
                int dbg___x_ = 0;
            }
#endif

            if (fieldCoordY & 1)
            {
                if (sprScan == scanTL) --fieldCoordY;
                else if (sprScan == scanTR) { --fieldCoordY; ++fieldCoordX; }
                else if (sprScan == scanBL) ++fieldCoordY;
                else if (sprScan == scanBR) { ++fieldCoordY; ++fieldCoordX; }
            }
            else
            {
                if (sprScan == scanTL)
                {
                    if (fieldCoordX > 0) { --fieldCoordY; --fieldCoordX; }
                    else sprScan = scanBorder;
                }
                else if (sprScan == scanTR) --fieldCoordY;
                else if (sprScan == scanBL)
                {
                    if (fieldCoordX > 0) { ++fieldCoordY; --fieldCoordX; }
                    else sprScan = scanBorder;
                }
                else if (sprScan == scanBR) ++fieldCoordY;
            }

            if (sprScan != scanBorder) r = fieldCoordToFieldIdx(fieldCoordX, fieldCoordY);

#ifdef PRJ_DEBUG
            if (bkpt)
            {
                int dbg___x_ = 0;
                bkpt = false;
            }
#endif
        }
    }

    try
    {
        if (m_field.at(r) == T_NOSHOW)
            r = static_cast<size_t>(-1);
    }
    catch (...) { r = static_cast<size_t>(-1); }

    return r;
}

void Game::reset()
{
    const size_t fieldSize = (fieldW * fieldH);

    m_field.assign(fieldSize, T_CLOSED);
    for (size_t y = 0; y < fieldH; ++y)
        if (y & 1) m_field[y * fieldW + fieldW - 1] = T_NOSHOW;
    m_field.shrink_to_fit();

    m_mines.assign(fieldSize, false);
    m_mines.shrink_to_fit();

    m_firstClick = true;
}
