#include "Timer.h"

void Timer::startTimer()
{
	std::lock_guard<std::mutex> lg(mMtx);
	mRunning = true;
	mStartTime = std::chrono::high_resolution_clock::now();
}

void Timer::startTimerFromEnd(uint32_t pEnd)
{
	std::lock_guard<std::mutex> lg(mMtx);
	mRunning = true;
	mEndTime = std::chrono::high_resolution_clock::now();
}

void Timer::stopTimer()
{
	std::lock_guard<std::mutex> lg(mMtx);
	mRunning = false;
	mEndTime = std::chrono::high_resolution_clock::now();
}

void Timer::resetTimer()
{
	mStartTime = std::chrono::high_resolution_clock::now();
}

void Timer::setProperFPS(const Uint32 pDelay)
{
	stopTimer();
	float elapsedTime = getDeltaTime(false);
	if (elapsedTime < pDelay)
		SDL_Delay(static_cast<Uint32>(pDelay - elapsedTime));
	else
		SDL_Delay(pDelay);
}

void Timer::setLimit(const float pLimit)
{
	mLimit = pLimit;
}

void Timer::setDimensionOfTime(const Dimension pType)
{
	mDimension = pType;
}

bool Timer::isRunning() const
{
	return mRunning;
}

bool Timer::isLimit()
{
	if (getDeltaTime(false) >= mLimit || getDeltaTime(true) <= mLimit)
		return true;
	return false;
}

float Timer::getLimit()
{
	return mLimit;
}

float Timer::getDeltaTime(bool pStopTimer)
{
	using namespace std::chrono;

	if (mRunning && pStopTimer)
		stopTimer();
	else
		mEndTime = high_resolution_clock::now();

	if (mDimension == Dimension::MILISECONDS)
		return static_cast<float>(duration_cast<milliseconds>(mEndTime - mStartTime).count());
	else if (mDimension == Dimension::SECONDS)
		return static_cast<float>(duration_cast<seconds>(mEndTime - mStartTime).count());
	else if (mDimension == Dimension::MINUTES)
		return static_cast<float>(duration_cast<minutes>(mEndTime - mStartTime).count());

	return 0.0f;
}

std::chrono::time_point<std::chrono::high_resolution_clock> Timer::getCurrentTimeOfTimer() const
{
	using namespace std::chrono;

	if (mRunning)
	{
		time_point<high_resolution_clock> time = high_resolution_clock::now();
		if (mDimension == Dimension::SECONDS)
			return time_point_cast<seconds>(time);
		else if (mDimension == Dimension::MINUTES)
			return time_point_cast<minutes>(time);
		else
			return time_point_cast<milliseconds>(time);
	}
	else
	{
		std::cout << "Time is not running\n";
		return std::chrono::high_resolution_clock::now();
	}
}