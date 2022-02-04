/*
author          Oliver Blaser
date            04.02.2022
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

    void gameOver() { m_face = 1; }
    void resetSmiley() { m_face = 0; }
    void won() { m_face = 2; }

    virtual bool isMouse(const olc::vi2d& mousePos) const;
    virtual void draw(olc::PixelGameEngine* pge, int drawMode);

private:
    std::unique_ptr<olc::Sprite> spr_click;
    std::unique_ptr<olc::Sprite> spr_expl;
    std::unique_ptr<olc::Sprite> spr_hover;
    std::unique_ptr<olc::Sprite> spr_normal;
    std::unique_ptr<olc::Sprite> spr_won;
    int m_face;
};



class AboutGui : public gui::Manager
{
public:
    enum EVENTS
    {
        EVT_BACK = gui::EVT__begin_custon
    };

public:
    using gui::Manager::Manager; // same ctors as gui::Manager
    virtual ~AboutGui() {}

    virtual int update();

private:
    virtual void initControls();

protected:
    gui::StringButton* btn_webpage = nullptr;
    gui::StringButton* btn_olcWebpage = nullptr;
    gui::StringButton* btn_back = nullptr;
};



enum GUI_EVENTS
{
    EVT_RESET_CLICK = gui::EVT__begin_custon,
    EVT_LEFT_CLICK,
    EVT_RIGHT_CLICK,
    EVT_RNM_DN_CLICK,
    EVT_RNM_UP_CLICK
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
    gui::StaticText* st_nRemaining = nullptr;

    gui::StringButton* btn_left = nullptr;
    gui::StaticText* st_fieldName = nullptr;
    gui::StringButton* btn_right = nullptr;

    gui::StringButton* btn_rnmDn = nullptr;
    gui::StaticText* st_rnm = nullptr;
    gui::StringButton* btn_rnmUp = nullptr;

    gui::StringButton* btn_about = nullptr;

private: // pages, frames, panels, or what ever they're called
    std::unique_ptr<AboutGui> p_about;

    enum DISPLAY_PAGE
    {
        P_GAME = 0,
        P_ABOUT,
        P__end_
    };
    
    int m_dispPage = P_GAME;
};


#endif // IG_APP_GAMEGUI_H
