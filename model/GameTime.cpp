#include "GameTime.hpp"

#if defined(_MSC_VER)
#  include <windows.h>
#else
#  include <sys/time.h>
#  include <unistd.h>
unsigned long
GetTickCount()
{
    timeval tv;
    gettimeofday(&tv, NULL);
    return (unsigned long)(tv.tv_sec * 1000 + tv.tv_usec / 1000);
}
#endif

static const long defaultMaxPeriod = 9;
static const long defaultMinPeriod = 0;
static const int invalidAttackTime = -1;

GameTime::GameTime(IGameTimeView& view, IGameControlView& ctrlView, GameConfig& config)
    : m_config(config)
    , m_view(view)
    , m_ctrlView(ctrlView)
    , m_isPlaying(false)
    , m_isBuzzing(false)
    , m_buzzerReason(Buzzer_Ready)
    , m_attackStartTime(-1)
{
    ResetTime();
    ResetPeriod();
}

void
GameTime::UpdateTime()
{
    m_playTime = m_stopWatch.GetTime();
    if (m_isPlaying) {
        if (m_attackTime == invalidAttackTime) {
            m_attackStartTime = m_playTime;
        }
        m_attackTime = m_playTime - m_attackStartTime;
    }
    else if (m_attackTime > 0) {
        m_attackTime = m_playTime - m_attackStartTime;
    }
}

void
GameTime::SoundBuzzer()
{
    if (m_isPlaying && ! m_isBuzzing) {
        m_buzzerStartTime = GetTickCount();
        m_view.StartBuzzer();
        m_isBuzzing = true;
    }
}

void
GameTime::StopBuzzer()
{
    if (m_isBuzzing) {
        if (GetTickCount() - m_buzzerStartTime > m_config.GetBuzzerLength()) {
            m_view.StopBuzzer();
            m_isBuzzing = false;
        }
    }
}

void
GameTime::UpdateView()
{
    if (m_playTime > m_config.GetPeriodLength()) {
        m_view.UpdateGameClock(m_config.GetPeriodLength());
        if (m_buzzerReason != Buzzer_EndOfPeriod) {
            SoundBuzzer();
            m_buzzerReason = Buzzer_EndOfPeriod;
        }
        PauseInterval();
    }
    else {
        m_view.UpdateGameClock(m_config.GetPeriodLength() - m_playTime);
    }
    if (m_attackTime > m_config.GetShotClock()) {
        m_view.UpdateShotClock(0);
        if (m_buzzerReason != Buzzer_EndOfAttack) {
            SoundBuzzer();
            m_buzzerReason = Buzzer_EndOfAttack;
        }
    }
    else if (m_attackTime < 0) {
        m_view.UpdateShotClock(m_config.GetShotClock());
    }
    else {
        m_view.UpdateShotClock(m_config.GetShotClock() - m_attackTime);
    }
}

void
GameTime::TickTock()
{
    UpdateTime();

    UpdateView();

    StopBuzzer();
}

void
GameTime::PlayInterval()
{
    m_stopWatch.Start();
    m_ctrlView.EnablePlay(false);
    m_ctrlView.EnablePause(true);
    m_isPlaying = true;
}

void
GameTime::Play()
{
    if (!m_isPlaying) {
        PlayInterval();
        UpdateTime();
        UpdateView();
    }
}

void
GameTime::PauseInterval()
{
    m_isPlaying = false;
    m_stopWatch.Pause();
    m_ctrlView.EnablePause(false);
    m_ctrlView.EnablePlay(true);
}

void
GameTime::Pause()
{
    if (m_isPlaying) {
        UpdateTime();
        PauseInterval();
        UpdateView();
    }
}

void
GameTime::ResetTime()
{
    Pause();

    m_stopWatch.Stop();
    m_playTime = 0;

    ResetShotClock();
}

int
GameTime::GetGameClock() const
{
    return m_config.GetPeriodLength() - m_playTime;
}

void
GameTime::SetGameClock(int millis)
{
    if (0 <= millis) {
        long newPlayTime = m_config.GetPeriodLength() - millis;
        if (m_attackStartTime >= 0) {
            m_attackStartTime += (newPlayTime - m_stopWatch.GetTime());
        }
        m_stopWatch.SetTime(newPlayTime);

        UpdateTime();
        UpdateView();
    }
}

void
GameTime::IncrGameClock()
{
    // decrease play time
    if (m_stopWatch.GetTime() > 1000) {
        m_stopWatch.DecrTime(1000);
        m_attackStartTime -= 1000;

        UpdateTime();
        UpdateView();
    }
}

void
GameTime::DecrGameClock()
{
    // increase play time
    if (m_stopWatch.GetTime() < m_config.GetPeriodLength() - 1000) {
        m_stopWatch.IncrTime(1000);
        m_attackStartTime += 1000;

        UpdateTime();
        UpdateView();
    }
}

void
GameTime::ResetShotClock()
{
    m_attackTime = invalidAttackTime;
    m_buzzerReason = Buzzer_Ready;

    UpdateTime();
    UpdateView();
}

void
GameTime::IncrShotClock()
{
    // decrease attack time
    m_attackTime = ((m_attackTime / 1000) - 1) * 1000;
    if (m_attackTime < 0) {
        m_attackTime = 0;
    }
    m_attackStartTime = m_playTime - m_attackTime;

    UpdateTime();
    UpdateView();
}

void
GameTime::DecrShotClock()
{
    // increase attack time
    m_attackTime = ((m_attackTime / 1000) + 1) * 1000;
    if (m_attackTime > m_config.GetShotClock() - 1000) {
        m_attackTime = m_config.GetShotClock() - 1000;
    }
    m_attackStartTime = m_playTime - m_attackTime;

    UpdateTime();
    UpdateView();
}

void
GameTime::IncrPeriod()
{
    m_period = (++m_period > defaultMaxPeriod) ? defaultMaxPeriod : m_period;
    m_view.UpdatePeriod(m_period);
}

void
GameTime::DecrPeriod()
{
    m_period = (--m_period < defaultMinPeriod) ? defaultMinPeriod : m_period;
    m_view.UpdatePeriod(m_period);
}

void
GameTime::ResetPeriod()
{
    m_period = 0;
    m_view.UpdatePeriod(m_period);
}


// StopWatchTimer
StopWatchTimer::StopWatchTimer()
    : m_startTime(0)
    , m_state(Timer_Stopped)
{
}

void
StopWatchTimer::Start()
{
    if (m_state == Timer_Paused) {
        m_startTime += (static_cast<long>(GetTickCount()) - m_pauseTime);
        m_state = Timer_Running;
    }
    else if (m_state == Timer_Stopped) {
        m_startTime += static_cast<long>(GetTickCount());
        m_pauseTime = 0;
    }
    m_state = Timer_Running;
}

void
StopWatchTimer::Pause()
{
    if (m_state == Timer_Running) {
        m_pauseTime = static_cast<long>(GetTickCount());
        m_state = Timer_Paused;
    }
}

void
StopWatchTimer::Stop()
{
    if (m_state != Timer_Stopped) {
        m_startTime = 0;
        m_state = Timer_Stopped;
    }
}

long
StopWatchTimer::GetTime() const
{
    switch (m_state) {
    case Timer_Stopped:
        return m_startTime;

    case Timer_Paused:
        return m_pauseTime - m_startTime;

    case Timer_Running:
        return static_cast<long>(GetTickCount()) - m_startTime;

    default:
        return 0;
    }
}

void
StopWatchTimer::SetTime(long millis)
{
    switch (m_state) {
    case Timer_Stopped:
        m_startTime += millis;
        break;

    case Timer_Paused:
        m_startTime = m_pauseTime - millis;
        break;

    case Timer_Running:
        m_startTime = static_cast<long>(GetTickCount()) - millis;
        break;
    }
}

void
StopWatchTimer::IncrTime(long millis)
{
    m_startTime -= 1000;
}

void
StopWatchTimer::DecrTime(long millis)
{
    m_startTime += 1000;
}
