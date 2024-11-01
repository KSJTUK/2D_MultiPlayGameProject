#include "pch.h"
#include "Timer.h"

GTime::GTime()
{
}

GTime::~GTime()
{
}

double GTime::SetTimeScale(double scale)
{
	auto temp = mTimeScale;
	mTimeScale = scale;
	return temp;
}


double GTime::GetTimeScale()
{
	return mTimeScale;
}

uint64_t GTime::GetFrameCount()
{
	return mFrameCount;
}


// �� �������� ���۵� ������ ȣ���� 
void GTime::AdvanceTime() {
	GTime::UpdateDeltaTime();
	GTime::SampleDeltaTime();
	GTime::AddScaledStarted();
	GTime::CheckEvent();
}

// Scene �� ���۵� �� ȣ�� 
void GTime::StartSceneTime()
{
	mSceneStarted = clock::now();
}

void GTime::UpdateDeltaTime()
{
	auto now = clock::now();
	mDeltaTime = std::chrono::duration_cast<duration>(now - mPrev);
	mPrev = now;
	mFrameCount++;
}
void GTime::AddScaledStarted()
{
	mScaledStarted += mDeltaTime * mTimeScale;
}

void GTime::SampleDeltaTime()
{
	mDeltaTimeBuffer[mDeltaTimeSampleingIndex] = mDeltaTime * mTimeScale;
	mDeltaTimeSampleingIndex = (mDeltaTimeSampleingIndex + 1) % mDeltaTimeBufferSize;
}

bool GTime::PopEvent()
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

void GTime::CheckEvent()
{
	if (mEvents.empty()) {
		return;
	}
	while (GTime::PopEvent());
}

GTime Time{};