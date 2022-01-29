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
    std::unordered_map<char, olc::Pixel> colors;

    const char* hexagon_v1_variantA[] =
    {
        "            ##            ",
        "          ######          ",
        "         ########         ",
        "       ####    ####       ",
        "      ####      ####      ",
        "    ####          ####    ",
        "   ####            ####   ",
        " ####                #### ",
        "####                  ####",
        "##                      ##",
        "##                      ##",
        "##                      ##",
        "##                      ##",
        "##                      ##",
        "##                      ##",
        "##                      ##",
        "##                      ##",
        "##                      ##",
        "##                      ##",
        "##                      ##",
        "##                      ##",
        "####                  ####",
        " ####                #### ",
        "   ####            ####   ",
        "    ####          ####    ",
        "      ####      ####      ",
        "       ####    ####       ",
        "         ########         ",
        "          ######          ",
        "            ##            "
    };

    const char* hexagon_v1_variantA_1[] =
    {
        "            ##            ",
        "          ######          ",
        "         ########         ",
        "       ####    ####       ",
        "      ####      ####      ",
        "    ####          ####    ",
        "   ####            ####   ",
        " ####                #### ",
        "####        nn        ####",
        "##          nn          ##",
        "##        nnnn          ##",
        "##        nnnn          ##",
        "##      nn  nn          ##",
        "##      nn  nn          ##",
        "##          nn          ##",
        "##          nn          ##",
        "##          nn          ##",
        "##          nn          ##",
        "##          nn          ##",
        "##          nn          ##",
        "##      nnnnnnnnnn      ##",
        "####    nnnnnnnnnn    ####",
        " ####                #### ",
        "   ####            ####   ",
        "    ####          ####    ",
        "      ####      ####      ",
        "       ####    ####       ",
        "         ########         ",
        "          ######          ",
        "            ##            "
    };

    const char* hexagon_v1_variantA_2[] =
    {
        "            ##            ",
        "          ######          ",
        "         ########         ",
        "       ####    ####       ",
        "      ####      ####      ",
        "    ####          ####    ",
        "   ####            ####   ",
        " ####                #### ",
        "####      nnnnnn      ####",
        "##        nnnnnn        ##",
        "##      nn      nn      ##",
        "##      nn      nn      ##",
        "##              nn      ##",
        "##              nn      ##",
        "##            nn        ##",
        "##            nn        ##",
        "##          nn          ##",
        "##          nn          ##",
        "##        nn            ##",
        "##        nn            ##",
        "##      nnnnnnnnnn      ##",
        "####    nnnnnnnnnn    ####",
        " ####                #### ",
        "   ####            ####   ",
        "    ####          ####    ",
        "      ####      ####      ",
        "       ####    ####       ",
        "         ########         ",
        "          ######          ",
        "            ##            "
    };

    const char* hexagon_v1_variantA_3[] =
    {
        "            ##            ",
        "          ######          ",
        "         ########         ",
        "       ####    ####       ",
        "      ####      ####      ",
        "    ####          ####    ",
        "   ####            ####   ",
        " ####                #### ",
        "####      nnnnnn      ####",
        "##        nnnnnn        ##",
        "##      nn      nn      ##",
        "##      nn      nn      ##",
        "##              nn      ##",
        "##              nn      ##",
        "##          nnnn        ##",
        "##          nnnn        ##",
        "##              nn      ##",
        "##              nn      ##",
        "##      nn      nn      ##",
        "##      nn      nn      ##",
        "##        nnnnnn        ##",
        "####      nnnnnn      ####",
        " ####                #### ",
        "   ####            ####   ",
        "    ####          ####    ",
        "      ####      ####      ",
        "       ####    ####       ",
        "         ########         ",
        "          ######          ",
        "            ##            "
    };

    const char* hexagon_v1_variantA_4[] =
    {
        "            ##            ",
        "          ######          ",
        "         ########         ",
        "       ####    ####       ",
        "      ####      ####      ",
        "    ####          ####    ",
        "   ####            ####   ",
        " ####                #### ",
        "####          nn      ####",
        "##            nn        ##",
        "##          nnnn        ##",
        "##          nnnn        ##",
        "##        nn  nn        ##",
        "##        nn  nn        ##",
        "##      nn    nn        ##",
        "##      nn    nn        ##",
        "##      nnnnnnnnnn      ##",
        "##      nnnnnnnnnn      ##",
        "##            nn        ##",
        "##            nn        ##",
        "##            nn        ##",
        "####          nn      ####",
        " ####                #### ",
        "   ####            ####   ",
        "    ####          ####    ",
        "      ####      ####      ",
        "       ####    ####       ",
        "         ########         ",
        "          ######          ",
        "            ##            "
    };

    const char* hexagon_v1_variantA_5[] =
    {
        "            ##            ",
        "          ######          ",
        "         ########         ",
        "       ####    ####       ",
        "      ####      ####      ",
        "    ####          ####    ",
        "   ####            ####   ",
        " ####                #### ",
        "####    nnnnnnnnnn    ####",
        "##      nnnnnnnnnn      ##",
        "##      nn              ##",
        "##      nn              ##",
        "##      nnnnnnnn        ##",
        "##      nnnnnnnn        ##",
        "##              nn      ##",
        "##              nn      ##",
        "##              nn      ##",
        "##              nn      ##",
        "##              nn      ##",
        "##              nn      ##",
        "##      nnnnnnnn        ##",
        "####    nnnnnnnn      ####",
        " ####                #### ",
        "   ####            ####   ",
        "    ####          ####    ",
        "      ####      ####      ",
        "       ####    ####       ",
        "         ########         ",
        "          ######          ",
        "            ##            "
    };

    const char* hexagon_v1_variantA_6[] =
    {
        "            ##            ",
        "          ######          ",
        "         ########         ",
        "       ####    ####       ",
        "      ####      ####      ",
        "    ####          ####    ",
        "   ####            ####   ",
        " ####                #### ",
        "####        nnnn      ####",
        "##          nnnn        ##",
        "##        nn            ##",
        "##        nn            ##",
        "##      nn              ##",
        "##      nn              ##",
        "##      nnnnnnnn        ##",
        "##      nnnnnnnn        ##",
        "##      nn      nn      ##",
        "##      nn      nn      ##",
        "##      nn      nn      ##",
        "##      nn      nn      ##",
        "##        nnnnnn        ##",
        "####      nnnnnn      ####",
        " ####                #### ",
        "   ####            ####   ",
        "    ####          ####    ",
        "      ####      ####      ",
        "       ####    ####       ",
        "         ########         ",
        "          ######          ",
        "            ##            "
    };

    const char* hexagon_v1_variantB[] =
    {
        "            ##            ",
        "          ######          ",
        "         ###  ###         ",
        "       ####    ####       ",
        "      ###        ###      ",
        "    ####          ####    ",
        "   ###              ###   ",
        " ####                #### ",
        "###                    ###",
        "##                      ##",
        "##                      ##",
        "##                      ##",
        "##                      ##",
        "##                      ##",
        "##                      ##",
        "##                      ##",
        "##                      ##",
        "##                      ##",
        "##                      ##",
        "##                      ##",
        "##                      ##",
        "###                    ###",
        " ####                #### ",
        "   ###              ###   ",
        "    ####          ####    ",
        "      ###        ###      ",
        "       ####    ####       ",
        "         ###  ###         ",
        "          ######          ",
        "            ##            "
    };
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
    colors.insert_or_assign(' ', olc::Pixel(255, 255, 255, 0));
    colors.insert_or_assign('#', olc::Pixel(0, 123, 255));
    colors.insert_or_assign('n', olc::Pixel(0, 123, 255, 127));
    colors.insert_or_assign('c', olc::Pixel(0, 92, 191));
    colors.insert_or_assign('.', olc::Pixel(79, 164, 255));

    m_spr = xpm_to_sprite_(26, 30, colors, hexagon_v1_variantA);

    return true;
}

bool Game::OnUserUpdate(float tElapsed)
{
    Clear(olc::BLANK);

    static bool a_or_b = true;
    if (GetKey(olc::F3).bReleased)
    {
        a_or_b = !a_or_b;
        delete m_spr;
        if (a_or_b) m_spr = xpm_to_sprite_(26, 30, colors, hexagon_v1_variantA);
        else m_spr = xpm_to_sprite_(26, 30, colors, hexagon_v1_variantB);
    }
    if (a_or_b) DrawString(1, 1, "A");
    else DrawString(1, 1, "B");

    if (GetKey(olc::K0).bReleased) { a_or_b = true; delete m_spr; m_spr = xpm_to_sprite_(26, 30, colors, hexagon_v1_variantA); }
    if (GetKey(olc::K1).bReleased) { a_or_b = true; delete m_spr; m_spr = xpm_to_sprite_(26, 30, colors, hexagon_v1_variantA_1); }
    if (GetKey(olc::K2).bReleased) { a_or_b = true; delete m_spr; m_spr = xpm_to_sprite_(26, 30, colors, hexagon_v1_variantA_2); }
    if (GetKey(olc::K3).bReleased) { a_or_b = true; delete m_spr; m_spr = xpm_to_sprite_(26, 30, colors, hexagon_v1_variantA_3); }
    if (GetKey(olc::K4).bReleased) { a_or_b = true; delete m_spr; m_spr = xpm_to_sprite_(26, 30, colors, hexagon_v1_variantA_4); }
    if (GetKey(olc::K5).bReleased) { a_or_b = true; delete m_spr; m_spr = xpm_to_sprite_(26, 30, colors, hexagon_v1_variantA_5); }
    if (GetKey(olc::K6).bReleased) { a_or_b = true; delete m_spr; m_spr = xpm_to_sprite_(26, 30, colors, hexagon_v1_variantA_6); }

#ifdef PRJ_DEBUG
    if (GetKey(olc::ESCAPE).bReleased) olc_Terminate();
#endif

    const auto pm = GetPixelMode();
    SetPixelMode(olc::Pixel::ALPHA);

    const vi2d origin(10, 10);
    DrawSprite(origin + vi2d(24 * 0, 0), m_spr);
    DrawSprite(origin + vi2d(24 * 1, 0), m_spr);
    DrawSprite(origin + vi2d(24 * 2, 0), m_spr);

    const auto origin_ = origin; // + vi2d(0, 5);
    DrawSprite(origin_ + vi2d(12 + 24 * 0, 20), m_spr);

    static bool tmp = false;
    if (GetKey(olc::F4).bReleased) tmp = !tmp;
    if (tmp)
    {
        DrawSprite(origin + vi2d(24 * 3, 0), m_spr);
        DrawSprite(origin + vi2d(24 * 4, 0), m_spr);
        DrawSprite(origin + vi2d(24 * 5, 0), m_spr);
        DrawSprite(origin + vi2d(24 * 6, 0), m_spr);

        DrawSprite(origin_ + vi2d(12 + 24 * 1, 20), m_spr);
        DrawSprite(origin_ + vi2d(12 + 24 * 2, 20), m_spr);
        DrawSprite(origin_ + vi2d(12 + 24 * 3, 20), m_spr);
        DrawSprite(origin_ + vi2d(12 + 24 * 4, 20), m_spr);
        DrawSprite(origin_ + vi2d(12 + 24 * 5, 20), m_spr);

        DrawSprite(origin + vi2d(24 * 0, 20 * 2), m_spr);
        DrawSprite(origin + vi2d(24 * 1, 20 * 2), m_spr);
        DrawSprite(origin + vi2d(24 * 2, 20 * 2), m_spr);
        DrawSprite(origin + vi2d(24 * 3, 20 * 2), m_spr);
        DrawSprite(origin + vi2d(24 * 4, 20 * 2), m_spr);
        DrawSprite(origin + vi2d(24 * 5, 20 * 2), m_spr);
        DrawSprite(origin + vi2d(24 * 6, 20 * 2), m_spr);

        DrawSprite(origin_ + vi2d(12 + 24 * 0, 20 * 3), m_spr);
        DrawSprite(origin_ + vi2d(12 + 24 * 1, 20 * 3), m_spr);
        DrawSprite(origin_ + vi2d(12 + 24 * 2, 20 * 3), m_spr);
        DrawSprite(origin_ + vi2d(12 + 24 * 3, 20 * 3), m_spr);
        DrawSprite(origin_ + vi2d(12 + 24 * 4, 20 * 3), m_spr);
        DrawSprite(origin_ + vi2d(12 + 24 * 5, 20 * 3), m_spr);
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
