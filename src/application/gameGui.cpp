/*
author          Oliver Blaser
date            02.02.2022
copyright       OLC-3 - Copyright (c) 2022 Oliver Blaser
*/

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "gameGui.h"
#include "middleware/util.h"
#include "project.h"

#include "olcPixelGameEngine.h"


using olc::vi2d;

namespace
{
}



ResetButton::ResetButton(const olc::vi2d& pos)
    : Control(pos), m_face(0)
{
    spr_click = std::make_unique<olc::Sprite>("./assets/smiley_click.png");
    spr_expl = std::make_unique<olc::Sprite>("./assets/smiley_expl.png");
    spr_hover = std::make_unique<olc::Sprite>("./assets/smiley_hover2.png");
    spr_normal = std::make_unique<olc::Sprite>("./assets/smiley.png");
    spr_won = std::make_unique<olc::Sprite>("./assets/smiley_won.png");
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
    const olc::Pixel pxHover(0xf8, 0xa1, 0x1e);
    const olc::Pixel pxClick(0xfc, 0xde, 0x3e);

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
        if (m_face == 0)
        {
            if (spr_normal.get()->pColData.empty()) pge->FillCircle(m_pos, radius, pxNormal);
            else pge->DrawSprite(pos, spr_normal.get());
        }
        else if (m_face == 1)
        {
            if (spr_expl.get()->pColData.empty()) pge->FillCircle(m_pos, radius, pxNormal);
            else pge->DrawSprite(pos, spr_expl.get());
        }
        else if (m_face == 2)
        {
            if (spr_won.get()->pColData.empty()) pge->FillCircle(m_pos, radius, pxNormal);
            else pge->DrawSprite(pos, spr_won.get());
        }
        else pge->FillCircle(m_pos, radius, olc::Pixel(173, 0, 0));
    }

    pge->SetPixelMode(pm);

#if defined(PRJ_DEBUG) && 0
    pge->DrawCircle(m_pos, radius, olc::Pixel(0, 0, 0));
#endif
}



int GameGui::update()
{
    int evt = gui::EVT_NONE;

    if (m_dispPage == P_GAME)
    {
        const vi2d mousePos = m_pge->GetMousePos();
        const int mouseHoverId = ctrl_getId(mousePos);

        if (m_pge->GetMouse(olc::Mouse::LEFT).bPressed) m_mouseDnId = mouseHoverId;
        if (m_pge->GetMouse(olc::Mouse::LEFT).bReleased)
        {
            if (mouseHoverId == m_mouseDnId)
            {
                if (btn_reset->getId() == mouseHoverId) evt = EVT_RESET_CLICK;
                if (btn_left->getId() == mouseHoverId) evt = EVT_LEFT_CLICK;
                if (btn_right->getId() == mouseHoverId) evt = EVT_RIGHT_CLICK;
                if (btn_about->getId() == mouseHoverId) m_dispPage = P_ABOUT;
            }

            m_mouseDnId = -1;
        }

        ctrl_draw(mouseHoverId);
    }
    else if (m_dispPage == P_ABOUT)
    {
        evt = gui::EVT_POPUP;
        const int evtAbout = p_about.get()->update();
        if (evtAbout == AboutGui::EVT_BACK) m_dispPage = P_GAME;
    }
    else m_pge->DrawString(2, 2, "ERROR: " + std::string(OMW__FILENAME__) + ":" + std::to_string(__LINE__), olc::DARK_MAGENTA, 2);

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
    addControl(btn_about);



    p_about = std::make_unique<AboutGui>(m_pge);
    p_about.get()->init();
}



int AboutGui::update()
{
    int evt = gui::EVT_NONE;

    const vi2d mousePos = m_pge->GetMousePos();
    const int mouseHoverId = ctrl_getId(mousePos);

    if (m_pge->GetMouse(olc::Mouse::LEFT).bPressed) m_mouseDnId = mouseHoverId;
    if (m_pge->GetMouse(olc::Mouse::LEFT).bReleased)
    {
        if (mouseHoverId == m_mouseDnId)
        {
            if (btn_webpage->getId() == mouseHoverId) util::openUrl(prj::website);
            if (btn_olcWebpage->getId() == mouseHoverId) util::openUrl("https://github.com/OneLoneCoder/olcPixelGameEngine");
            if (btn_back->getId() == mouseHoverId) evt = EVT_BACK;
        }

        m_mouseDnId = -1;
    }

    // draw border
    {
        constexpr int32_t width = 5;
        constexpr int32_t margin = 30 - width / 2;
        const int32_t scrnW = m_pge->ScreenWidth();
        const int32_t scrnH = m_pge->ScreenHeight();
        const olc::Pixel px(0xa0, 0xa0, 0xa0);

        const vi2d horizTopSize(390, width);
        const vi2d horizBottomSize(scrnW - 2 * margin, width);
        const vi2d vertSize(width, scrnH - 2 * (width + margin));

        m_pge->FillRect(vi2d(margin, margin), horizTopSize, px);
        m_pge->FillRect(vi2d(scrnW - margin - horizTopSize.x, margin), horizTopSize, px);
        m_pge->FillRect(vi2d(margin, scrnH - margin - width), horizBottomSize, px);

        m_pge->FillRect(vi2d(margin, margin + width), vertSize, px);
        m_pge->FillRect(vi2d(scrnW - margin - width, margin + width), vertSize, px);
    }

    ctrl_draw(mouseHoverId);

    return evt;
}

void AboutGui::initControls()
{
    const int32_t scrnW = m_pge->ScreenWidth();
    const int32_t scrnH = m_pge->ScreenHeight();

    gui::StaticText* st;
    vi2d stSize;

    st = new gui::StaticText(vi2d(), "About");
    stSize = st->getSize();
    st->setPos(scrnW / 2 - stSize.x / 2, 22);
    addControl(st);

    btn_webpage = new gui::StringButton(vi2d(50, 80), "< " + std::string(prj::website) + " >");
    addControl(btn_webpage);


    // https://community.onelonecoder.com/2020/05/20/how-to-attribute-credit-cite-the-olcpixelgameengine/


    btn_olcWebpage = new gui::StringButton(vi2d(50, 100), "< olcPixelGameEngine >");
    addControl(btn_olcWebpage);



    btn_back = new gui::StringButton(vi2d(50, 400), "< back >");
    addControl(btn_back);
}
