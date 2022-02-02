/*
author          Oliver Blaser
date            02.02.2022
copyright       OLC-3 - Copyright (c) 2022 Oliver Blaser
*/

#ifndef IG_APP_GAMEGUI_H
#define IG_APP_GAMEGUI_H

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "middleware/gui.h"
#include "middleware/guiControls.h"
#include "middleware/util.h"
#include "project.h"

#include "olcPixelGameEngine.h"



class ResetButton : public gui::Control
{
public:
    static constexpr int32_t radius = 18;

public:
    ResetButton(const olc::vi2d& pos);
    virtual ~ResetButton() {}

    void gameOver(bool state) { m_gameOver = state; }

    virtual bool isMouse(const olc::vi2d& mousePos) const;
    virtual void draw(olc::PixelGameEngine* pge, int drawMode);

private:
    std::unique_ptr<olc::Sprite> spr_click;
    std::unique_ptr<olc::Sprite> spr_expl;
    std::unique_ptr<olc::Sprite> spr_hover;
    std::unique_ptr<olc::Sprite> spr_normal;
    bool m_gameOver;
};



enum GUI_EVENTS
{
    EVT_RESET_CLICK = gui::EVT__begin_custon,
    EVT_LEFT_CLICK,
    EVT_RIGHT_CLICK,
    EVT_ABOUT_CLICK
};

class GameGui : public gui::Manager
{
public:
    using gui::Manager::Manager; // same ctors as gui::Manager
    virtual ~GameGui() {}
protected:
    virtual int update();
private:
    virtual void initControls();

protected:
    ResetButton* btn_reset = nullptr;
    gui::StringButton* btn_left = nullptr;
    gui::StaticText* st_fieldName = nullptr;
    gui::StringButton* btn_right = nullptr;
    gui::StringButton* btn_about = nullptr;
};



#endif // IG_APP_GAMEGUI_H
