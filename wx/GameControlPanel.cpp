#include "GameControlPanel.hpp"
#include "GameTimePanel.hpp"

// +--------------+--------------+--------------+--------------+
// |              |              |              |              |
// |    Score     |     Time     |              |    Score     |
// |              |              |              |              |
// +--------------+--------------+--------------+--------------+
// |              |              |              |              |
// |    Fouls     |  Shot Clock  |    Period    |    Fouls     |
// |              |              |              |              |
// +--------------+--------------+--------------+--------------+

GameControlPanel::GameControlPanel(wxWindow* parent)
    : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize,
              wxTAB_TRAVERSAL | wxBORDER_RAISED)
    , m_playButton(NULL)
    , m_pauseButton(NULL)

{
    wxGridSizer *bgGrid = new wxGridSizer(2, 4, 10, 10);

    // (0, 0)
    wxStaticBoxSizer *lScoreBox = new wxStaticBoxSizer(wxVERTICAL, this,
                                                       wxT("Home Score"));
    lScoreBox->Add(new wxButton(this, ID_INCR_LEFT_SCORE, wxT("+")), 0,
                   wxALIGN_CENTER);
    lScoreBox->Add(new wxButton(this, ID_DECR_LEFT_SCORE, wxT("-")), 0,
                   wxALIGN_CENTER);
    bgGrid->Add(lScoreBox, 1, wxEXPAND | wxALIGN_TOP | wxALIGN_CENTER);

    // (1, 0)
    wxStaticBoxSizer *timeBox = new wxStaticBoxSizer(wxVERTICAL, this,
                                                     wxT("Game"));
    timeBox->Add(new wxButton(this, ID_NEW_GAME, wxT("New")), 0,
                 wxALIGN_CENTER);
    m_playButton = new wxButton(this, ID_PLAY_GAME, wxT("Play"));
    timeBox->Add(m_playButton, 0, wxALIGN_CENTER);
    m_pauseButton = new wxButton(this, ID_PAUSE_GAME, wxT("Pause"));
    timeBox->Add(m_pauseButton, 0, wxALIGN_CENTER);
    bgGrid->Add(timeBox, 1, wxEXPAND | wxALIGN_TOP | wxALIGN_CENTER);

    // (2, 0)
    wxStaticBoxSizer *periBox = new wxStaticBoxSizer(wxVERTICAL, this,
                                                     wxT("Period"));
    periBox->Add(new wxButton(this, ID_NEW_PERIOD, wxT("New")), 0,
                 wxALIGN_CENTER);
    periBox->Add(new wxButton(this, ID_INCR_PERIOD, wxT("+")), 0,
                 wxALIGN_CENTER);
    periBox->Add(new wxButton(this, ID_DECR_PERIOD, wxT("-")), 0,
                 wxALIGN_CENTER);
    bgGrid->Add(periBox, 1, wxEXPAND | wxALIGN_TOP | wxALIGN_CENTER);

    // (3, 0)
    wxStaticBoxSizer *rScoreBox = new wxStaticBoxSizer(wxVERTICAL, this,
                                                       wxT("Visitor Score"));
    rScoreBox->Add(new wxButton(this, ID_INCR_RIGHT_SCORE, wxT("+")), 0,
                   wxALIGN_CENTER);
    rScoreBox->Add(new wxButton(this, ID_DECR_RIGHT_SCORE, wxT("-")), 0,
                   wxALIGN_CENTER);
    bgGrid->Add(rScoreBox, 1, wxEXPAND | wxALIGN_TOP | wxALIGN_CENTER);

    // (0, 1)
    wxStaticBoxSizer *lFoulBox = new wxStaticBoxSizer(wxVERTICAL, this,
                                                      wxT("Home Fouls"));
    lFoulBox->Add(new wxButton(this, ID_INCR_LEFT_FOUL, wxT("+")), 0,
                  wxALIGN_CENTER);
    lFoulBox->Add(new wxButton(this, ID_DECR_LEFT_FOUL, wxT("-")), 0,
                  wxALIGN_CENTER);
    bgGrid->Add(lFoulBox, 1, wxEXPAND | wxALIGN_TOP | wxALIGN_CENTER);

    // (1, 1)
    wxStaticBoxSizer *gameClockBox = new wxStaticBoxSizer(wxVERTICAL, this,
                                                          wxT("Game Clock"));
    gameClockBox->Add(new wxButton(this, ID_SET_GAME_CLOCK, wxT("Set")),
                      0, wxALIGN_CENTER);
    gameClockBox->Add(new wxButton(this, ID_INCR_GAME_CLOCK, wxT("+")), 0,
                  wxALIGN_CENTER);
    gameClockBox->Add(new wxButton(this, ID_DECR_GAME_CLOCK, wxT("-")), 0,
                  wxALIGN_CENTER);
    bgGrid->Add(gameClockBox, 1, wxEXPAND | wxALIGN_TOP | wxALIGN_CENTER);

    // (2, 1)
    wxStaticBoxSizer *shotClockBox = new wxStaticBoxSizer(wxVERTICAL, this,
                                                          wxT("Shot Clock"));
    shotClockBox->Add(new wxButton(this, ID_RESET_SHOT_CLOCK, wxT("Reset")),
                      0, wxALIGN_CENTER);
    shotClockBox->Add(new wxButton(this, ID_INCR_SHOT_CLOCK, wxT("+")), 0,
                  wxALIGN_CENTER);
    shotClockBox->Add(new wxButton(this, ID_DECR_SHOT_CLOCK, wxT("-")), 0,
                  wxALIGN_CENTER);
    bgGrid->Add(shotClockBox, 1, wxEXPAND | wxALIGN_TOP | wxALIGN_CENTER);

    // (3, 1)
    wxStaticBoxSizer *rFoulBox = new wxStaticBoxSizer(wxVERTICAL, this,
                                                      wxT("Visitor Fouls"));
    rFoulBox->Add(new wxButton(this, ID_INCR_RIGHT_FOUL, wxT("+")), 0,
                  wxALIGN_CENTER);
    rFoulBox->Add(new wxButton(this, ID_DECR_RIGHT_FOUL, wxT("-")), 0,
                  wxALIGN_CENTER);
    bgGrid->Add(rFoulBox, 1, wxEXPAND | wxALIGN_TOP | wxALIGN_CENTER);

    SetSizer(bgGrid);
}

void
GameControlPanel::EnablePlay(bool enable)
{
    m_playButton->Enable(enable);
}

void
GameControlPanel::EnablePause(bool enable)
{
    m_pauseButton->Enable(enable);
}
