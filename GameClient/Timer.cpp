#include "pch.h"
#include "Timer.h"

Timer::Timer()
{
}

Timer::~Timer()
{
}

double Timer::SetTimeScale(double scale)
{
	auto temp = mTimeScale;
	mTimeScale = scale;
	return temp;
}


double Timer::GetTimeScale()
{
	return mTimeScale;
}

uint64_t Timer::GetFrameCount()
{
	return mFrameCount;
}


// �� �������� ���۵� ������ ȣ���� 
void Timer::AdvanceTime() {
	Timer::UpdateDeltaTime();
	Timer::SampleDeltaTime();
	Timer::AddScaledStarted();
	Timer::CheckEvent();
}

// Scene �� ���۵� �� ȣ�� 
void Timer::StartSceneTime()
{
	mSceneStarted = clock::now();
}

void Timer::UpdateDeltaTime()
{
	auto now = clock::now();
	mDeltaTime = std::chrono::duration_cast<duration>(now - mPrev);
	mPrev = now;
	mFrameCount++;
}
void Timer::AddScaledStarted()
{
	mScaledStarted += mDeltaTime * mTimeScale;
}

void Timer::SampleDeltaTime()
{
	mDeltaTimeBuffer[mDeltaTimeSampleingIndex] = mDeltaTime * mTimeScale;
	mDeltaTimeSampleingIndex = (mDeltaTimeSampleingIndex + 1) % mDeltaTimeBufferSize;
}

bool Timer::PopEvent()
{
	auto ev = mEvents.top();
	mEvents.pop();

	if (ev.mInvokeTime < clock::now()) {
		std::invoke(ev.mCallBack);
		mEvents.emplace(clock::now() + ev.mTimeout, std::move(ev.mTimeout), std::move(ev.mCallBack));

		return true;
	}
	return false;
}

void Timer::CheckEvent()
{
	if (mEvents.empty()) {
		return;
	}
	while (Timer::PopEvent());
}