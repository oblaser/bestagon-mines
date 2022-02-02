/*
author          Oliver Blaser
date            02.02.2022
copyright       OLC-3 - Copyright (c) 2022 Oliver Blaser
*/

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

#include "gameGui.h"
#include "project.h"

#include "olcPixelGameEngine.h"


using olc::vi2d;

namespace
{
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
    // sprites origin is top-left, reset button ctrl pos is midpoint of the circle
    const vi2d pos(m_pos.x - 20, m_pos.y - 20);

    const olc::Pixel pxNormal(0xfc, 0xc1, 0x29);
    const olc::Pixel pxHover(0xfc, 0xde, 0x3e);
    const olc::Pixel pxClick(0xf8, 0xa1, 0x1e);

    const auto pm = pge->GetPixelMode();
    pge->SetPixelMode(olc::Pixel::ALPHA);

    if (drawMode == gui::Control::MOUSE_HOVER)
    {
        if (spr_hover.get()->pColData.empty()) pge->FillCircle(m_pos, radius, pxHover);
        else pge->DrawSprite(pos, spr_hover.get());
    }
    else if (drawMode == gui::Control::MOUSE_DOWN)
    {
        if (spr_click.get()->pColData.empty()) pge->FillCircle(m_pos, radius, pxClick);
        else pge->DrawSprite(pos, spr_click.get());
    }
    else
    {
        if (m_gameOver)
        {
            if (spr_expl.get()->pColData.empty()) pge->FillCircle(m_pos, radius, pxNormal);
            else pge->DrawSprite(pos, spr_expl.get());
        }
        else
        {
            if (spr_normal.get()->pColData.empty()) pge->FillCircle(m_pos, radius, pxNormal);
            else pge->DrawSprite(pos, spr_normal.get());
        }
    }

    pge->SetPixelMode(pm);

#if defined(PRJ_DEBUG) && 0
    pge->DrawCircle(m_pos, radius, olc::Pixel(0, 0, 0));
#endif
}



int GameGui::update()
{
    int evt = gui::EVT_NONE;

    const vi2d mousePos = m_pge->GetMousePos();
    const int mouseHoverId = ctrl_getId(mousePos);

    if (m_pge->GetMouse(olc::Mouse::LEFT).bPressed) m_mouseDnId = mouseHoverId;
    if (m_pge->GetMouse(olc::Mouse::LEFT).bReleased)
    {
        //if ((mouseHoverId == m_mouseDnId) && (mouseHoverId >= ctrlStartId))
        if (mouseHoverId == m_mouseDnId)
        {
            if (btn_reset->getId() == mouseHoverId) evt = EVT_RESET_CLICK;
            if (btn_left->getId() == mouseHoverId) evt = EVT_LEFT_CLICK;
            if (btn_right->getId() == mouseHoverId) evt = EVT_RIGHT_CLICK;
            if (btn_about->getId() == mouseHoverId) evt = EVT_ABOUT_CLICK;
        }

        m_mouseDnId = -1;
    }

    m_pge->DrawString(722, 540, std::to_string(mousePos.x) + " " + std::to_string(mousePos.y));

    ctrl_draw(mouseHoverId);

    return evt;
}

void GameGui::initControls()
{
    btn_reset = new ResetButton(vi2d(1000 - 130, 60));
    addControl(btn_reset);



    constexpr int32_t arrowYPos = 150;
    constexpr int32_t arrowMargin = 40;

    btn_left = new gui::StringButton(vi2d(740 + arrowMargin, arrowYPos), "<");
    addControl(btn_left);

    st_fieldName = new gui::StaticText(vi2d(822, arrowYPos), "###");
    addControl(st_fieldName);

    btn_right = new gui::StringButton(vi2d(1000 - arrowMargin - 16, arrowYPos), ">");
    addControl(btn_right);



    btn_about = new gui::StringButton(vi2d(830, 250), "about");
#ifndef PRJ_DEBUG
    btn_about->disable();
#endif
    addControl(btn_about);
}
