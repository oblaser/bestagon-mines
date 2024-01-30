/*
author          Oliver Blaser
date            02.02.2022
copyright       OLC-3 - Copyright (c) 2022 Oliver Blaser
*/

#ifndef IG_GUI_H
#define IG_GUI_H

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "guiControls.h"

#include "olcPixelGameEngine.h"


namespace gui
{
    enum EVENT
    {
        EVT__begin_system = 0,

        EVT_NONE = 0,


    // flags, not events
        EVT_POPUP, // indicating that the gui needs full control over PGE


        EVT__end_system,
        EVT__begin_custon = 10000,
    };

    class Manager
    {
    public:
        Manager(olc::PixelGameEngine* pge);
        virtual ~Manager();

        void init();

    protected:
        virtual int update() = 0;
        virtual void initControls() = 0;

        void addControl(gui::Control* ctrl) { ctrl_add(ctrl); }

    protected:
        int m_mouseDnId;
        olc::PixelGameEngine* const m_pge = nullptr;


    private:
        std::vector<gui::Control*> m_ctrl;
        void ctrl_add(gui::Control* ctrl);
    protected: // tmp
        int ctrl_getId(const olc::vi2d& mousePos);
    private: // tmp
        int ctrl_getNewId();
    protected: // tmp
        void ctrl_draw(int mouseHoverId);
    private: // tmp

    private: // make not-copyable
        Manager(const gui::Manager& other) = delete;
        gui::Manager& operator=(const gui::Manager&);
    };
}


#endif // IG_GUI_H
