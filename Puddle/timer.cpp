#include "timer.h"

Timer::Timer() {
	startTicks = 0;
	pausedTicks = 0;

	paused = false;
	started = false;
}

void Timer::start() {
	started = true;
	paused = false;

	startTicks = SDL_GetPerformanceCounter();
	pausedTicks = 0;
}

void Timer::stop() {
	started = false;
	paused = false;

	startTicks = 0;
	pausedTicks = 0;
}

void Timer::pause() {
	if (started && !paused) {
		paused = true;

		pausedTicks = SDL_GetPerformanceCounter() - startTicks;
		startTicks = 0;
	}
}

void Timer::unpause() {
	if (started && paused) {
		paused = false;

		startTicks = SDL_GetPerformanceCounter() - pausedTicks;
	}
}

float Timer::getTicks() {
	Uint64 time = 0;

	if (started) {
		if (paused) {
			time = pausedTicks;
		}
		else {
			time = SDL_GetPerformanceCounter() - startTicks;
		}
	}

	return time / 10000.0f;
}

bool Timer::isStarted() { return started; }
bool Timer::isPaused() { return paused && started; }