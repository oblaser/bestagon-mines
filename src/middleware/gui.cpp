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

#include "gui.h"
#include "guiControls.h"

#include "olcPixelGameEngine.h"


using olc::vi2d;

namespace
{
    constexpr int ctrlStartId = 1000;
}



#pragma region Manager
gui::Manager::Manager(olc::PixelGameEngine* pge)
    : m_mouseDnId(-1), m_pge(pge)
{}

gui::Manager::~Manager()
{
    for (size_t i = 0; i < m_ctrl.size(); ++i)
    {
        delete m_ctrl[i];
    }
}

void gui::Manager::init()
{
    initControls();

    m_ctrl.shrink_to_fit();
}

void gui::Manager::ctrl_add(gui::Control* ctrl)
{
    if (ctrl->getId() < ctrlStartId) ctrl->setId(ctrl_getNewId());

    m_ctrl.push_back(ctrl);
}

int gui::Manager::ctrl_getId(const olc::vi2d& mousePos)
{
    int r = -1;
    for (size_t i = 0; (i < m_ctrl.size()) && (r == -1); ++i)
    {
        gui::Control*& ctrl = m_ctrl[i];
        if (ctrl->isMouse(mousePos)) r = ctrl->getId();
    }
    return r;
}

int gui::Manager::ctrl_getNewId()
{
    int r = ctrlStartId;

    bool exists;
    do
    {
        exists = false;
        for (size_t i = 0; (i < m_ctrl.size()) && !exists; ++i)
        {
            if (m_ctrl[i]->getId() == r)
            {
                exists = true;
                ++r;
            }
        }
    }
    while (exists);

    return r;
}

void gui::Manager::ctrl_draw(int mouseHoverId)
{
    const auto pm = m_pge->GetPixelMode();
    m_pge->SetPixelMode(olc::Pixel::NORMAL);

    for (size_t i = 0; i < m_ctrl.size(); ++i)
    {
        Control*& ctrl = m_ctrl[i];

        int dm = gui::Control::NORMAL;
        if (ctrl->getId() == m_mouseDnId) dm = gui::Control::MOUSE_DOWN;
        else if (ctrl->getId() == mouseHoverId) dm = gui::Control::MOUSE_HOVER;

        ctrl->draw(m_pge, dm);
    }

    m_pge->SetPixelMode(pm);
}
#pragma endregion
