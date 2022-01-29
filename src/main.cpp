/*
author          Oliver Blaser
date            29.01.2022
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

    //if (game.Construct(480, 480, 1, 1))
    if (game.Construct(200, 120, 8, 8))
    {
        game.Start();
    }
    
    return 0;
}
