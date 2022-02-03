/*
author          Oliver Blaser
date            02.02.2022
copyright       OLC-3 - Copyright (c) 2022 Oliver Blaser
*/

#ifndef IG_GUI_CONTROLS_H
#define IG_GUI_CONTROLS_H

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "olcPixelGameEngine.h"


namespace gui
{
    class Control
    {
    public:
        enum STATE
        {
            NORMAL = 0,
            MOUSE_HOVER,
            MOUSE_DOWN
        };

    public:
        Control() : m_enabled(true), m_id(-1), m_pos(-1, -1) {}
        Control(const olc::vi2d& pos) : m_enabled(true), m_id(-1), m_pos(pos) {}
        Control(int id, const olc::vi2d& pos) : m_enabled(true), m_id(id), m_pos(pos) {}
        virtual ~Control() {}

        int getId() const { return m_id; }
        const olc::vi2d& getPos() const { return m_pos; }

        void enable(bool state = true) { m_enabled = state; }
        void disable() { enable(false); }
        void setId(int id) { m_id = id; }
        void setPos(const olc::vi2d& pos) { m_pos = pos; }
        void setPos(int32_t x, int32_t y) { m_pos = olc::vi2d(x, y); }

        bool isEnabled() const { return m_enabled; }

        virtual bool isMouse(const olc::vi2d& mousePos) const = 0;
        virtual void draw(olc::PixelGameEngine* pge, int drawMode) = 0;

    protected:
        bool m_enabled;
        int m_id;
        olc::vi2d m_pos;
    };



    class StaticText : public Control
    {
    public:
        StaticText(const olc::vi2d& pos, const std::string& label);
        virtual ~StaticText() {}

        const olc::vi2d& getSize() const { return m_size; }

        void setLabel(const std::string& label);

        virtual bool isMouse(const olc::vi2d& mousePos) const;
        virtual void draw(olc::PixelGameEngine* pge, int drawMode);

    protected:
        std::string m_label;
        olc::vi2d m_size;
    };



    class StringButton : public StaticText
    {
    public:
        StringButton(const olc::vi2d& pos, const std::string& label);
        virtual ~StringButton() {}

        virtual void draw(olc::PixelGameEngine* pge, int drawMode);
    };
}


#endif // IG_GUI_CONTROLS_H
