#ifndef __GAMECONFIGDIALOG_HPP__
#define __GAMECONFIGDIALOG_HPP__

#include "common.hpp"
#include "../model/GameConfig.hpp"

class GameConfigDialog : public wxDialog
{
public:
    GameConfigDialog(wxWindow *parent, const wxString& title,
					 GameConfig& config);

    int ShowModal();

private:
    void OnOK(wxCommandEvent& evt);
    void OnCancel(wxCommandEvent& evt);
    void OnPeriodLengthText(wxCommandEvent& evt);
    void OnShotClockText(wxCommandEvent& evt);

    void UpdateNumberText(wxTextCtrl* textCtrl);

private:
    GameConfig& m_config;

    wxTextCtrl* m_periLenText;
    wxString m_oldPeriLen;

    wxTextCtrl* m_shotClockText;
    wxString m_oldShotClock;
};

#endif  // __GAMECONFIGDIALOG_HPP__
