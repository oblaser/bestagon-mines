/*
author          Oliver Blaser
date            02.02.2022
copyright       OLC-3 - Copyright (c) 2022 Oliver Blaser
*/

#include <cstddef>
#include <cstdint>
#include <unordered_map>

#include "middleware/xpmSprite.h"
#include "project.h"
#include "sprites.h"

#include "olcPixelGameEngine.h"


namespace
{
    const char* const hexagon_v1_variantA[] =
    {
        "            ##            ",
        "          ######          ",
        "         ########         ",
        "       ####....####       ",
        "      ####......####      ",
        "    ####..........####    ",
        "   ####............####   ",
        " ####................#### ",
        "####..................####",
        "##......................##",
        "##......................##",
        "##......................##",
        "##......................##",
        "##......................##",
        "##......................##",
        "##......................##",
        "##......................##",
        "##......................##",
        "##......................##",
        "##......................##",
        "##......................##",
        "####..................####",
        " ####................#### ",
        "   ####............####   ",
        "    ####..........####    ",
        "      ####......####      ",
        "       ####....####       ",
        "         ########         ",
        "          ######          ",
        "            ##            "
    };

    const char* const hexagon_v1_variantA_closed[] =
    {
        "            ##            ",
        "          ######          ",
        "         ########         ",
        "       ####cccc####       ",
        "      ####cccccc####      ",
        "    ####cccccccccc####    ",
        "   ####cccccccccccc####   ",
        " ####cccccccccccccccc#### ",
        "####cccccccccccccccccc####",
        "##cccccccccccccccccccccc##",
        "##cccccccccccccccccccccc##",
        "##cccccccccccccccccccccc##",
        "##cccccccccccccccccccccc##",
        "##cccccccccccccccccccccc##",
        "##cccccccccccccccccccccc##",
        "##cccccccccccccccccccccc##",
        "##cccccccccccccccccccccc##",
        "##cccccccccccccccccccccc##",
        "##cccccccccccccccccccccc##",
        "##cccccccccccccccccccccc##",
        "##cccccccccccccccccccccc##",
        "####cccccccccccccccccc####",
        " ####cccccccccccccccc#### ",
        "   ####cccccccccccc####   ",
        "    ####cccccccccc####    ",
        "      ####cccccc####      ",
        "       ####cccc####       ",
        "         ########         ",
        "          ######          ",
        "            ##            "
    };

    const char* const hexagon_v1_variantA_mine[] =
    {
        "            ##            ",
        "          ######          ",
        "         ########         ",
        "       ####cccc####       ",
        "      ####cccccc####      ",
        "    ####cccccccccc####    ",
        "   ####cccccccccccc####   ",
        " ####cccccccccccccccc#### ",
        "####cccccccccccccccccc####", // "####ccccccccmmcccccccc####",
        "##ccccccccmmmmmmcccccccc##",
        "##cccccccmmmmmmmmccccccc##",
        "##ccccccmmmmmmmmmmcccccc##",
        "##cccccmmmmmmmmmmmmccccc##",
        "##cccccmmmmmmmmmmmmccccc##",
        "##cccccmmmmmmmmmmmmccccc##", // "##ccccmmmmmmmmmmmmmmcccc##",
        "##cccccmmmmmmmmmmmmccccc##", // "##ccccmmmmmmmmmmmmmmcccc##",
        "##cccccmmmmmmmmmmmmccccc##",
        "##cccccmmmmmmmmmmmmccccc##",
        "##ccccccmmmmmmmmmmcccccc##",
        "##cccccccmmmmmmmmccccccc##",
        "##ccccccccmmmmmmcccccccc##",
        "####cccccccccccccccccc####", // "####ccccccccmmcccccccc####",
        " ####cccccccccccccccc#### ",
        "   ####cccccccccccc####   ",
        "    ####cccccccccc####    ",
        "      ####cccccc####      ",
        "       ####cccc####       ",
        "         ########         ",
        "          ######          ",
        "            ##            "
    };

    const char* const hexagon_v1_variantA_flag[] =
    {
        "            ##            ", // "            ##            ",
        "          ######          ", // "          ######          ",
        "         ########         ", // "         ########         ",
        "       ####cccc####       ", // "       ####cccc####       ",
        "      ####cccccc####      ", // "      ####cccccc####      ",
        "    ####cccccccccc####    ", // "    ####cccccccccc####    ",
        "   ####cccccccccccc####   ", // "   ####cccccccccccc####   ",
        " ####cccccccccccccccc#### ", // " ####cccccccccccccccc#### ",
        "####cccccccpfccccccccc####", // "####cccccccffccccccccc####",
        "##cccccccccpfffccccccccc##", // "##cccccccccffffccccccccc##",
        "##cccccccccpfffffccccccc##", // "##cccccccccffffffccccccc##",
        "##cccccccccpfffffffccccc##", // "##cccccccccffffffffccccc##",
        "##cccccccccpfffffccccccc##", // "##cccccccccffffffccccccc##",
        "##cccccccccpfffccccccccc##", // "##cccccccccffffccccccccc##",
        "##cccccccccpfccccccccccc##", // "##cccccccccffccccccccccc##",
        "##cccccccccppccccccccccc##", // "##cccccccccffccccccccccc##",
        "##cccccccccppccccccccccc##", // "##cccccccccffccccccccccc##",
        "##cccccccccppccccccccccc##", // "##cccccccccffccccccccccc##",
        "##cccccccccppccccccccccc##", // "##cccccccccffccccccccccc##",
        "##ccccccccmmmmmccccccccc##", // "##ccccccccfffffccccccccc##",
        "##cccccccmmmmmmmmccccccc##", // "##cccccccffffffffccccccc##",
        "####ccccmmmmmmmmmmcccc####", // "####ccccffffffffffcccc####",
        " ####cccccccccccccccc#### ", // " ####cccccccccccccccc#### ",
        "   ####cccccccccccc####   ", // "   ####cccccccccccc####   ",
        "    ####cccccccccc####    ", // "    ####cccccccccc####    ",
        "      ####cccccc####      ", // "      ####cccccc####      ",
        "       ####cccc####       ", // "       ####cccc####       ",
        "         ########         ", // "         ########         ",
        "          ######          ", // "          ######          ",
        "            ##            "  // "            ##            "
    };

    const char* const hexagon_v1_variantA_flag_wrong[] =
    {
        "            ##            ",
        "          ######          ",
        "         ########         ",
        "       ####cccc####       ",
        "      ####cccccc####      ",
        "    ####cccccccccc####    ",
        "   ####cccccccccccc####   ",
        " ####@@cccccccccccc@@#### ",
        "####c@@@cccpfccccc@@@c####",
        "##cccc@@@ccpfffcc@@@cccc##",
        "##ccccc@@@cpffff@@@ccccc##",
        "##cccccc@@@pfff@@@fccccc##",
        "##ccccccc@@@ff@@@ccccccc##",
        "##cccccccc@@@@@@cccccccc##",
        "##ccccccccc@@@@ccccccccc##",
        "##ccccccccc@@@@ccccccccc##",
        "##cccccccc@@@@@@cccccccc##",
        "##ccccccc@@@pc@@@ccccccc##",
        "##cccccc@@@ppcc@@@cccccc##",
        "##ccccc@@@mmmmmc@@@ccccc##",
        "##cccc@@@mmmmmmmm@@@cccc##",
        "####c@@@mmmmmmmmmm@@@c####",
        " ####@@cccccccccccc@@#### ",
        "   ####cccccccccccc####   ",
        "    ####cccccccccc####    ",
        "      ####cccccc####      ",
        "       ####cccc####       ",
        "         ########         ",
        "          ######          ",
        "            ##            "
    };

    const char* const hexagon_v1_variantA_1[] =
    {
        "            ##            ",
        "          ######          ",
        "         ########         ",
        "       ####....####       ",
        "      ####......####      ",
        "    ####..........####    ",
        "   ####............####   ",
        " ####................#### ",
        "####........11........####",
        "##..........11..........##",
        "##........1111..........##",
        "##........1111..........##",
        "##......11..11..........##",
        "##......11..11..........##",
        "##..........11..........##",
        "##..........11..........##",
        "##..........11..........##",
        "##..........11..........##",
        "##..........11..........##",
        "##..........11..........##",
        "##......1111111111......##",
        "####....1111111111....####",
        " ####................#### ",
        "   ####............####   ",
        "    ####..........####    ",
        "      ####......####      ",
        "       ####....####       ",
        "         ########         ",
        "          ######          ",
        "            ##            "
    };

    const char* const hexagon_v1_variantA_2[] =
    {
        "            ##            ",
        "          ######          ",
        "         ########         ",
        "       ####....####       ",
        "      ####......####      ",
        "    ####..........####    ",
        "   ####............####   ",
        " ####................#### ",
        "####......222222......####",
        "##........222222........##",
        "##......22......22......##",
        "##......22......22......##",
        "##..............22......##",
        "##..............22......##",
        "##............22........##",
        "##............22........##",
        "##..........22..........##",
        "##..........22..........##",
        "##........22............##",
        "##........22............##",
        "##......2222222222......##",
        "####....2222222222....####",
        " ####................#### ",
        "   ####............####   ",
        "    ####..........####    ",
        "      ####......####      ",
        "       ####....####       ",
        "         ########         ",
        "          ######          ",
        "            ##            "
    };

    const char* const hexagon_v1_variantA_3[] =
    {
        "            ##            ",
        "          ######          ",
        "         ########         ",
        "       ####....####       ",
        "      ####......####      ",
        "    ####..........####    ",
        "   ####............####   ",
        " ####................#### ",
        "####......333333......####",
        "##........333333........##",
        "##......33......33......##",
        "##......33......33......##",
        "##..............33......##",
        "##..............33......##",
        "##..........3333........##",
        "##..........3333........##",
        "##..............33......##",
        "##..............33......##",
        "##......33......33......##",
        "##......33......33......##",
        "##........333333........##",
        "####......333333......####",
        " ####................#### ",
        "   ####............####   ",
        "    ####..........####    ",
        "      ####......####      ",
        "       ####....####       ",
        "         ########         ",
        "          ######          ",
        "            ##            "
    };

    const char* const hexagon_v1_variantA_4[] =
    {
        "            ##            ",
        "          ######          ",
        "         ########         ",
        "       ####....####       ",
        "      ####......####      ",
        "    ####..........####    ",
        "   ####............####   ",
        " ####................#### ",
        "####..........44......####",
        "##............44........##",
        "##..........4444........##",
        "##..........4444........##",
        "##........44..44........##",
        "##........44..44........##",
        "##......44....44........##",
        "##......44....44........##",
        "##......4444444444......##",
        "##......4444444444......##",
        "##............44........##",
        "##............44........##",
        "##............44........##",
        "####..........44......####",
        " ####................#### ",
        "   ####............####   ",
        "    ####..........####    ",
        "      ####......####      ",
        "       ####....####       ",
        "         ########         ",
        "          ######          ",
        "            ##            "
    };

    const char* const hexagon_v1_variantA_5[] =
    {
        "            ##            ",
        "          ######          ",
        "         ########         ",
        "       ####....####       ",
        "      ####......####      ",
        "    ####..........####    ",
        "   ####............####   ",
        " ####................#### ",
        "####....5555555555....####",
        "##......5555555555......##",
        "##......55..............##",
        "##......55..............##",
        "##......55555555........##",
        "##......55555555........##",
        "##..............55......##",
        "##..............55......##",
        "##..............55......##",
        "##..............55......##",
        "##..............55......##",
        "##..............55......##",
        "##......55555555........##",
        "####....55555555......####",
        " ####................#### ",
        "   ####............####   ",
        "    ####..........####    ",
        "      ####......####      ",
        "       ####....####       ",
        "         ########         ",
        "          ######          ",
        "            ##            "
    };

    const char* const hexagon_v1_variantA_6[] =
    {
        "            ##            ",
        "          ######          ",
        "         ########         ",
        "       ####....####       ",
        "      ####......####      ",
        "    ####..........####    ",
        "   ####............####   ",
        " ####................#### ",
        "####........6666......####",
        "##..........6666........##",
        "##........66............##",
        "##........66............##",
        "##......66..............##",
        "##......66..............##",
        "##......66666666........##",
        "##......66666666........##",
        "##......66......66......##",
        "##......66......66......##",
        "##......66......66......##",
        "##......66......66......##",
        "##........666666........##",
        "####......666666......####",
        " ####................#### ",
        "   ####............####   ",
        "    ####..........####    ",
        "      ####......####      ",
        "       ####....####       ",
        "         ########         ",
        "          ######          ",
        "            ##            "
    };
}



SpriteOwner::SpriteOwner()
    : m_sprites(0)
{}

SpriteOwner::~SpriteOwner()
{
    deleteSprites();
}

void SpriteOwner::loadSprites()
{
    colors.insert_or_assign(' ', olc::Pixel(0xff, 0x00, 0xcc, 0));
    colors.insert_or_assign('#', olc::Pixel(0xa0, 0xa0, 0xa0)); // border
    //colors.insert_or_assign('#', olc::Pixel(0xba, 0xba, 0xba)); // border
    colors.insert_or_assign('.', olc::Pixel(0xe8, 0xe8, 0xe8)); // open tile background
    colors.insert_or_assign('c', olc::Pixel(0x90, 0x90, 0x90)); // closed tile
    colors.insert_or_assign('1', olc::Pixel(0x00, 0x34, 0xa3));
    colors.insert_or_assign('2', olc::Pixel(0x8b, 0x00, 0x00));
    colors.insert_or_assign('3', olc::Pixel(0xc3, 0x00, 0x00));
    colors.insert_or_assign('4', olc::Pixel(0xff, 0x00, 0x00));
    colors.insert_or_assign('5', olc::Pixel(0xff, 0x00, 0x00));
    colors.insert_or_assign('6', olc::Pixel(0xff, 0x00, 0x00));
    colors.insert_or_assign('m', olc::Pixel(0x33, 0x21, 0x04));
    colors.insert_or_assign('p', olc::Pixel(0, 0, 0));
    colors.insert_or_assign('f', olc::Pixel(0xff, 0x1e, 0x00));
    colors.insert_or_assign('@', olc::Pixel(0x9D, 0x12, 0x00));

    deleteSprites();

    spr_closed = xpm_to_sprite_(26, 30, colors, hexagon_v1_variantA_closed);
    m_sprites.push_back(spr_closed);

    spr_flag = xpm_to_sprite_(26, 30, colors, hexagon_v1_variantA_flag);
    m_sprites.push_back(spr_flag);

    spr_flag_wrong = xpm_to_sprite_(26, 30, colors, hexagon_v1_variantA_flag_wrong);
    m_sprites.push_back(spr_flag_wrong);

    spr_mine = xpm_to_sprite_(26, 30, colors, hexagon_v1_variantA_mine);
    m_sprites.push_back(spr_mine);

    spr_num[0] = xpm_to_sprite_(26, 30, colors, hexagon_v1_variantA);
    m_sprites.push_back(spr_num[0]);

    spr_num[1] = xpm_to_sprite_(26, 30, colors, hexagon_v1_variantA_1);
    m_sprites.push_back(spr_num[1]);

    spr_num[2] = xpm_to_sprite_(26, 30, colors, hexagon_v1_variantA_2);
    m_sprites.push_back(spr_num[2]);

    spr_num[3] = xpm_to_sprite_(26, 30, colors, hexagon_v1_variantA_3);
    m_sprites.push_back(spr_num[3]);

    spr_num[4] = xpm_to_sprite_(26, 30, colors, hexagon_v1_variantA_4);
    m_sprites.push_back(spr_num[4]);

    spr_num[5] = xpm_to_sprite_(26, 30, colors, hexagon_v1_variantA_5);
    m_sprites.push_back(spr_num[5]);

    spr_num[6] = xpm_to_sprite_(26, 30, colors, hexagon_v1_variantA_6);
    m_sprites.push_back(spr_num[6]);



    colors.insert_or_assign('f', olc::Pixel(0x00, 0x7F, 0x00));
    spr_flag_correct = xpm_to_sprite_(26, 30, colors, hexagon_v1_variantA_flag);
    m_sprites.push_back(spr_flag_correct);

    colors.insert_or_assign('m', olc::Pixel(173, 0, 0));
    spr_exploded = xpm_to_sprite_(26, 30, colors, hexagon_v1_variantA_mine);
    m_sprites.push_back(spr_exploded);

    colors.insert_or_assign('m', olc::Pixel(0, 127, 0));
    spr_mineFound = xpm_to_sprite_(26, 30, colors, hexagon_v1_variantA_mine);
    m_sprites.push_back(spr_mineFound);

    colors.insert_or_assign('c', olc::Pixel(173, 0, 0));
    spr_error = xpm_to_sprite_(26, 30, colors, hexagon_v1_variantA_closed);
    m_sprites.push_back(spr_error);

#ifdef PRJ_DEBUG
    colors.insert_or_assign('c', olc::Pixel(0, 123, 255, 150));
    dbg_spr_cursor = xpm_to_sprite_(26, 30, colors, hexagon_v1_variantA_closed);
    m_sprites.push_back(dbg_spr_cursor);
#endif

    m_sprites.shrink_to_fit();
}

void SpriteOwner::deleteSprites()
{
    for (size_t i = 0; i < m_sprites.size(); ++i)
    {
        delete m_sprites[i];
    }

    m_sprites.clear();
}












/*

const char* const hexagon_v1_variantB[] =
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

*/
