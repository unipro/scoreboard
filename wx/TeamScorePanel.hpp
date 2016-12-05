#ifndef __TEAMSCOREPANEL_HPP__
#define __TEAMSCOREPANEL_HPP__

#include "common.hpp"
#include "../model/Interfaces.hpp"
#include "LcdNumberWindow.hpp"

class TeamScorePanel : public wxPanel, public ITeamScoreView
{
public:
    TeamScorePanel(wxWindow* parent, const wxString& name);

    // ITeamScoreView methods
    void UpdateScore(int n);
    void UpdateFoul(int n);

protected:
    wxSizer *m_sizer;
    LcdNumberWindow *m_scoreCtrl;
    LcdNumberWindow *m_foulCtrl;
};

class LeftTeamScorePanel : public TeamScorePanel
{
public:
    LeftTeamScorePanel(wxWindow* parent, const wxString& name);
};

class RightTeamScorePanel : public TeamScorePanel
{
public:
    RightTeamScorePanel(wxWindow* parent, const wxString& name);
};

#endif  // __TEAMSCOREPANEL_HPP__
