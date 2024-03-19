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

Uint64 Timer::getTicks() {
	Uint64 time = 0;

	if (started) {
		if (paused) {
			time = pausedTicks;
		}
		else {
			time = SDL_GetPerformanceCounter() - startTicks;
		}
	}

	return time;
}

bool Timer::isStarted() { return started; }
bool Timer::isPaused() { return paused && started; }