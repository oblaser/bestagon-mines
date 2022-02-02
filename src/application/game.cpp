/*
author          Oliver Blaser
date            02.02.2022
copyright       OLC-3 - Copyright (c) 2022 Oliver Blaser
*/

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <memory>
#include <string>
#include <vector>

#include "game.h"
#include "gui.h"
#include "middleware/util.h"
#include "project.h"

#include "olcPixelGameEngine.h"
#ifdef OLC_PLATFORM_WINAPI
//#include "../prj/vs/resources/resources.h"
#include <Windows.h>
#endif


using olc::vf2d;
using olc::vi2d;

namespace
{
    constexpr size_t fieldW = 30;
    constexpr size_t fieldH = 25;
    constexpr int32_t fieldPixelW = fieldW * SPR_XOFF;
    constexpr int32_t fieldPixelH = fieldH * SPR_YOFF;
    constexpr double relNMines = 0.16;

    enum FIELD_VARIANT
    {
        FV_RECT1 = 0,
        FV_RECT2,
        FV_RECT3,
        FV_HEX1,
        FV_HEX2,
        FV_HEX3,
        FV_RING,

        FV__end_
    };
    const char* fieldVarStr[FIELD_VARIANT::FV__end_] =
    {
        "RECT 1",
        "RECT 2",
        "RECT 3",
        "HEX 1",
        "HEX 2",
        "HEX 3",
        " RING"
    };
    std::string getFieldVarStr(int fv)
    {
        std::string r;
        if ((fv >= 0) && (fv < FV__end_)) r = fieldVarStr[fv];
        else r = "FV-" + std::to_string(fv);
        return r;
    }

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
    : m_gui(this), m_mouseDnFieldIdx((size_t)-1), m_mouseRDnFieldIdx((size_t)-1), m_firstClick(true), m_fieldVariant(FV_HEX2)
{
    std::string frameTitle = prj::appName;

#ifdef PRJ_DEBUG
    frameTitle += " - DEBUG";
#endif

    sAppName = frameTitle;

    reset();
}

Game::~Game()
{}

bool Game::OnUserCreate()
{
#ifdef OLC_PLATFORM_WINAPI
    HWND hwnd_pge = FindWindowExW(nullptr, nullptr, L"OLC_PIXEL_GAME_ENGINE", nullptr);
    HICON hicon = LoadIconW(GetModuleHandleW(nullptr), L"AAAAA_MainIcon");
    //HICON hicon = LoadIconW(GetModuleHandleW(nullptr), MAKEINTRESOURCEW(IDI_MAINICON));
    SendMessageW(hwnd_pge, WM_SETICON, ICON_SMALL, (LPARAM)hicon);
    SendMessageW(hwnd_pge, WM_SETICON, ICON_BIG, (LPARAM)hicon);
#endif

    loadSprites();

    m_gui.init();
    m_gui.stFieldName()->setLabel(getFieldVarStr(m_fieldVariant));

    return true;
}

bool Game::OnUserUpdate(float tElapsed)
{
    Clear(olc::Pixel(0xf0, 0xf0, 0xf0));

    m_gui.enable(m_firstClick);
    const int guiEvt = m_gui.update();

    const vi2d fieldOrig(20, 20);

#pragma region input
    const vi2d mousePos = GetMousePos();
    const size_t mouseFieldIdx = mousePosToFieldIdx(mousePos, fieldOrig);

    if (GetMouse(olc::Mouse::RIGHT).bPressed) m_mouseRDnFieldIdx = mouseFieldIdx;
    if (GetMouse(olc::Mouse::RIGHT).bReleased && (mouseFieldIdx == m_mouseRDnFieldIdx) && m_stateIsPlaying && (mouseFieldIdx < m_field.size()) && (m_field[mouseFieldIdx] == T_CLOSED))
    {
        m_field[mouseFieldIdx] = T_FLAG;
    }

    if (GetMouse(olc::Mouse::LEFT).bPressed) m_mouseDnFieldIdx = mouseFieldIdx;
    if (GetMouse(olc::Mouse::LEFT).bReleased && (mouseFieldIdx == m_mouseDnFieldIdx) && m_stateIsPlaying && (mouseFieldIdx < m_field.size()))
    {
        if (m_field[mouseFieldIdx] == T_CLOSED)
        {
            if (m_firstClick) { m_firstClick = false; distributeField(mouseFieldIdx); }

            if (m_mines[mouseFieldIdx])
            {
                m_stateIsPlaying = false;
                m_gui.btnReset()->gameOver(true);

                for (size_t y = 0; y < fieldH; ++y) for (size_t x = 0; x < fieldW; ++x)
                {
                    const size_t fieldIdx = y * fieldW + x;
                    if (m_mines[fieldIdx]) m_field[fieldIdx] = (fieldIdx == mouseFieldIdx ? T_EXPLODED : T_MINE);
                }
            }
            else m_field[mouseFieldIdx] = cntMinesAround(mouseFieldIdx);

            const bool won = discoverCheckField();

            if (won)
            {
                m_stateIsPlaying = false;

                for (size_t y = 0; y < fieldH; ++y) for (size_t x = 0; x < fieldW; ++x)
                {
                    const size_t fieldIdx = y * fieldW + x;
                    if (m_mines[fieldIdx]) m_field[fieldIdx] = T_MINE_FOUND;
                }
            }
        }
        else if (m_field[mouseFieldIdx] == T_FLAG) m_field[mouseFieldIdx] = T_CLOSED;
    }

    if (guiEvt)
    {
        if (guiEvt == Gui::EVT_RESET_CLICK) reset();
        else if (guiEvt == Gui::EVT_LEFT_CLICK)
        {
            --m_fieldVariant;
            if (m_fieldVariant < 0) m_fieldVariant = FV__end_ - 1;
            m_gui.stFieldName()->setLabel(getFieldVarStr(m_fieldVariant));
            reset();
        }
        else if (guiEvt == Gui::EVT_RIGHT_CLICK)
        {
            ++m_fieldVariant;
            if (m_fieldVariant >= FV__end_) m_fieldVariant = 0;
            m_gui.stFieldName()->setLabel(getFieldVarStr(m_fieldVariant));
            reset();
        }
        else if (guiEvt == Gui::EVT_ABOUT_CLICK)
        {
        }
    }

#ifdef PRJ_DEBUG
    if (GetKey(olc::F5).bReleased) bkpt = true;
    if (GetKey(olc::ESCAPE).bReleased) olc_Terminate();
#endif
#pragma endregion

#pragma region draw

    const auto pm = GetPixelMode();
#if defined(PRJ_DEBUG) && 1
    SetPixelMode(olc::Pixel::ALPHA);
#else
    SetPixelMode(olc::Pixel::MASK);
#endif

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
                else if (fieldVal == T_MINE_FOUND) spr = spr_mineFound;
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

    constexpr int32_t appInfoTxtYOff = 460;
    DrawString(vi2d(813, appInfoTxtYOff), prj::appName, olc::BLACK); // appName strlen = 14
    const std::string versionStr = std::string("v") + prj::versionStr;
    DrawString(vi2d(813 + (14 - (int32_t)versionStr.length()) * 4, appInfoTxtYOff + 10), versionStr, olc::BLACK); // 813 + ((14 - strlen) / 2) * 8
#ifdef PRJ_DEBUG
    DrawString(vi2d(813 + (14 - 5) * 4, appInfoTxtYOff + 20), "DEBUG", olc::DARK_RED);
#endif

#ifdef PRJ_DEBUG
    if ((mousePos.x >= fieldOrig.x) && (mousePos.x < (fieldOrig.x + fieldPixelW)) &&
        (mousePos.y >= fieldOrig.y) && (mousePos.y < (fieldOrig.y + fieldPixelH)))
    {
        const int32_t fieldIdxY = (mousePos.y - fieldOrig.y) / SPR_YOFF;
        const int32_t fieldIdxX = (mousePos.x - fieldOrig.x - (fieldIdxY & 1 ? SPR_XOFF_OR : 0)) / SPR_XOFF;
        DrawRect(fieldOrig.x + (fieldIdxY & 1 ? SPR_XOFF_OR : 0) + SPR_XOFF * fieldIdxX, fieldOrig.y + SPR_YOFF * fieldIdxY, 26, 30, olc::RED);
    }
#endif

#ifdef PRJ_DEBUG
    DrawString(2, 540, mousePos.str(), olc::BLACK);
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

void Game::createField()
{
    if (m_fieldVariant == FV_RECT1)
    {
        const vi2d lo(10, 5);
        const vi2d hi(20, 15);

        for (int32_t y = 0; y < (int32_t)fieldH; ++y)                               // rect 1 algorithm
        {                                                                           //
            for (int32_t x = 0; x < (int32_t)fieldW; ++x)                           //
            {                                                                       //
                if ((x < lo.x) || (y < lo.y) || (x > (y & 1 ? hi.x - 1 : hi.x)) || (y > hi.y)) //
                    m_field[fieldCoordToFieldIdx(x, y)] = T_NOSHOW;                 //
            }                                                                       //
        }                                                                           //
    }
    else if (m_fieldVariant == FV_RECT2)
    {
        const vi2d lo(5, 3);
        const vi2d hi(fieldW - 5, fieldH - 4);

        for (int32_t y = 0; y < (int32_t)fieldH; ++y)                               // rect 1 algorithm
        {                                                                           //
            for (int32_t x = 0; x < (int32_t)fieldW; ++x)                           //
            {                                                                       //
                if ((x < lo.x) || (y < lo.y) || (x > (y & 1 ? hi.x - 1 : hi.x)) || (y > hi.y)) //
                    m_field[fieldCoordToFieldIdx(x, y)] = T_NOSHOW;                 //
            }                                                                       //
        }                                                                           //
    }
    else if (m_fieldVariant == FV_RECT3)
    {
        for (size_t y = 0; y < fieldH; ++y)                                         // rect max algorithm
            if (y & 1) m_field[y * fieldW + fieldW - 1] = T_NOSHOW;                 //
    }
    else if (m_fieldVariant == FV_HEX1)
    {
        // const int32_t nHide[fieldH] =
        // {
        //     15, 15, 15, 15, 15,
        //     15, 15, 15, 15, 15,
        //     15, 15, 15, 15, 15,
        //     15, 15, 15, 15, 15,
        //     15, 15, 15, 15, 15
        // };

        const int32_t nHide[fieldH] =
        {
            15, 15, 15, 15, 10,
            9, 9, 8, 8, 7,
            7, 6, 6, 6, 7,
            7, 8, 8, 9, 9,
            10, 15, 15, 15, 15
        };

        for (int32_t y = 0; y < (int32_t)fieldH; ++y)                               // hex 1 algorithm
        {                                                                           //
            m_field[fieldCoordToFieldIdx(0, y)] = T_NOSHOW;                         //
                                                                                    //
            for (int32_t i = 0; i < nHide[y]; ++i)                                  //
            {                                                                       //
                size_t idx;                                                         //
                                                                                    //
                if (y & 1)                                                          //
                {                                                                   //
                    idx = fieldCoordToFieldIdx(i + 1, y);                           //
                    if (idx < m_field.size()) m_field[idx] = T_NOSHOW;              //
                                                                                    //
                    idx = fieldCoordToFieldIdx((int32_t)fieldW - i - 1 - 1, y);     //
                    if (idx < m_field.size()) m_field[idx] = T_NOSHOW;              //
                }                                                                   //
                else                                                                //
                {                                                                   //
                    idx = fieldCoordToFieldIdx(i + 1, y);                           //
                    if (idx < m_field.size()) m_field[idx] = T_NOSHOW;              //
                                                                                    //
                    idx = fieldCoordToFieldIdx((int32_t)fieldW - i - 1, y);         //
                    if (idx < m_field.size()) m_field[idx] = T_NOSHOW;              //
                }                                                                   //
            }                                                                       //
        }                                                                           //

        for (size_t y = 0; y < fieldH; ++y)                                         // rect max algorithm
            if (y & 1) m_field[y * fieldW + fieldW - 1] = T_NOSHOW;                 //
    }
    else if (m_fieldVariant == FV_HEX2)
    {
        const int32_t nHide[fieldH] =
        {
            8, 7, 7, 6, 6,
            5, 5, 4, 4, 3,
            3, 2, 2, 2, 3,
            3, 4, 4, 5, 5,
            6, 6, 7, 7, 8
        };

        for (int32_t y = 0; y < (int32_t)fieldH; ++y)                               // hex 1 algorithm
        {                                                                           //
            m_field[fieldCoordToFieldIdx(0, y)] = T_NOSHOW;                         //
                                                                                    //
            for (int32_t i = 0; i < nHide[y]; ++i)                                  //
            {                                                                       //
                size_t idx;                                                         //
                                                                                    //
                if (y & 1)                                                          //
                {                                                                   //
                    idx = fieldCoordToFieldIdx(i + 1, y);                           //
                    if (idx < m_field.size()) m_field[idx] = T_NOSHOW;              //
                                                                                    //
                    idx = fieldCoordToFieldIdx((int32_t)fieldW - i - 1 - 1, y);     //
                    if (idx < m_field.size()) m_field[idx] = T_NOSHOW;              //
                }                                                                   //
                else                                                                //
                {                                                                   //
                    idx = fieldCoordToFieldIdx(i + 1, y);                           //
                    if (idx < m_field.size()) m_field[idx] = T_NOSHOW;              //
                                                                                    //
                    idx = fieldCoordToFieldIdx((int32_t)fieldW - i - 1, y);         //
                    if (idx < m_field.size()) m_field[idx] = T_NOSHOW;              //
                }                                                                   //
            }                                                                       //
        }                                                                           //

        for (size_t y = 0; y < fieldH; ++y)                                         // rect max algorithm
            if (y & 1) m_field[y * fieldW + fieldW - 1] = T_NOSHOW;                 //
    }
    else if (m_fieldVariant == FV_HEX3)
    {
        const int32_t nHide[fieldH] =
        {
            6, 5, 5, 4, 4,
            3, 3, 2, 2, 1,
            1, 0, 0, 0, 1,
            1, 2, 2, 3, 3,
            4, 4, 5, 5, 6
        };

        for (int32_t y = 0; y < (int32_t)fieldH; ++y)                               // hex full algorithm
        {                                                                           //
            for (int32_t i = 0; i < nHide[y]; ++i)                                  //
            {                                                                       //
                size_t idx;                                                         //
                                                                                    //
                if (y & 1)                                                          //
                {                                                                   //
                    idx = fieldCoordToFieldIdx(i, y);                               //
                    if (idx < m_field.size()) m_field[idx] = T_NOSHOW;              //
                                                                                    //
                    idx = fieldCoordToFieldIdx((int32_t)fieldW - i - 1 - 1, y);     //
                    if (idx < m_field.size()) m_field[idx] = T_NOSHOW;              //
                }                                                                   //
                else                                                                //
                {                                                                   //
                    idx = fieldCoordToFieldIdx(i, y);                               //
                    if (idx < m_field.size()) m_field[idx] = T_NOSHOW;              //
                                                                                    //
                    idx = fieldCoordToFieldIdx((int32_t)fieldW - i - 1, y);         //
                    if (idx < m_field.size()) m_field[idx] = T_NOSHOW;              //
                }                                                                   //
            }                                                                       //
        }                                                                           //

        for (size_t y = 0; y < fieldH; ++y)                                         // rect max algorithm
            if (y & 1) m_field[y * fieldW + fieldW - 1] = T_NOSHOW;                 //
    }
    else if (m_fieldVariant == FV_RING)
    {
        const int32_t nHide[fieldH] =
        {
            8, 7, 7, 6, 6,
            5, 5, 4, 4, 3,
            3, 2, 2, 2, 3,
            3, 4, 4, 5, 5,
            6, 6, 7, 7, 8
        };

        for (int32_t y = 0; y < (int32_t)fieldH; ++y)                               // hex 1 algorithm
        {                                                                           //
            m_field[fieldCoordToFieldIdx(0, y)] = T_NOSHOW;                         //
                                                                                    //
            for (int32_t i = 0; i < nHide[y]; ++i)                                  //
            {                                                                       //
                size_t idx;                                                         //
                                                                                    //
                if (y & 1)                                                          //
                {                                                                   //
                    idx = fieldCoordToFieldIdx(i + 1, y);                           //
                    if (idx < m_field.size()) m_field[idx] = T_NOSHOW;              //
                                                                                    //
                    idx = fieldCoordToFieldIdx((int32_t)fieldW - i - 1 - 1, y);     //
                    if (idx < m_field.size()) m_field[idx] = T_NOSHOW;              //
                }                                                                   //
                else                                                                //
                {                                                                   //
                    idx = fieldCoordToFieldIdx(i + 1, y);                           //
                    if (idx < m_field.size()) m_field[idx] = T_NOSHOW;              //
                                                                                    //
                    idx = fieldCoordToFieldIdx((int32_t)fieldW - i - 1, y);         //
                    if (idx < m_field.size()) m_field[idx] = T_NOSHOW;              //
                }                                                                   //
            }                                                                       //
        }                                                                           //

        for (size_t y = 0; y < fieldH; ++y)                                         // rect max algorithm
            if (y & 1) m_field[y * fieldW + fieldW - 1] = T_NOSHOW;                 //

        // ring 1
        for (int32_t i = 0; i < 5; ++i) m_field[fieldCoordToFieldIdx(13 + i, 8)] = T_NOSHOW;
        for (int32_t i = 0; i < 6; ++i) m_field[fieldCoordToFieldIdx(12 + i, 9)] = T_NOSHOW;
        for (int32_t i = 0; i < 7; ++i) m_field[fieldCoordToFieldIdx(12 + i, 10)] = T_NOSHOW;
        for (int32_t i = 0; i < 8; ++i) m_field[fieldCoordToFieldIdx(11 + i, 11)] = T_NOSHOW;
        for (int32_t i = 0; i < 9; ++i) m_field[fieldCoordToFieldIdx(11 + i, 12)] = T_NOSHOW;
        for (int32_t i = 0; i < 8; ++i) m_field[fieldCoordToFieldIdx(11 + i, 13)] = T_NOSHOW;
        for (int32_t i = 0; i < 7; ++i) m_field[fieldCoordToFieldIdx(12 + i, 14)] = T_NOSHOW;
        for (int32_t i = 0; i < 6; ++i) m_field[fieldCoordToFieldIdx(12 + i, 15)] = T_NOSHOW;
        for (int32_t i = 0; i < 5; ++i) m_field[fieldCoordToFieldIdx(13 + i, 16)] = T_NOSHOW;

        // ring 2
        // for (int32_t i = 0; i < 4; ++i) m_field[fieldCoordToFieldIdx(13 + i, 9)] = T_NOSHOW;
        // for (int32_t i = 0; i < 5; ++i) m_field[fieldCoordToFieldIdx(13 + i, 10)] = T_NOSHOW;
        // for (int32_t i = 0; i < 6; ++i) m_field[fieldCoordToFieldIdx(12 + i, 11)] = T_NOSHOW;
        // for (int32_t i = 0; i < 7; ++i) m_field[fieldCoordToFieldIdx(12 + i, 12)] = T_NOSHOW;
        // for (int32_t i = 0; i < 6; ++i) m_field[fieldCoordToFieldIdx(12 + i, 13)] = T_NOSHOW;
        // for (int32_t i = 0; i < 5; ++i) m_field[fieldCoordToFieldIdx(13 + i, 14)] = T_NOSHOW;
        // for (int32_t i = 0; i < 4; ++i) m_field[fieldCoordToFieldIdx(13 + i, 15)] = T_NOSHOW;
    }
    else
    {
        for (size_t y = 0; y < fieldH; ++y)                                         // rect max algorithm
            if (y & 1) m_field[y * fieldW + fieldW - 1] = T_NOSHOW;                 //
    }
}

// 
// returns `true` if all safe tiles are found
// 
bool Game::discoverCheckField()
{
    bool foundAllSafe = true;
    int32_t cntLoops = 0;

    bool discoveringTiles;
    do
    {
        discoveringTiles = false;

        for (int32_t x = 0; x < fieldW; ++x)
        {
            for (int32_t y = 0; y < fieldH; ++y)
            {
                const size_t idx = fieldCoordToFieldIdx(x, y);
                const int tile = m_field[idx];

                if (foundAllSafe && !m_mines[idx] && (tile > T_6)) foundAllSafe = false;

                if (tile == T_OPEN)
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

    return foundAllSafe;
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

    size_t nMines = (size_t)(nTiles * relNMines);

    std::srand((unsigned int)std::time(nullptr));

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

#if defined(PRJ_DEBUG) && 0
#define DBG_mousePosToFieldIdx (1)
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

#ifdef DBG_mousePosToFieldIdx
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
    createField();
    m_field.shrink_to_fit();

    m_mines.assign(fieldSize, false);
    m_mines.shrink_to_fit();

    m_firstClick = true;
    m_stateIsPlaying = true;
    if (m_gui.btnReset()) m_gui.btnReset()->gameOver(false);
}
