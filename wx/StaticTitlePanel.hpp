#ifndef __STATICTITLEPANEL_HPP__
#define __STATICTITLEPANEL_HPP__

#include <wx/wx.h>
 
class StaticTitlePanel : public wxPanel
{
private:
    wxString m_title;

public:
    StaticTitlePanel(wxWindow* parent, wxString title);

    void Paint();
    void Render(wxDC& dc);

    void OnPaint(wxPaintEvent& evt);
    void OnSize(wxSizeEvent& event);
};
 
#endif  // __STATICTITLEPANEL_HPP__
