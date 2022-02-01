/*
author          Oliver Blaser
date            01.02.2022
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
    spr_hover = std::make_unique<olc::Sprite>("./assets/smiley_hover2.png");
    spr_normal = std::make_unique<olc::Sprite>("./assets/smiley.png");
}

bool ResetButton::isMouse(const olc::vi2d& mousePos) const
{
    const olc::vf2d diff = m_pos - mousePos;
    return (std::abs(diff.mag2()) <= (radius * radius)) && m_enabled;
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



StaticText::StaticText(const olc::vi2d& pos, const std::string& label)
    : Control(pos), m_label(), m_size()
{
    setLabel(label);
}

void StaticText::setLabel(const std::string& label)
{
    m_size = vi2d(16 * label.length() + 1, 16 + 1);
    m_label = label;
}

bool StaticText::isMouse(const olc::vi2d& mousePos) const
{
    return (
        (mousePos.x >= m_pos.x) && (mousePos.x <= (m_pos.x + m_size.x)) &&
        (mousePos.y >= m_pos.y) && (mousePos.y <= (m_pos.y + m_size.y))
        ) && m_enabled;
}

void StaticText::draw(olc::PixelGameEngine* pge, int drawMode)
{
    const vi2d offs(2, 2);

    if (m_enabled)
    {
        pge->DrawString(m_pos + offs, m_label, olc::Pixel(0, 0, 0), 2);
    }
    else pge->DrawString(m_pos + offs, m_label, olc::Pixel(0xC8, 0xC8, 0xC8), 2);

#if defined(PRJ_DEBUG) && 0
    pge->DrawRect(m_pos, m_size, olc::BLUE);
#endif
}



StringButton::StringButton(const olc::vi2d& pos, const std::string& label)
    : StaticText(pos, label)
{}

void StringButton::draw(olc::PixelGameEngine* pge, int drawMode)
{
    const vi2d offs(2, 2);

    if (m_enabled)
    {
        if (drawMode == DM_HOVER) pge->DrawString(m_pos + offs, m_label, olc::Pixel(0x69, 0x69, 0x69), 2);
        else if (drawMode == DM_DOWN) pge->DrawString(m_pos + offs, m_label, olc::Pixel(0xC8, 0xC8, 0xC8), 2);
        else pge->DrawString(m_pos + offs, m_label, olc::Pixel(0, 0, 0), 2);
    }
    else pge->DrawString(m_pos + offs, m_label, olc::Pixel(0xC8, 0xC8, 0xC8), 2);

#if defined(PRJ_DEBUG) && 0
    pge->DrawRect(m_pos, m_size, olc::BLUE);
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

void Gui::enable(bool state)
{
    if (btn_left) btn_left->enable(state);
    if (st_fieldName) st_fieldName->enable(state);
    if (btn_right) btn_right->enable(state);
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
            if (btn_left->id() == mouseHoverId) evt = EVT_LEFT_CLICK;
            if (btn_right->id() == mouseHoverId) evt = EVT_RIGHT_CLICK;
            if (btn_about->id() == mouseHoverId) evt = EVT_ABOUT_CLICK;
        }

        m_mouseDnId = -1;
    }

    m_pge->DrawString(722, 540, std::to_string(mousePos.x) + " " + std::to_string(mousePos.y));

    ctrl_draw(mouseHoverId);

    return evt;
}

void Gui::initControls()
{
    btn_reset = new ResetButton(vi2d(1000 - 130, 60));
    ctrl_add(btn_reset);



    constexpr int32_t arrowYPos = 150;
    constexpr int32_t arrowMargin = 40;

    btn_left = new StringButton(vi2d(740 + arrowMargin, arrowYPos), "<");
    ctrl_add(btn_left);

    st_fieldName = new StaticText(vi2d(822, arrowYPos), "###");
    ctrl_add(st_fieldName);

    btn_right = new StringButton(vi2d(1000 - arrowMargin - 16, arrowYPos), ">");
    ctrl_add(btn_right);



    btn_about = new StringButton(vi2d(830, 250), "about");
#ifndef PRJ_DEBUG
    btn_about->disable();
#endif
    ctrl_add(btn_about);
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
