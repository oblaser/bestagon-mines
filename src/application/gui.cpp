/*
author          Oliver Blaser
date            31.01.2022
copyright       OLC-3 - Copyright (c) 2022 Oliver Blaser
*/

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

#include "gui.h"
#include "project.h"

#include "olcPixelGameEngine.h"


using olc::vi2d;

namespace
{
    constexpr int ctrlStartId = 1000;
}



ResetButton::ResetButton(const olc::vi2d& pos)
    : Control(pos), m_gameOver(false)
{
    spr_click = std::make_unique<olc::Sprite>("./assets/smiley_click.png");
    spr_expl = std::make_unique<olc::Sprite>("./assets/smiley_expl.png");
    spr_hover = std::make_unique<olc::Sprite>("./assets/smiley_o.png");
    spr_normal = std::make_unique<olc::Sprite>("./assets/smiley.png");
}

bool ResetButton::isMouse(const olc::vi2d& mousePos) const
{
    const olc::vf2d diff = m_pos - mousePos;
    return (std::abs(diff.mag2()) <= (radius * radius));
}

void ResetButton::draw(olc::PixelGameEngine* pge, int drawMode)
{
    const vi2d pos(m_pos.x - 20, m_pos.y - 20);

    const auto pm = pge->GetPixelMode();
    pge->SetPixelMode(olc::Pixel::ALPHA);

    if (drawMode == DM_HOVER) pge->DrawSprite(pos, spr_hover.get());
    else if (drawMode == DM_DOWN) pge->DrawSprite(pos, spr_click.get());
    else
    {
        //pge->FillCircle(m_pos, radius, olc::Pixel(255, 190, 0));
        //pge->DrawCircle(m_pos, radius - 1, olc::Pixel(122, 71, 0));
        //pge->DrawCircle(m_pos, radius, olc::Pixel(122, 71, 0));

        if (m_gameOver) pge->DrawSprite(pos, spr_expl.get());
        else pge->DrawSprite(pos, spr_normal.get());
    }

    pge->SetPixelMode(pm);

#if defined(PRJ_DEBUG) && 0
    pge->DrawCircle(m_pos, radius, olc::Pixel(0, 0, 0));
#endif
}



Gui::Gui(olc::PixelGameEngine* pge)
    : m_mouseDnId(-1), m_pge(pge)
{}

Gui::~Gui()
{
    for (size_t i = 0; i < m_ctrl.size(); ++i)
    {
        delete m_ctrl[i];
    }
}

void Gui::init()
{
    initControls();

    m_ctrl.shrink_to_fit();
}

void Gui::gameOver(bool state)
{
    if (btn_reset) btn_reset->gameOver(state);
}

int Gui::update()
{
    int evt = EVT_NONE;

    const vi2d mousePos = m_pge->GetMousePos();
    const int mouseHoverId = ctrl_getId(mousePos);

    if (m_pge->GetMouse(olc::Mouse::LEFT).bPressed) m_mouseDnId = mouseHoverId;
    if (m_pge->GetMouse(olc::Mouse::LEFT).bReleased)
    {
        if ((mouseHoverId == m_mouseDnId) && (mouseHoverId >= ctrlStartId))
        {
            if (btn_reset->id() == mouseHoverId) evt = EVT_RESET_CLICK;
        }

        m_mouseDnId = -1;
    }

    m_pge->DrawString(722, 540, std::to_string(mousePos.x) + " " + std::to_string(mousePos.y));

    ctrl_draw(mouseHoverId);

    return evt;
}

void Gui::initControls()
{
    btn_reset = new ResetButton(vi2d(1000 - 60, 60));
    ctrl_add(btn_reset);
}

void Gui::ctrl_add(Control* ctrl)
{
    if (ctrl->id() < ctrlStartId) ctrl->setId(ctrl_getNewId());
    m_ctrl.push_back(ctrl);
}

int Gui::ctrl_getId(const olc::vi2d& mousePos)
{
    int r = -1;
    for (size_t i = 0; (i < m_ctrl.size()) && (r == -1); ++i)
    {
        Control*& ctrl = m_ctrl[i];
        if (ctrl->isMouse(mousePos)) r = ctrl->id();
    }
    return r;
}

int Gui::ctrl_getNewId()
{
    int r = ctrlStartId;

    bool exists;
    do
    {
        exists = false;
        for (size_t i = 0; (i < m_ctrl.size()) && !exists; ++i)
        {
            if (m_ctrl[i]->id() == r)
            {
                exists = true;
                ++r;
            }
        }
    }
    while (exists);

    return r;
}

void Gui::ctrl_draw(int mouseHoverId)
{
    const auto pm = m_pge->GetPixelMode();
    m_pge->SetPixelMode(olc::Pixel::NORMAL);

    for (size_t i = 0; i < m_ctrl.size(); ++i)
    {
        Control*& ctrl = m_ctrl[i];

        int dm = Control::DM_NORMAL;
        if (ctrl->id() == m_mouseDnId) dm = Control::DM_DOWN;
        else if (ctrl->id() == mouseHoverId) dm = Control::DM_HOVER;

        ctrl->draw(m_pge, dm);
    }

    m_pge->SetPixelMode(pm);
}
