/*
author          Oliver Blaser
date            02.02.2022
copyright       OLC-3 - Copyright (c) 2022 Oliver Blaser
*/

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

#include "guiControls.h"

#include "olcPixelGameEngine.h"


using olc::vi2d;

namespace
{
}



#pragma region StaticText
gui::StaticText::StaticText(const olc::vi2d& pos, const std::string& label)
    : Control(pos), m_label(), m_size()
{
    setLabel(label);
}

void gui::StaticText::setLabel(const std::string& label)
{
    m_size = vi2d(16 * label.length() + 1, 16 + 1);
    m_label = label;
}

bool gui::StaticText::isMouse(const olc::vi2d& mousePos) const
{
    return (
        (mousePos.x >= m_pos.x) && (mousePos.x <= (m_pos.x + m_size.x)) &&
        (mousePos.y >= m_pos.y) && (mousePos.y <= (m_pos.y + m_size.y))
        ) && m_enabled;
}

void gui::StaticText::draw(olc::PixelGameEngine* pge, int drawMode)
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
#pragma endregion


#pragma region StringButton
gui::StringButton::StringButton(const olc::vi2d& pos, const std::string& label)
    : StaticText(pos, label)
{}

void gui::StringButton::draw(olc::PixelGameEngine* pge, int drawMode)
{
    const vi2d offs(2, 2);

    if (m_enabled)
    {
        if (drawMode == gui::Control::MOUSE_HOVER) pge->DrawString(m_pos + offs, m_label, olc::Pixel(0x69, 0x69, 0x69), 2);
        else if (drawMode == gui::Control::MOUSE_DOWN) pge->DrawString(m_pos + offs, m_label, olc::Pixel(0xC8, 0xC8, 0xC8), 2);
        else pge->DrawString(m_pos + offs, m_label, olc::Pixel(0, 0, 0), 2);
    }
    else pge->DrawString(m_pos + offs, m_label, olc::Pixel(0xC8, 0xC8, 0xC8), 2);

#if defined(PRJ_DEBUG) && 0
    pge->DrawRect(m_pos, m_size, olc::BLUE);
#endif
}
#pragma endregion

