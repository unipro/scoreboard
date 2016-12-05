#ifndef __SCOREBOARD_INTERFACE_HPP__
#define __SCOREBOARD_INTERFACE_HPP__

class IGameTimeView
{
public:
    virtual void UpdateGameClock(int rest) = 0;
    virtual void UpdateShotClock(int rest) = 0;
    virtual void UpdatePeriod(int n) = 0;
    virtual void StartBuzzer() = 0;
    virtual void StopBuzzer() = 0;
};

class IGameControlView
{
public:
    virtual void EnablePlay(bool enable) = 0;
    virtual void EnablePause(bool enable) = 0;
};

class ITeamScoreView
{
public:
    virtual void UpdateScore(int n) = 0;
    virtual void UpdateFoul(int n) = 0;
};

#endif  // __SCOREBOARD_INTERFACE_HPP__
