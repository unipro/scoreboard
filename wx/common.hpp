#ifndef __SCOREBOARD_WX_COMMON_HPP__
#define __SCOREBOARD_WX_COMMON_HPP__

#include <wx/wx.h>
#include <SDL.h>
#ifdef _MSC_VER
#include "../config-win32.h"
#else
#include "../config.h"
#endif

#include "resource.h"

#define N_ELEMENTS(arr) (sizeof(arr) / sizeof((arr)[0]))

#endif  // __SCOREBOARD_WX_COMMON_HPP__
