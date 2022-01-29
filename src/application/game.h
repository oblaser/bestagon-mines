/*
author          Oliver Blaser
date            29.01.2022
copyright       OLC-3 - Copyright (c) 2022 Oliver Blaser
*/

#ifndef IG_APP_GAME_H
#define IG_APP_GAME_H

#include <memory>
#include <string>

//#include "something"

#include "olcPixelGameEngine.h"


class Game : public olc::PixelGameEngine
{
public:
    Game();

public:
    bool OnUserCreate() override;
    bool OnUserUpdate(float tElapsed) override;
    bool OnUserDestroy() override;

private:
    std::unique_ptr<olc::Sprite> m_spr;
};


#endif // IG_APP_GAME_H
