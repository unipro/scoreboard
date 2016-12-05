#ifndef __GAMETIMEPANEL_HPP__
#define __GAMETIMEPANEL_HPP__

#include "common.hpp"
#include "../model/Interfaces.hpp"
#include "LcdNumberWindow.hpp"

class GameTimePanel : public wxPanel, public IGameTimeView
{
public:
    GameTimePanel(wxWindow* parent);

    // IGameTimeView methods
    void UpdateGameClock(int rest);
    void UpdateShotClock(int rest);
    void UpdatePeriod(int n);
    void StartBuzzer();
    void StopBuzzer();

private:
    LcdNumberWindow* m_timeCtrl;
    LcdNumberWindow* m_shotClockCtrl;
    LcdNumberWindow* m_periodCtrl;
    bool m_buzzing;
};

#endif  // __GAMETIMEPANEL_HPP__
