#include "ScoreboardFrame.hpp"
#include "TeamScorePanel.hpp"
#include "GameTimePanel.hpp"
#include "GameControlPanel.hpp"
#include "GameConfigDialog.hpp"
#include <wx/aboutdlg.h>

#if defined(__WXGTK__) || defined(__WXMOTIF__)
#include "scoreboard.xpm"
#endif

//      1    :         2         :    1
// +---------+-------------------+---------+
// |         |                   |         |
// |         |                   |         |
// |  team   |                   |  team   |
// |  panel  |     game panel    |  panel  |
// |         |                   |         | 1
// |         |                   |         |
// |         |                   |         |
// |         |                   |         |
// |         |                   |         |
// |         +-------------------+         | :
// |         |                   |         |
// |         |     ctrl panel    |         | 0
// |         |                   |         |
// +---------+-------------------+---------+

GameConfig g_config;

wxSize ScoreboardFrame::MIN_SIZE(1280, 720);
wxColour ScoreboardFrame::BG_COLOR(0x43, 0x43, 0x43);

class GameClockDialog : public wxDialog
{
public:
    GameClockDialog(wxWindow *parent, const wxString& title, GameConfig& config);

    int ShowModal();

    void SetGameClock(int gameclock) {
        m_gameClock = gameclock;
    }
    int GetGameClock() const {
        return m_gameClock;
    }

private:
    void OnOK(wxCommandEvent& evt);
    void OnCancel(wxCommandEvent& evt);
    void OnGameClockText(wxCommandEvent& evt);

    void UpdateNumberText(wxTextCtrl* textCtrl);

private:
    GameConfig& m_config;

    wxTextCtrl* m_gameClockText;
    wxString m_oldGameClock;
    int m_gameClock;
};

ScoreboardFrame::ScoreboardFrame(const wxString& title)
    : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, ScoreboardFrame::MIN_SIZE)
    , m_timer(this, ID_CLOCK_TIMER)
{
    // icon
    static wxIcon icon(wxICON(scoreboard));
    SetIcon(icon);

    // file menu
    wxMenu *game = new wxMenu;
    game->Append(new wxMenuItem(game, ID_NEW_GAME, wxT("New &Game\tCtrl+G")));
    game->Append(new wxMenuItem(game, ID_NEW_PERIOD,
                                wxT("New &Period\tCtrl+P")));
    game->AppendSeparator();
    game->Append(new wxMenuItem(game, wxID_SETUP, wxT("&Options...")));
    game->AppendSeparator();
    game->Append(new wxMenuItem(game, wxID_EXIT, wxT("&Quit\tCtrl+W")));

    // help menu
    wxMenu *help = new wxMenu;
    help->Append(wxID_ABOUT);

    // append menus
    wxMenuBar *menubar = new wxMenuBar;
    menubar->Append(game, wxT("&Game"));
    menubar->Append(help, wxT("&Help"));
    SetMenuBar(menubar);

    // background panel and box.
    wxPanel *bgPan = new wxPanel(this, wxID_ANY);
    wxBoxSizer *bgBox = new wxBoxSizer(wxHORIZONTAL);

    // team panels and models
    TeamScorePanel *leftTeamPan = new LeftTeamScorePanel(bgPan, wxT("Home"));
    TeamScorePanel *rightTeamPan = new RightTeamScorePanel(bgPan, wxT("Visitor"));
    m_leftTeam = new TeamScore(*leftTeamPan);
    m_rightTeam = new TeamScore(*rightTeamPan);

    // game time panel, control panel and model
    wxBoxSizer *midBox = new wxBoxSizer(wxVERTICAL);
    GameTimePanel *gameTimePan = new GameTimePanel(bgPan);
    GameControlPanel *gameCtrlPan = new GameControlPanel(bgPan);
    m_gameTime = new GameTime(*gameTimePan, *gameCtrlPan, g_config);

    // add game time panel and control panel
    midBox->Add(gameTimePan, 2, wxEXPAND | wxTOP, 10);
    midBox->Add(-1, 10);
    midBox->Add(gameCtrlPan, 1, wxEXPAND | wxBOTTOM, 10);

    // add team panels
    bgBox->Add(leftTeamPan, 1, wxEXPAND | wxTOP | wxBOTTOM | wxLEFT, 10);
    bgBox->Add(10, -1);
    bgBox->Add(midBox, 2, wxEXPAND);
    bgBox->Add(10, -1);
    bgBox->Add(rightTeamPan, 1, wxEXPAND | wxTOP | wxBOTTOM | wxRIGHT, 10);

    bgPan->SetSizer(bgBox);

    // set background color
    SetBackgroundColour(BG_COLOR);

    // Accelerator table
    wxAcceleratorEntry entries[6];
    entries[0].Set(wxACCEL_NORMAL, WXK_SPACE, ID_TOGGLE_PLAY);
    entries[1].Set(wxACCEL_NORMAL, WXK_RETURN, ID_RESET_SHOT_CLOCK);
    entries[2].Set(wxACCEL_CTRL, WXK_LEFT, ID_INCR_LEFT_SCORE);
    entries[3].Set(wxACCEL_CTRL, WXK_RIGHT, ID_INCR_RIGHT_SCORE);
    entries[4].Set(wxACCEL_ALT, WXK_LEFT, ID_INCR_LEFT_FOUL);
    entries[5].Set(wxACCEL_ALT, WXK_RIGHT, ID_INCR_RIGHT_FOUL);
    wxAcceleratorTable accel(N_ELEMENTS(entries), entries);
    SetAcceleratorTable(accel);

    // event
    Connect(ID_CLOCK_TIMER, wxEVT_TIMER,
            wxTimerEventHandler(ScoreboardFrame::OnTimer));

    Connect(ID_NEW_GAME, wxEVT_COMMAND_MENU_SELECTED, 
            wxCommandEventHandler(ScoreboardFrame::OnNewGame));
    Connect(ID_NEW_PERIOD, wxEVT_COMMAND_MENU_SELECTED, 
            wxCommandEventHandler(ScoreboardFrame::OnNewPeriod));
    Connect(wxID_SETUP, wxEVT_COMMAND_MENU_SELECTED, 
            wxCommandEventHandler(ScoreboardFrame::OnSetup));
    Connect(wxID_EXIT, wxEVT_COMMAND_MENU_SELECTED,
            wxCommandEventHandler(ScoreboardFrame::OnQuit));
    Connect(wxID_ABOUT, wxEVT_COMMAND_MENU_SELECTED,
            wxCommandEventHandler(ScoreboardFrame::OnAbout));

    Connect(ID_NEW_GAME, wxEVT_COMMAND_BUTTON_CLICKED, 
            wxCommandEventHandler(ScoreboardFrame::OnNewGame));
    Connect(ID_TOGGLE_PLAY, wxEVT_COMMAND_MENU_SELECTED, 
            wxCommandEventHandler(ScoreboardFrame::OnTogglePlay));
    Connect(ID_PLAY_GAME, wxEVT_COMMAND_BUTTON_CLICKED, 
            wxCommandEventHandler(ScoreboardFrame::OnPlayGame));
    Connect(ID_PAUSE_GAME, wxEVT_COMMAND_BUTTON_CLICKED, 
            wxCommandEventHandler(ScoreboardFrame::OnPauseGame));

    Connect(ID_SET_GAME_CLOCK, wxEVT_COMMAND_BUTTON_CLICKED, 
            wxCommandEventHandler(ScoreboardFrame::OnSetGameClock));
    Connect(ID_INCR_GAME_CLOCK, wxEVT_COMMAND_BUTTON_CLICKED, 
            wxCommandEventHandler(ScoreboardFrame::OnIncrGameClock));
    Connect(ID_DECR_GAME_CLOCK, wxEVT_COMMAND_BUTTON_CLICKED, 
            wxCommandEventHandler(ScoreboardFrame::OnDecrGameClock));

    Connect(ID_RESET_SHOT_CLOCK, wxEVT_COMMAND_BUTTON_CLICKED, 
            wxCommandEventHandler(ScoreboardFrame::OnResetShotClock));
    Connect(ID_INCR_SHOT_CLOCK, wxEVT_COMMAND_BUTTON_CLICKED, 
            wxCommandEventHandler(ScoreboardFrame::OnIncrShotClock));
    Connect(ID_DECR_SHOT_CLOCK, wxEVT_COMMAND_BUTTON_CLICKED, 
            wxCommandEventHandler(ScoreboardFrame::OnDecrShotClock));

    Connect(ID_INCR_LEFT_SCORE, wxEVT_COMMAND_BUTTON_CLICKED, 
            wxCommandEventHandler(ScoreboardFrame::OnIncrLeftScore));
    Connect(ID_DECR_LEFT_SCORE, wxEVT_COMMAND_BUTTON_CLICKED, 
            wxCommandEventHandler(ScoreboardFrame::OnDecrLeftScore));

    Connect(ID_INCR_RIGHT_SCORE, wxEVT_COMMAND_BUTTON_CLICKED, 
            wxCommandEventHandler(ScoreboardFrame::OnIncrRightScore));
    Connect(ID_DECR_RIGHT_SCORE, wxEVT_COMMAND_BUTTON_CLICKED, 
            wxCommandEventHandler(ScoreboardFrame::OnDecrRightScore));

    Connect(ID_INCR_LEFT_FOUL, wxEVT_COMMAND_BUTTON_CLICKED, 
            wxCommandEventHandler(ScoreboardFrame::OnIncrLeftFoul));
    Connect(ID_DECR_LEFT_FOUL, wxEVT_COMMAND_BUTTON_CLICKED, 
            wxCommandEventHandler(ScoreboardFrame::OnDecrLeftFoul));

    Connect(ID_INCR_RIGHT_FOUL, wxEVT_COMMAND_BUTTON_CLICKED, 
            wxCommandEventHandler(ScoreboardFrame::OnIncrRightFoul));
    Connect(ID_DECR_RIGHT_FOUL, wxEVT_COMMAND_BUTTON_CLICKED, 
            wxCommandEventHandler(ScoreboardFrame::OnDecrRightFoul));

    Connect(ID_NEW_PERIOD, wxEVT_COMMAND_BUTTON_CLICKED, 
            wxCommandEventHandler(ScoreboardFrame::OnNewPeriod));
    Connect(ID_INCR_PERIOD, wxEVT_COMMAND_BUTTON_CLICKED, 
            wxCommandEventHandler(ScoreboardFrame::OnIncrPeriod));
    Connect(ID_DECR_PERIOD, wxEVT_COMMAND_BUTTON_CLICKED, 
            wxCommandEventHandler(ScoreboardFrame::OnDecrPeriod));

    // start timer
    m_timer.Start(10);
}

ScoreboardFrame::~ScoreboardFrame()
{
    delete m_gameTime;
    delete m_leftTeam;
    delete m_rightTeam;
}

#if defined(__WXGTK__)
// avoid wxGTK bug
// (http://trac.wxwidgets.org/ticket/10751)
static
void
SetBackgroundColour(wxWindow *wnd, const wxColour& colour)
{
    wxWindowList & children = wnd->GetChildren();
    for (wxWindowList::Node *node = children.GetFirst(); node;
         node = node->GetNext()) {
        wxWindow *curr = (wxWindow *)node->GetData();
        if (curr->IsKindOf(CLASSINFO(wxPanel))) {
            curr->SetBackgroundColour(colour);
            ::SetBackgroundColour(curr, colour);
        }
    }
}
#endif

bool
ScoreboardFrame::SetBackgroundColour(const wxColour& colour)
{
#if defined(__WXGTK__)
    ::SetBackgroundColour(this, colour);
#endif
    return wxFrame::SetBackgroundColour(colour);
}

void
ScoreboardFrame::OnQuit(wxCommandEvent& WXUNUSED(evt))
{
    Close(true);
}

void
ScoreboardFrame::OnAbout(wxCommandEvent& WXUNUSED(evt))
{
    wxAboutDialogInfo info;
    info.SetName(wxT(PACKAGE_NAME));
    info.SetVersion(wxT(PACKAGE_VERSION));
    info.SetDescription(wxT("Simple scoreboard application"));
    info.SetCopyright(wxT("Copyright (C) 2012 Daniel Jun"));
#if defined(PACKAGE_URL)
    info.SetWebSite(wxT(PACKAGE_URL));
#else
    info.SetWebSite(wxT("http://dev.naver.com/projects/scoreboard"));
#endif
    info.AddDeveloper(wxT("Daniel Jun"));

    ::wxAboutBox(info);
}

void
ScoreboardFrame::OnSetup(wxCommandEvent& WXUNUSED(evt))
{
    PauseGame();
    GameConfigDialog configDlg(this, wxT("Option"), g_config);
    configDlg.ShowModal();
}

void
ScoreboardFrame::OnTimer(wxTimerEvent& event)
{
    m_gameTime->TickTock();
}

void
ScoreboardFrame::OnNewGame(wxCommandEvent& WXUNUSED(evt))
{
    m_gameTime->ResetTime();
    m_gameTime->ResetPeriod();

    m_leftTeam->ResetScore();
    m_leftTeam->ResetFoul();

    m_rightTeam->ResetScore();
    m_rightTeam->ResetFoul();
}

void
ScoreboardFrame::PlayGame()
{
    m_gameTime->Play();
}

void
ScoreboardFrame::PauseGame()
{
    m_gameTime->Pause();
}


void
ScoreboardFrame::OnTogglePlay(wxCommandEvent& WXUNUSED(evt))
{
    if (m_gameTime->IsPlaying()) {
        PauseGame();
    }
    else {
        PlayGame();
    }
}

void
ScoreboardFrame::OnPlayGame(wxCommandEvent& WXUNUSED(evt))
{
    PlayGame();
}

void
ScoreboardFrame::OnPauseGame(wxCommandEvent& WXUNUSED(evt))
{
    PauseGame();
}

void
ScoreboardFrame::OnSetGameClock(wxCommandEvent& WXUNUSED(evt))
{
    GameClockDialog gameClockDlg(this, wxT("Set Game Clock"), g_config);
    gameClockDlg.SetGameClock(m_gameTime->GetGameClock() / 60000);
    if (gameClockDlg.ShowModal() == wxID_OK) {
        m_gameTime->SetGameClock(gameClockDlg.GetGameClock() * 60000);
    }
}

void
ScoreboardFrame::OnIncrGameClock(wxCommandEvent& WXUNUSED(evt))
{
    m_gameTime->IncrGameClock();
}

void
ScoreboardFrame::OnDecrGameClock(wxCommandEvent& WXUNUSED(evt))
{
    m_gameTime->DecrGameClock();
}

void
ScoreboardFrame::OnResetShotClock(wxCommandEvent& WXUNUSED(evt))
{
    m_gameTime->ResetShotClock();
}

void
ScoreboardFrame::OnIncrShotClock(wxCommandEvent& WXUNUSED(evt))
{
    m_gameTime->IncrShotClock();
}

void
ScoreboardFrame::OnDecrShotClock(wxCommandEvent& WXUNUSED(evt))
{
    m_gameTime->DecrShotClock();
}

void
ScoreboardFrame::OnIncrLeftScore(wxCommandEvent& WXUNUSED(evt))
{
    m_leftTeam->IncrScore();
}

void
ScoreboardFrame::OnDecrLeftScore(wxCommandEvent& WXUNUSED(evt))
{
    m_leftTeam->DecrScore();
}

void
ScoreboardFrame::OnIncrRightScore(wxCommandEvent& WXUNUSED(evt))
{
    m_rightTeam->IncrScore();
}

void
ScoreboardFrame::OnDecrRightScore(wxCommandEvent& WXUNUSED(evt))
{
    m_rightTeam->DecrScore();
}

void
ScoreboardFrame::OnIncrLeftFoul(wxCommandEvent& WXUNUSED(evt))
{
    m_leftTeam->IncrFoul();
}

void
ScoreboardFrame::OnDecrLeftFoul(wxCommandEvent& WXUNUSED(evt))
{
    m_leftTeam->DecrFoul();
}

void
ScoreboardFrame::OnIncrRightFoul(wxCommandEvent& WXUNUSED(evt))
{
    m_rightTeam->IncrFoul();
}

void
ScoreboardFrame::OnDecrRightFoul(wxCommandEvent& WXUNUSED(evt))
{
    m_rightTeam->DecrFoul();
}

void
ScoreboardFrame::OnNewPeriod(wxCommandEvent& WXUNUSED(evt))
{
    m_gameTime->ResetTime();
    m_gameTime->IncrPeriod();

    m_leftTeam->ResetFoul();
    m_rightTeam->ResetFoul();
}

void
ScoreboardFrame::OnIncrPeriod(wxCommandEvent& WXUNUSED(evt))
{
    m_gameTime->IncrPeriod();
}

void
ScoreboardFrame::OnDecrPeriod(wxCommandEvent& WXUNUSED(evt))
{
    m_gameTime->DecrPeriod();
}



// GameClockDialog

GameClockDialog::GameClockDialog(wxWindow *parent, const wxString& title, GameConfig& config)
    : wxDialog(parent, wxID_ANY, title, wxDefaultPosition, wxSize(240, 150),
               wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
    , m_config(config)
    , m_gameClock(-1)
{
    wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);

    wxPanel *panel = new wxPanel(this, wxID_ANY);
    wxFlexGridSizer *grid = new wxFlexGridSizer(1, 2, 4, 4);
    grid->Add(new wxStaticText(panel, wxID_ANY, wxT("Game Clock (min)")),
              0, wxALIGN_RIGHT);
    m_gameClockText = new wxTextCtrl(panel, ID_PERIOD_LENGTH_TEXT, wxT(""),
                                   wxDefaultPosition, wxDefaultSize);
    m_gameClockText->SetMaxLength(2); // 0 ~ 99
    grid->Add(m_gameClockText, 1, wxEXPAND);
    grid->AddGrowableCol(1, 1);
    panel->SetSizer(grid);

    vbox->Add(panel, 1, wxEXPAND | wxTOP | wxLEFT | wxRIGHT, 10);

    wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);
    hbox->Add(new wxButton(this, wxID_OK, wxT("OK"), 
                           wxDefaultPosition, wxSize(70, 30)), 1);
    hbox->Add(new wxButton(this, wxID_CANCEL, wxT("Close"), 
                           wxDefaultPosition, wxSize(70, 30)), 1, wxLEFT, 5);

    vbox->Add(hbox, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 10);

    SetSizer(vbox);

    Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, 
            wxCommandEventHandler(GameClockDialog::OnOK));
    Connect(wxID_CANCEL, wxEVT_COMMAND_BUTTON_CLICKED, 
            wxCommandEventHandler(GameClockDialog::OnCancel));

    Connect(ID_GAME_CLOCK_TEXT, wxEVT_COMMAND_TEXT_UPDATED, 
            wxCommandEventHandler(GameClockDialog::OnGameClockText));
}

int
GameClockDialog::ShowModal()
{
    if (m_gameClock < 0)
        m_gameClock = m_config.GetPeriodLengthInMin();
    m_oldGameClock.Empty();
    m_oldGameClock << m_gameClock;
    m_gameClockText->SetValue(m_oldGameClock);

    return wxDialog::ShowModal();
}

void
GameClockDialog::OnOK(wxCommandEvent& evt)
{
    m_gameClock = wxAtoi(m_gameClockText->GetValue());
    if (m_gameClock < 0 || m_config.GetPeriodLengthInMin() < m_gameClock) {
        wxMessageDialog dlg(NULL, 
                            wxT("Invalid Game Clock"), 
                            wxT("Error"),
                            wxOK | wxICON_ERROR);
        dlg.ShowModal();

        m_gameClockText->SetValue(m_oldGameClock);
        m_gameClockText->SetFocus();
        return;
    }

    EndModal(wxID_OK);
}

void
GameClockDialog::OnCancel(wxCommandEvent& evt)
{
    EndModal(wxID_CANCEL);
}

void
GameClockDialog::UpdateNumberText(wxTextCtrl* textCtrl)
{
    if (!textCtrl->IsModified())
        return;

    wxString val = textCtrl->GetValue();
    if (! val.IsNumber()) {
        long insPnt = textCtrl->GetInsertionPoint();
        textCtrl->Remove(insPnt - 1, insPnt);
    }
}

void
GameClockDialog::OnGameClockText(wxCommandEvent& evt)
{
    UpdateNumberText(m_gameClockText);
}
