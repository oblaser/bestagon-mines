/*
author          Oliver Blaser
date            31.01.2022
copyright       OLC-3 - Copyright (c) 2022 Oliver Blaser
*/

#ifndef IG_APP_GUI_H
#define IG_APP_GUI_H

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "project.h"

#include "olcPixelGameEngine.h"



#pragma region internal

class Control
{
public:
    enum DRAW_MODE
    {
        DM_NORMAL = 0,
        DM_HOVER, // mouse hover
        DM_DOWN // mouse down
    };

public:
    Control() : m_id(-1), m_pos(-1, -1) {}
    Control(const olc::vi2d& pos) : m_id(-1), m_pos(pos) {}
    Control(int id, const olc::vi2d& pos) : m_id(id), m_pos(pos) {}
    virtual ~Control() {}

    int id() const { return m_id; }
    const olc::vi2d& pos() const { return m_pos; }

    void setId(int id) { m_id = id; }

    virtual bool isMouse(const olc::vi2d& mousePos) const = 0;
    virtual void draw(olc::PixelGameEngine* pge, int drawMode) = 0;

protected:
    int m_id;
    olc::vi2d m_pos;
};

class ResetButton : public Control
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

#pragma endregion



class Gui
{
public:
    enum EVT
    {
        EVT_NONE = 0,
        EVT_RESET_CLICK,
        EVT_LEFT_CLICK,
        EVT_RIGHT_CLICK,
        EVT_ABOUT_CLICK
    };

public:
    Gui(olc::PixelGameEngine* pge);
    ~Gui();

    void init();

    void gameOver(bool state);

    int update();

private:
    int m_mouseDnId;
    olc::PixelGameEngine* const m_pge = nullptr;

    ResetButton* btn_reset = nullptr;
    ResetButton* btn_about = nullptr;

    void initControls();

private:
    std::vector<Control*> m_ctrl;
    void ctrl_add(Control* ctrl);
    int ctrl_getId(const olc::vi2d& mousePos);
    int ctrl_getNewId();
    void ctrl_draw(int mouseHoverId);
};



#endif // IG_APP_GUI_H
