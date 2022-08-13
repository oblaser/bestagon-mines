/*
author          Oliver Blaser
date            13.08.2022
copyright       OLC-3 - Copyright (c) 2022 Oliver Blaser
*/

#include <string>

#include "application/game.h"
#include "project.h"

#ifdef OLC_PLATFORM_WINAPI
#include <Windows.h>
#endif



int main()
{
#ifdef OLC_PLATFORM_WINAPI
    ShowWindow(GetConsoleWindow(), SW_HIDE);
#endif

    Game game;

    constexpr bool vSync =
#ifdef PRJ_DEBUG
        false;
#else
        true;
#endif

    if (game.Construct(1000, 550, 1, 1, false, vSync))
    {
        game.Start();
    }

    return 0;
}
