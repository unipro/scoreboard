#ifndef __SCOREBOARD_WX_MAIN_H__
#define __SCOREBOARD_WX_MAIN_H__

#include "common.hpp"

class MyApp : public wxApp
{
public:
    bool OnInit();
    int OnRun();
    int OnExit();
};

#endif  // __SCOREBOARD_WX_MAIN_H__
