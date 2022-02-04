/*
author          Oliver Blaser
date            28.01.2022
copyright       OLC-3 - Copyright (c) 2022 Oliver Blaser
*/

#ifndef IG_PROJECT_H
#define IG_PROJECT_H

//#include <omw/defs.h>
//#include <omw/version.h>


namespace prj
{
    const char* const appDirName = "bestagon-mines";

    const char* const appName = "Bestagon Mines";
    const char* const exeName = "bestagon-mines"; // eq to the linker settings

    //const omw::Version version(0, 1, 0, "alpha");
    const char* const versionStr = "1.0.0";

    const char* const website = "https://github.com/oblaser/bestagon-mines";
}


//#ifdef OMW_DEBUG
#ifdef _DEBUG
#define PRJ_DEBUG (1)
#else
#undef PRJ_DEBUG
#endif


#endif // IG_PROJECT_H
