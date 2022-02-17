/*
author          Oliver Blaser
date            17.02.2022
copyright       OLC-3 - Copyright (c) 2022 Oliver Blaser
*/

#ifndef IG_RES_SPRITES_H
#define IG_RES_SPRITES_H

#include <cstddef>
#include <cstdint>
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



struct TileColors
{
    olc::Pixel border = olc::Pixel(0xa0, 0xa0, 0xa0); // olc::Pixel(0xba, 0xba, 0xba)
    olc::Pixel closed = olc::Pixel(0x90, 0x90, 0x90);
    olc::Pixel openBg = olc::Pixel(0xe8, 0xe8, 0xe8);
    olc::Pixel num1 = olc::Pixel(0x00, 0x34, 0xa3);
    olc::Pixel num2 = olc::Pixel(0x8b, 0x00, 0x00);
    olc::Pixel num3 = olc::Pixel(0xc3, 0x00, 0x00);
    olc::Pixel num4 = olc::Pixel(0xff, 0x00, 0x00);
    olc::Pixel num5 = olc::Pixel(0xff, 0x00, 0x00);
    olc::Pixel num6 = olc::Pixel(0xff, 0x00, 0x00);
    olc::Pixel mine = olc::Pixel(0x33, 0x21, 0x04);
    olc::Pixel mineExploded = olc::Pixel(173, 0, 0);
    olc::Pixel mineCorrect = olc::Pixel(0, 127, 0);
    olc::Pixel pole = olc::Pixel(0, 0, 0);
    olc::Pixel flag = olc::Pixel(0xff, 0x1e, 0x00);
    olc::Pixel flagCorrect = olc::Pixel(0x00, 0x7F, 0x00);
    olc::Pixel flagStrike = olc::Pixel(0x9D, 0x12, 0x00);
};



class SpriteOwner
{
public:
    SpriteOwner();
    virtual ~SpriteOwner();

    void loadSprites(const TileColors& col, bool useSmoothNumbers);

protected:
    olc::Sprite* spr_closed = nullptr;
    olc::Sprite* spr_error = nullptr;
    olc::Sprite* spr_exploded = nullptr;
    olc::Sprite* spr_flag = nullptr;
    olc::Sprite* spr_flag_correct = nullptr;
    olc::Sprite* spr_flag_wrong = nullptr;
    olc::Sprite* spr_mine = nullptr;
    olc::Sprite* spr_mineFound = nullptr;
    olc::Sprite* spr_num[7];

#ifdef PRJ_DEBUG
    olc::Sprite* dbg_spr_cursor = nullptr;
#endif

private:
    std::vector<olc::Sprite*> m_sprites; // copies of the sprite pointers, to delete in a loop...

    void deleteSprites();
};


#endif // IG_RES_SPRITES_H
