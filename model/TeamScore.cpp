#include "TeamScore.hpp"

static const long defaultMaxScore = 999;
static const long defaultMinScore = 0;
static const long defaultMaxFoul = 99;
static const long defaultMinFoul = 0;

TeamScore::TeamScore(ITeamScoreView& view)
    : m_view(view)
{
    Reset();
}

void
TeamScore::Reset()
{
    ResetScore();
    ResetFoul();
}

void
TeamScore::ResetScore()
{
    m_score = 0;
    m_view.UpdateScore(m_score);
}

void
TeamScore::IncrScore()
{
    m_score = (++m_score > defaultMaxScore) ? defaultMaxScore : m_score;
    m_view.UpdateScore(m_score);
}

void
TeamScore::DecrScore()
{
    m_score = (--m_score < defaultMinScore) ? defaultMinScore : m_score;
    m_view.UpdateScore(m_score);
}

void
TeamScore::ResetFoul()
{
    m_foul = 0;
    m_view.UpdateFoul(m_foul);
}

void
TeamScore::IncrFoul()
{
    m_foul = (++m_foul > defaultMaxFoul) ? defaultMaxFoul : m_foul;
    m_view.UpdateFoul(m_foul);
}

void
TeamScore::DecrFoul()
{
    m_foul = (--m_foul < defaultMinFoul) ? defaultMinFoul : m_foul;
    m_view.UpdateFoul(m_foul);
}
