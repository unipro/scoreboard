#include "LcdNumberWindow.hpp"

#define LCD_NUMBER_SEGMENTS 8

LcdNumberWindow::LcdNumberWindow(wxWindow* parent, wxPoint pos, wxSize size)
    : wxWindow(parent, wxID_ANY, pos, size, wxSUNKEN_BORDER)
{
    m_segLen = 40;
    m_segWidth = 10;
    m_space = 5;

    m_numDigit = 6;

    m_lightColr = wxColour(0, 255, 0);
    m_grayColr = wxColour(0, 64, 0);

    SetBackgroundColour(wxColour(0, 0, 0));

    Connect(wxEVT_PAINT, wxPaintEventHandler(LcdNumberWindow::OnPaint));
    Connect(wxEVT_SIZE, wxSizeEventHandler(LcdNumberWindow::OnSize));
}


LcdNumberWindow::~LcdNumberWindow()
{
}


void
LcdNumberWindow::OnPaint(wxPaintEvent& event)
{
    wxPaintDC dc(this);

    int dw = GetClientSize().GetWidth();
    int dh = GetClientSize().GetHeight();

    int bw = GetBitmapWidth();
    int bh = GetBitmapHeight();

    double xs = (double) dw / bw;
    double ys = (double) dh / bh;

    double as = xs > ys? ys : xs;

    dc.SetUserScale(as, as);
    dc.SetDeviceOrigin(((dw - bw * as) / 2), ((dh - bh * as) / 2));

    DoDrawing(&dc);
}


void
LcdNumberWindow::OnSize(wxSizeEvent& event)
{
    Refresh();
}


void
LcdNumberWindow::DoDrawing(wxDC* dc)
{
    wxString buf;
    buf.Printf(wxT("%s"), m_value.c_str());
    while (buf.Replace(wxT(".."), wxT(". .")));

    char curr;
    char next;
    int buflen = buf.Len();
    int ac = buflen - 1;

    for (int c = 0; c < m_numDigit; c++) {
ReadString:
        ac >= 0? curr = buf.GetChar(ac): curr = wxT(' ');
        ac >= 0 && ac < buflen - 1? next = buf.GetChar(ac + 1): next = wxT(' ');

        if (curr == wxT('.')) {
            ac--;
            goto ReadString;
        }

        wxDigitData *data = new wxDigitData;

        data->value = curr;
        data->comma = false;

        if (next == wxT('.')) {
            data->comma = true;
        }

        DrawDigit(dc, c, data);
        ac--;

        delete data;
    }
}


void
LcdNumberWindow::DrawDigit(wxDC* dc, int digit, wxDigitData* data)
{
    unsigned char dec = Decode(data->value);

    if (data->value == wxT(':')) {
        DrawTwoDots(dc, digit);

        return;
    }

    for (int c = 0; c < LCD_NUMBER_SEGMENTS - 1; c++) {
        DrawSegment(dc, digit, c, (dec >> c) & 1);
    }

    DrawSegment(dc, digit, 7, data->comma);
}


void
LcdNumberWindow::DrawTwoDots(wxDC* dc, int digit)
{
    int sl = m_segLen;
    int sw = m_segWidth;

    int x = DigitX(digit);
    int y = DigitY(digit);

    wxBrush brushOn(m_lightColr, wxSOLID);

    x += (sl / 2) - sw;
    y += (sl / 2) - sw;

    dc->SetBrush(brushOn);
    dc->SetPen(wxPen(GetBackgroundColour(), 1, wxSOLID));

    dc->DrawEllipse(x, y, 2 * sw, 2 * sw);

    y += sl;

    dc->DrawEllipse(x, y, 2 * sw, 2 * sw);
}


void
LcdNumberWindow::DrawSegment(wxDC* dc, int digit, int seg, bool state)
{
    int sl = m_segLen;
    int sw = m_segWidth;

    int x = DigitX(digit);
    int y = DigitY(digit);

    wxBrush brushOn(m_lightColr, wxSOLID);
    wxBrush brushOff(m_grayColr, wxSOLID);

    if (state) {
        dc->SetBrush(brushOn);
    }
    else {
        dc->SetBrush(brushOff);
    }

    dc->SetPen(wxPen(GetBackgroundColour(), 1, wxSOLID));

    wxPoint points[4];

    switch(seg) {
    case 0:
        points[0].x = x;
        points[0].y = y;
        points[1].x = x + sl;
        points[1].y = y;
        points[2].x = x + sl - sw;
        points[2].y = y + sw;
        points[3].x = x + sw;
        points[3].y = y + sw;
        break;
    case 5:
        y += 2 * sl - sw;
        points[0].x = x + sw;
        points[0].y = y;
        points[1].x = x + sl - sw;
        points[1].y = y;
        points[2].x = x + sl;
        points[2].y = y + sw;
        points[3].x = x;
        points[3].y = y + sw;
        break;
    case 4:
        y += sl;
        x += sl - sw;
        points[0].x = x;
        points[0].y = y + sw / 2;
        points[1].x = x + sw;
        points[1].y = y;
        points[2].x = x + sw;
        points[2].y = y + sl;
        points[3].x = x;
        points[3].y = y + sl - sw;
        break;    
    case 2:
        x += sl - sw;
        points[0].x = x;
        points[0].y = y + sw;
        points[1].x = x + sw;
        points[1].y = y;
        points[2].x = x + sw;
        points[2].y = y + sl;
        points[3].x = x;
        points[3].y = y + sl - sw / 2;
        break;
    case 3:
        y += sl;
        points[0].x = x;
        points[0].y = y;
        points[1].x = x;
        points[1].y = y + sl;
        points[2].x = x + sw;
        points[2].y = y + sl - sw;
        points[3].x = x + sw;
        points[3].y = y + sw - sw / 2;
        break;
    case 1:
        points[0].x = x;
        points[0].y = y;
        points[1].x = x;
        points[1].y = y + sl;
        points[2].x = x + sw;
        points[2].y = y + sl - sw / 2;
        points[3].x = x + sw;
        points[3].y = y + sw;
        break;
    case 6:
    default:
        break;
    }

    if (seg < 6) {
        dc->DrawPolygon(4, points);
    }
    
    if (seg == 6) {
        y += sl - sw / 2;
        wxPoint p6[6];

        p6[0].x = x;
        p6[0].y = y + sw / 2;
        p6[1].x = x + sw;
        p6[1].y = y;
        p6[2].x = x + sl - sw;
        p6[2].y = y;
        p6[3].x = x + sl;
        p6[3].y = y + sw / 2;
        p6[4].x = x + sl - sw;
        p6[4].y = y + sw;
        p6[5].x = x + sw;
        p6[5].y = y + sw;

        dc->DrawPolygon(6, p6);
    }

    if (seg == 7) {
        y += 2 * sl;
        x += sl;

        dc->DrawEllipse(x + 1, y - sw, sw, sw);
    }
}


// Protected functions that calculate sizes.
// Needed by OnPaint

int
LcdNumberWindow::GetDigitWidth(void)
{
    return m_segLen + m_segWidth + m_space;
}


int
LcdNumberWindow::GetDigitHeight(void)
{
    return (2 * m_segLen) + (2 * m_space);
}


int
LcdNumberWindow::GetBitmapWidth(void)
{
    return (m_numDigit * GetDigitWidth()) + m_space;
}


int
LcdNumberWindow::GetBitmapHeight(void)
{
    return GetDigitHeight();
}


int
LcdNumberWindow::DigitX(int digit)
{
    return GetBitmapWidth() - ((digit + 1) * GetDigitWidth());
}


int
LcdNumberWindow::DigitY(int digit)
{
    return m_space;
}


// Public functions accessible by the user.

void
LcdNumberWindow::SetNumberDigits(int ndigits)
{
    m_numDigit = ndigits;
    Refresh(false);
}


void
LcdNumberWindow::SetValue(wxString value)
{
    if (m_value.Cmp(value)) {
        m_value = value;
        Refresh(false);
    }
}


wxString
LcdNumberWindow::GetValue(void)
{
    return m_value;
}


int
LcdNumberWindow::GetNumberDigits(void)
{
    return m_numDigit;
}


void
LcdNumberWindow::SetLightColour(wxColour c)
{
    m_lightColr = c;
}


void
LcdNumberWindow::SetGrayColour(wxColour c)
{
    m_grayColr = c;
}


wxColour
LcdNumberWindow::GetLightColour(void)
{
    return m_lightColr;
}


wxColour LcdNumberWindow::GetGrayColour(void)
{
    return m_grayColr;
}


int
LcdNumberWindow::GetDigitsNeeded(wxString value)
{
    wxString tst = value;

    while(tst.Replace(wxT("."), wxT("")));

    return tst.Len();
}


// The decoder function. The heart of the LcdNumberWindow.


//      ***0***
//     *       *
//     1       2
//     *       *
//      ***6***
//     *       *  
//     3       4
//     *       *
//      ***5***

// A 10
// B 11
// C 12
// D 13
// E 14
// F 15

//     8421 8421
//     -654 3210
//---------------------
// 0 : 0011.1111 = 0x3F
// 1 : 0001.0100 = 0x14
// 2 : 0110.1101 = 0x6D
// 3 : 0111.0101 = 0x75
// 4 : 0101.0110 = 0x56
// 5 : 0111.0011 = 0x73
// 6 : 0111.1011 = 0x7B
// 7 : 0001,0101 = 0x15
// 8 : 0111.1111 = 0x7F
// 9 : 0111.0111 = 0x77
//   : 0000.0000 = 0x00
// - : 0100.0000 = 0x40
// E : 0110.1011 = 0x6B
// r : 0100.1000 = 0x48
// o : 0111.1000 = 0x78
// ^ : 0100.0111 = 0x47
// C : 0010.1011 = 0x2B


unsigned char
LcdNumberWindow::Decode(char c)
{
    unsigned char ret = 0;
    
    struct DecodedDisplay
    {
        char ch;
        unsigned char value;
    };

    DecodedDisplay dec[] = {
        { wxT('0'), 0x3F }, 
        { wxT('1'), 0x14 },
        { wxT('2'), 0x6D },
        { wxT('3'), 0x75 },
        { wxT('4'), 0x56 },
        { wxT('5'), 0x73 },
        { wxT('6'), 0x7B },
        { wxT('7'), 0x15 },
        { wxT('8'), 0x7F },
        { wxT('9'), 0x77 },
        { wxT(' '), 0x00 },
        { wxT('-'), 0x40 },
        { wxT('E'), 0x6B },
        { wxT('r'), 0x48 },
        { wxT('o'), 0x78 },
        { wxT('^'), 0x47 },
        { wxT('C'), 0x2B },
        { 0, 0 }
    };

    for (int d = 0; dec[d].ch != 0; d++) {
        if (dec[d].ch == c) {
            ret = dec[d].value;
            break;
        }
    }

    return ret;
}
