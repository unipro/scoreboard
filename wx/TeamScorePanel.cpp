#include "TeamScorePanel.hpp"
#include "LcdNumberWindow.hpp"
#include "StaticTitlePanel.hpp"

// +--------------------+
// |                    |
// |       Home         |
// |                    |
// | +----------------+ |
// | | +--+ +--+ +--+ | |
// | | |  | |  | |  | | |
// | | +--+ +--+ +--+ | |
// | | |  | |  | |  | | |
// | | +--+ +--+ +--+ | |
// | +----------------+ |
// |                    |
// |  fouls             |
// | +-----------+      |
// | | +--+ +--+ |      |
// | | |  | |  | |      |
// | | +--+ +--+ |      |
// | | |  | |  | |      |
// | | +--+ +--+ |      |
// | +-----------+      |
// +--------------------+
// 

TeamScorePanel::TeamScorePanel(wxWindow* parent, const wxString& name)
    : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize,
              wxBORDER_RAISED, name)
{
    m_sizer = new wxBoxSizer(wxVERTICAL);

    // 팀이름 추가
    m_sizer->Add(new StaticTitlePanel(this, name),
               1, wxEXPAND | wxTOP | wxLEFT | wxRIGHT, 10);

    m_sizer->Add(-1, 10);

    // 점수 추가
    m_scoreCtrl = new LcdNumberWindow(this, wxDefaultPosition, wxDefaultSize);
    m_scoreCtrl->SetNumberDigits(3);
    m_sizer->Add(m_scoreCtrl, 3, wxEXPAND | wxLEFT | wxRIGHT, 10);

    SetSizer(m_sizer);
}

void
TeamScorePanel::UpdateScore(int n)
{
    wxString str;
    str << n;
    m_scoreCtrl->SetValue(str);
}

void
TeamScorePanel::UpdateFoul(int n)
{
    wxString str;
    str << n;
    m_foulCtrl->SetValue(str);
}

LeftTeamScorePanel::LeftTeamScorePanel(wxWindow* parent, const wxString& name)
    : TeamScorePanel(parent, name)
{

    m_sizer->Add(-1, 10);

    // 파울수(와 기타) 추가
    wxFlexGridSizer *etcGrid = new wxFlexGridSizer(2, 2, 10, 10);
    etcGrid->Add(new StaticTitlePanel(this, wxT("Fouls")), 1, wxEXPAND);
    etcGrid->Add(new wxStaticText(this, wxID_ANY, wxT("")), 1, wxEXPAND);
    m_foulCtrl = new LcdNumberWindow(this, wxDefaultPosition, wxDefaultSize);
    m_foulCtrl->SetNumberDigits(2);
    etcGrid->Add(m_foulCtrl, 1, wxEXPAND);
    etcGrid->Add(new wxStaticText(this, wxID_ANY, wxT("")), 1, wxEXPAND);
    etcGrid->AddGrowableCol(0, 1);
    etcGrid->AddGrowableCol(1, 1);
    etcGrid->AddGrowableRow(0, 1);
    etcGrid->AddGrowableRow(1, 2);
    m_sizer->Add(etcGrid, 3, wxEXPAND | wxBOTTOM | wxLEFT | wxRIGHT, 10);
}

RightTeamScorePanel::RightTeamScorePanel(wxWindow* parent, const wxString& name)
    : TeamScorePanel(parent, name)
{
    m_sizer->Add(-1, 10);

    // 파울수(와 기타) 추가
    wxFlexGridSizer *etcGrid = new wxFlexGridSizer(2, 2, 10, 10);
    etcGrid->Add(new wxStaticText(this, wxID_ANY, wxT("")), 1, wxEXPAND);
    etcGrid->Add(new StaticTitlePanel(this, wxT("Fouls")), 1, wxEXPAND);
    m_foulCtrl = new LcdNumberWindow(this, wxDefaultPosition, wxDefaultSize);
    m_foulCtrl->SetNumberDigits(2);
    etcGrid->Add(new wxStaticText(this, wxID_ANY, wxT("")), 1, wxEXPAND);
    etcGrid->Add(m_foulCtrl, 1, wxEXPAND);
    etcGrid->AddGrowableCol(0, 1);
    etcGrid->AddGrowableCol(1, 1);
    etcGrid->AddGrowableRow(0, 1);
    etcGrid->AddGrowableRow(1, 2);
    m_sizer->Add(etcGrid, 3, wxEXPAND | wxBOTTOM | wxLEFT | wxRIGHT, 10);
}
