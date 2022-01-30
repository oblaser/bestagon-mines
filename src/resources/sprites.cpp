/*
author          Oliver Blaser
date            29.01.2022
copyright       OLC-3 - Copyright (c) 2022 Oliver Blaser
*/

#include <cstddef>
#include <cstdint>
#include <unordered_map>

#include "middleware/xpmSprite.h"
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
        "####ccccccccmmcccccccc####",
        "##ccccccccmmmmmmcccccccc##",
        "##cccccccmmmmmmmmccccccc##",
        "##ccccccmmmmmmmmmmcccccc##",
        "##cccccmmmmmmmmmmmmccccc##",
        "##cccccmmmmmmmmmmmmccccc##",
        "##ccccmmmmmmmmmmmmmmcccc##",
        "##ccccmmmmmmmmmmmmmmcccc##",
        "##cccccmmmmmmmmmmmmccccc##",
        "##cccccmmmmmmmmmmmmccccc##",
        "##ccccccmmmmmmmmmmcccccc##",
        "##cccccccmmmmmmmmccccccc##",
        "##ccccccccmmmmmmcccccccc##",
        "####ccccccccmmcccccccc####",
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
        "            ##            ",
        "          ######          ",
        "         ########         ",
        "       ####cccc####       ",
        "      ####cccccc####      ",
        "    ####cccccccccc####    ",
        "   ####cccccccccccc####   ",
        " ####cccccccccccccccc#### ",
        "####cccccccffccccccccc####",
        "##cccccccccffffccccccccc##",
        "##cccccccccffffffccccccc##",
        "##cccccccccffffffffccccc##",
        "##cccccccccffffffccccccc##",
        "##cccccccccffffccccccccc##",
        "##cccccccccffccccccccccc##",
        "##cccccccccffccccccccccc##",
        "##cccccccccffccccccccccc##",
        "##cccccccccffccccccccccc##",
        "##cccccccccffccccccccccc##",
        "##ccccccccfffffccccccccc##",
        "##cccccccffffffffccccccc##",
        "####ccccffffffffffcccc####",
        " ####cccccccccccccccc#### ",
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
    colors.insert_or_assign(' ', olc::Pixel(127, 127, 127, 0));
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
    colors.insert_or_assign('f', olc::Pixel(255, 89, 0));

    deleteSprites();

    spr_closed = xpm_to_sprite_(26, 30, colors, hexagon_v1_variantA_closed);
    m_sprites.push_back(spr_closed);

    spr_flag = xpm_to_sprite_(26, 30, colors, hexagon_v1_variantA_flag);
    m_sprites.push_back(spr_flag);

    spr_mine = xpm_to_sprite_(26, 30, colors, hexagon_v1_variantA_mine);
    m_sprites.push_back(spr_mine);

    spr_tiles[0] = xpm_to_sprite_(26, 30, colors, hexagon_v1_variantA);
    m_sprites.push_back(spr_tiles[0]);

    spr_tiles[1] = xpm_to_sprite_(26, 30, colors, hexagon_v1_variantA_1);
    m_sprites.push_back(spr_tiles[1]);

    spr_tiles[2] = xpm_to_sprite_(26, 30, colors, hexagon_v1_variantA_2);
    m_sprites.push_back(spr_tiles[2]);

    spr_tiles[3] = xpm_to_sprite_(26, 30, colors, hexagon_v1_variantA_3);
    m_sprites.push_back(spr_tiles[3]);

    spr_tiles[4] = xpm_to_sprite_(26, 30, colors, hexagon_v1_variantA_4);
    m_sprites.push_back(spr_tiles[4]);

    spr_tiles[5] = xpm_to_sprite_(26, 30, colors, hexagon_v1_variantA_5);
    m_sprites.push_back(spr_tiles[5]);

    spr_tiles[6] = xpm_to_sprite_(26, 30, colors, hexagon_v1_variantA_6);
    m_sprites.push_back(spr_tiles[6]);
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
