#ifndef __GAMECONTROLPANEL_HPP__
#define __GAMECONTROLPANEL_HPP__

#include "common.hpp"
#include "../model/Interfaces.hpp"

class GameControlPanel : public wxPanel, public IGameControlView
{
public:
    GameControlPanel(wxWindow* parent);

    // IGameControlView methods
    void EnablePlay(bool enable);
    void EnablePause(bool enable);

private:
    wxButton* m_playButton;
    wxButton* m_pauseButton;
};

#endif  // __GAMECONTROLPANEL_HPP__
