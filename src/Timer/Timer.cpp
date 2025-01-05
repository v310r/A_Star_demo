#include "pch.h"

#include "Timer.h"


Timer::Timer()
{
	StartTimer();
}

void Timer::StartTimer()
{
	m_bIsRunning = true;
	m_StartTimePoint = std::chrono::steady_clock::now();
}

void Timer::StopTimer()
{
	m_EndTimePoint = std::chrono::steady_clock::now();
	m_bIsRunning = false;
}

void Timer::ResetTimer()
{
	m_StartTimePoint = std::chrono::steady_clock::now();
	m_bIsRunning = true;
}

float Timer::GetElapsedTime() const
{
	if (m_bIsRunning)
	{
		return std::chrono::duration<float>(std::chrono::steady_clock::now() - m_StartTimePoint).count();
	}
	else
	{
		return std::chrono::duration<float>(m_EndTimePoint - m_StartTimePoint).count();
	}
}

float Timer::ConvertSecondsToMS(float seconds)
{
	return seconds * 1000;
}

