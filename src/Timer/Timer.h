#pragma once


class Timer
{
public:
	Timer();

	void StartTimer();

	void StopTimer();

	void ResetTimer();

	// returns elapsed time in seconds
	float GetElapsedTime() const;

	static float ConvertSecondsToMS(float seconds);

protected:
	std::chrono::steady_clock::time_point m_StartTimePoint, m_EndTimePoint;

	bool m_bIsRunning = false;
};