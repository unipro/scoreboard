#include "common.hpp"
#include "GameConfigDatabase.hpp"
#include <wx/config.h>

static const wxChar *keyPeriodLength = wxT("PeriodLength");
static const wxChar *keyShotClock = wxT("ShotClock");

bool
GameConfigDatabase::Read(GameConfig& config)
{
    int value;
    wxConfig *wxconfig = new wxConfig(wxT(PACKAGE_NAME));
    wxconfig->Read(keyPeriodLength, &value, -1);
    config.SetPeriodLengthInMin(value);
    wxconfig->Read(keyShotClock, &value, -1);
    config.SetShotClockInSec(value);
    delete wxconfig;
    return true;
}

bool
GameConfigDatabase::Write(GameConfig& config)
{
    wxConfig *wxconfig = new wxConfig(wxT(PACKAGE_NAME));
    wxconfig->Write(keyPeriodLength, config.GetPeriodLengthInMin());
    wxconfig->Write(keyShotClock, config.GetShotClockInSec());
    delete wxconfig;
    return true;
}
