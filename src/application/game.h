/*
author          Oliver Blaser
date            04.02.2022
copyright       OLC-3 - Copyright (c) 2022 Oliver Blaser
*/

#ifndef IG_APP_GAME_H
#define IG_APP_GAME_H

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

#include "gameGui.h"
#include "resources/sprites.h"

#include "olcPixelGameEngine.h"


class Game : public olc::PixelGameEngine, protected SpriteOwner, public GameGui
{
public:
    enum TILE_STATUS
    {
        T_NOSHOW = -1,

        // order is important!
        T_OPEN = 0,
        T_1,
        T_2,
        T_3,
        T_4,
        T_5,
        T_6,
        T_CLOSED,
        // end important order

        T_MINE,
        T_MINE_FOUND,
        T_EXPLODED,
        T_FLAG,
        T_FLAG_CORRECT,
        T_FLAG_WRONG
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
    size_t m_mouseRDnFieldIdx;
    bool m_firstClick;
    bool m_stateIsPlaying;
    int m_fieldVariant;
    double m_relNMines;

    int cntMinesAround(size_t fieldIdx);
    int cntMinesAround(int32_t x, int32_t y);
    void createField();
    bool iterateField();
    void distributeField(size_t clickedIdx);
    size_t mousePosToFieldIdx(const olc::vi2d& mousePos, const olc::vi2d& fieldOrig);
    void reset();
    void updateGame(float tElapsed, int guiEvt);
    void updateRemaining();
};


#endif // IG_APP_GAME_H
