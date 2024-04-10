#pragma once

#include<SDL.h>

struct Timer {
	Timer();

	void start();
	void stop();
	void pause();
	void unpause();

	float getTicks();

	bool isStarted();
	bool isPaused();

private:
	Uint64 startTicks;
	Uint64 pausedTicks;

	bool paused;
	bool started;
};