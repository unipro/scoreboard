#include "../model/GameConfig.hpp"
#include "GameTimePanel.hpp"
#include "LcdNumberWindow.hpp"
#include "StaticTitlePanel.hpp"
#include <SDL.h>

//     1     :    1     :      1
// +---------------------------------+
// | +-----------------------------+ |
// | |    +--+ +--+   +--+ +--+    | |
// | |    |  | |  | o |  | |  |    | |
// | |    +--+ +--+   +--+ +--+    | | 3
// | |    |  | |  | o |  | |  |    | |
// | |    +--+ +--+   +--+ +--+    | |
// | +-----------------------------+ | :
// |                                 |
// |  shot clock             period  | 1
// | +-----------+          +------+ |
// | | +--+ +--+ |     \    | +--+ | | :
// | | |  | |  | |   ---\   | |  | | |
// | | +--+ +--+ |   ---/   | +--+ | |
// | | |  | |  | |     /    | |  | | | 2
// | | +--+ +--+ |          | +--+ | |
// | +-----------+          +------+ |
// +---------------------------------+
// 
GameTimePanel::GameTimePanel(wxWindow* parent)
    : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize,
              wxBORDER_RAISED)
    , m_buzzing(false)
{
    wxBoxSizer *bgBox = new wxBoxSizer(wxVERTICAL);

    // play time
    m_timeCtrl = new LcdNumberWindow(this, wxDefaultPosition, wxDefaultSize);
    m_timeCtrl->SetNumberDigits(5);
    m_timeCtrl->SetValue(wxT("12:25"));
    bgBox->Add(m_timeCtrl, 3, wxEXPAND | wxTOP | wxLEFT | wxRIGHT, 10);

    bgBox->Add(-1, 10);

    // shot clock, attack direction, period
    wxFlexGridSizer *etcGrid = new wxFlexGridSizer(2, 3, 10, 10);
    etcGrid->Add(new StaticTitlePanel(this, wxT("Shot Clock")), 1, wxEXPAND);
    etcGrid->Add(new wxStaticText(this, -1, wxT("")), 1, wxEXPAND);    // TODO: attack direction
    etcGrid->Add(new StaticTitlePanel(this, wxT("Period")), 1, wxEXPAND);
    m_shotClockCtrl = new LcdNumberWindow(this, wxDefaultPosition, wxDefaultSize);
    m_shotClockCtrl->SetNumberDigits(2);
    wxStaticText *attackDir = new wxStaticText(this, wxID_ANY, wxT(""));
    m_periodCtrl = new LcdNumberWindow(this, wxDefaultPosition, wxDefaultSize);
    m_periodCtrl->SetNumberDigits(1);
    etcGrid->Add(m_shotClockCtrl, 1, wxEXPAND);
    etcGrid->Add(attackDir, 1, wxEXPAND);
    etcGrid->Add(m_periodCtrl, 1, wxEXPAND);
    etcGrid->AddGrowableCol(0, 1);
    etcGrid->AddGrowableCol(1, 1);
    etcGrid->AddGrowableCol(2, 1);
    etcGrid->AddGrowableRow(0, 1);
    etcGrid->AddGrowableRow(1, 2);
    bgBox->Add(etcGrid, 3, wxEXPAND | wxBOTTOM | wxLEFT | wxRIGHT, 10);

    SetSizer(bgBox);
}

void
GameTimePanel::UpdateGameClock(int rest)
{
    wxString timeStr;
    if (rest < 60000) {
        timeStr = wxTimeSpan::Milliseconds(rest).Format(wxT("%S:%l"));
        timeStr = timeStr.Left(5);
    }
    else {
        timeStr = wxTimeSpan::Milliseconds(rest).Format(wxT("%M:%S"));
    }
    if (m_timeCtrl->GetValue().Cmp(timeStr) != 0) {
        m_timeCtrl->SetValue(timeStr);
    }
}

void
GameTimePanel::UpdateShotClock(int rest)
{
    wxString timeStr = wxTimeSpan::Milliseconds(rest + 999).Format(wxT("%S"));
    if (m_shotClockCtrl->GetValue().Cmp(timeStr) != 0) {
        m_shotClockCtrl->SetValue(timeStr);
    }
}

void GameTimePanel::UpdatePeriod(int n)
{
    wxString str;
    str << n;
    m_periodCtrl->SetValue(str);
}

/* Buffer fill-upper */
static
void
CreateTone(void *userdata, Uint8 *stream, int len)
{
    static double angle = 0.0 ;
    int i = 0 ;
    for(i=0;i<len;i++) {
        *stream++ = 255*cos(angle) ; // I think that this should be
                                     // 128 * cos(angle) as cos is (-1,1)
        angle += 3.14159/100 ;       // and a Uint8 is only 8 bits (256 values)
        if( angle > 2.0*3.14159 ) {
            angle -= 2.0*3.14159 ;
        }
    }
}

/* Open the audio device to what we want */
void
GameTimePanel::StartBuzzer()
{
    if (! m_buzzing) {
        SDL_AudioSpec as;

        /* Fill out what we want */
        as.freq = 44100;
        as.format = AUDIO_S16SYS;
        as.channels = 2;
        as.samples = 1024;
        as.callback = CreateTone;

        /* Get it */
        SDL_OpenAudio(&as, NULL);
 
        /* Go! */
        SDL_PauseAudio(0);

        m_buzzing = true;
    }
}

/* Clean up things and close devices */
void
GameTimePanel::StopBuzzer()
{
    if (m_buzzing) {
        /* Close */
        SDL_CloseAudio();

        m_buzzing = false;
    }
}
