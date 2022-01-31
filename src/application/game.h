/*
author          Oliver Blaser
date            30.01.2022
copyright       OLC-3 - Copyright (c) 2022 Oliver Blaser
*/

#ifndef IG_APP_GAME_H
#define IG_APP_GAME_H

#include <string>
#include <vector>

#include "resources/sprites.h"

#include "olcPixelGameEngine.h"


class Game : public olc::PixelGameEngine, protected SpriteOwner
{
public:
    enum TILE_STATUS
    {
        T_NOSHOW = -1,
        T_OPEN = 0,
        T_1,
        T_2,
        T_3,
        T_4,
        T_5,
        T_6,
        T_CLOSED,
        T_MINE,
        T_EXPLODED,
        T_FLAG
    };

public:
    Game();
    virtual ~Game();

public:
    bool OnUserCreate() override;
    bool OnUserUpdate(float tElapsed) override;
    bool OnUserDestroy() override;

private:
    std::vector<int> m_field;
    std::vector<bool> m_mines;
    size_t m_mouseDnFieldIdx;
    bool m_firstClick;

    int cntMinesAround(size_t fieldIdx);
    void discoverField();
    void distributeField(size_t clickedIdx);
    size_t mousePosToFieldIdx(const olc::vi2d& mousePos, const olc::vi2d& fieldOrig);
    void reset();

};


#endif // IG_APP_GAME_H
