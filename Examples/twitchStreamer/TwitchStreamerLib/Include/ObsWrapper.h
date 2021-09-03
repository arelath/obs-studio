#pragma once

#include <memory>

#include "obs.h"

#include "Framework.h"
#include "SourceEnumerator.h"

class Logger;

class ObsWrapper
{
public:
	
	virtual ~ObsWrapper();

	static std::shared_ptr<ObsWrapper> CreateOBS();
	static std::shared_ptr<ObsWrapper> CreateOBSFromWindow(HWND hwnd);

	bool IsReady() { return mIsReady; }
	

	bool Start();


private:
	ObsWrapper();

	ObsWrapper(const ObsWrapper&);                 // Prevent copy-construction
	ObsWrapper& operator=(const ObsWrapper&);      // Prevent assignment

	static std::shared_ptr<ObsWrapper> CreateOBS(
		uint32_t baseWidth,
		uint32_t baseHeight,
		uint32_t outputWidth,
		uint32_t outputHeight);

	std::shared_ptr<Logger> mLogger;
	std::shared_ptr<SourceEnumerator> mSourceEnumerator;

	bool mIsReady = false;

};

