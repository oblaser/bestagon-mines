/*
author          Oliver Blaser
date            30.01.2022
copyright       OLC-3 - Copyright (c) 2022 Oliver Blaser
*/

#ifndef IG_RES_SPRITES_H
#define IG_RES_SPRITES_H

#include <cstddef>
#include <cstdint>
#include <unordered_map>
#include <vector>

#include "project.h"

#include "olcPixelGameEngine.h"


#define SPR_XOFF (24) // x offset per iteration
#define SPR_YOFF (20) // y offset per iteration
#define SPR_XOFF_OR (12) // base X offset in odd row


const char* const scanHexagon[] =
{
    "[[[[[[[[[[[[##]]]]]]]]]]]]",
    "[[[[[[[[[[######]]]]]]]]]]",
    "[[[[[[[[[########]]]]]]]]]",
    "[[[[[[[####....####]]]]]]]",
    "[[[[[[####......####]]]]]]",
    "[[[[####..........####]]]]",
    "[[[####............####]]]",
    "[####................####]",
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
    "{####................####}",
    "{{{####............####}}}",
    "{{{{####..........####}}}}",
    "{{{{{{####......####}}}}}}",
    "{{{{{{{####....####}}}}}}}",
    "{{{{{{{{{########}}}}}}}}}",
    "{{{{{{{{{{######}}}}}}}}}}",
    "{{{{{{{{{{{{##}}}}}}}}}}}}"
};
constexpr char scanTL = '[';
constexpr char scanTR = ']';
constexpr char scanBL = '{';
constexpr char scanBR = '}';
constexpr char scanBorder = '#';


class SpriteOwner
{
public:
    SpriteOwner();
    virtual ~SpriteOwner();

    void loadSprites();

protected:
    std::unordered_map<char, olc::Pixel> colors;

    olc::Sprite* spr_closed = nullptr;
    olc::Sprite* spr_error = nullptr;
    olc::Sprite* spr_exploded = nullptr;
    olc::Sprite* spr_flag = nullptr;
    olc::Sprite* spr_mine = nullptr;
    olc::Sprite* spr_num[7];

#ifdef PRJ_DEBUG
    olc::Sprite* dbg_spr_cursor = nullptr;
#endif

private:
    std::vector<olc::Sprite*> m_sprites; // copies of the sprite pointers, to delete in a loop...

    void deleteSprites();
};


#endif // IG_RES_SPRITES_H
