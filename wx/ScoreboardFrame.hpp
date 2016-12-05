#ifndef __SCOREBOARDFRAME_HPP__
#define __SCOREBOARDFRAME_HPP__

#include "common.hpp"
#include "../model/GameTime.hpp"
#include "../model/TeamScore.hpp"

extern GameConfig g_config;

class ScoreboardFrame : public wxFrame
{
public:
    static wxSize MIN_SIZE;
    static wxColour BG_COLOR;

public:
    ScoreboardFrame(const wxString& title);
    virtual ~ScoreboardFrame();

    bool SetBackgroundColour(const wxColour& colour);

private:
    void OnQuit(wxCommandEvent& evt);
    void OnAbout(wxCommandEvent& evt);
    void OnSetup(wxCommandEvent& evt);

    void OnTimer(wxTimerEvent& evt);

    void OnNewGame(wxCommandEvent& evt);
    void OnTogglePlay(wxCommandEvent& evt);
    void OnPlayGame(wxCommandEvent& evt);
    void OnPauseGame(wxCommandEvent& evt);

    void OnSetGameClock(wxCommandEvent& evt);
    void OnIncrGameClock(wxCommandEvent& evt);
    void OnDecrGameClock(wxCommandEvent& evt);

    void OnResetShotClock(wxCommandEvent& evt);
    void OnIncrShotClock(wxCommandEvent& evt);
    void OnDecrShotClock(wxCommandEvent& evt);

    void OnIncrLeftScore(wxCommandEvent& evt);
    void OnDecrLeftScore(wxCommandEvent& evt);

    void OnIncrRightScore(wxCommandEvent& evt);
    void OnDecrRightScore(wxCommandEvent& evt);

    void OnIncrLeftFoul(wxCommandEvent& evt);
    void OnDecrLeftFoul(wxCommandEvent& evt);

    void OnIncrRightFoul(wxCommandEvent& evt);
    void OnDecrRightFoul(wxCommandEvent& evt);

    void OnNewPeriod(wxCommandEvent& evt);
    void OnIncrPeriod(wxCommandEvent& evt);
    void OnDecrPeriod(wxCommandEvent& evt);

private:
    void PlayGame();
    void PauseGame();

private:
    // models
    GameTime* m_gameTime;
    TeamScore* m_leftTeam;
    TeamScore* m_rightTeam;

    wxTimer m_timer;
};

#endif  // __SCOREBOARDFRAME_HPP__
