#pragma once
#include "GraphicsConstants.h"
#include <chrono>

/* Chronometer class with basic functionality to keep track of time intervals 
    starts at the moment of construction unless false is passed as an argument */
class Chronometer
{
public:
	// Constructor
	Chronometer(bool is_started = true);

	/* Static methods */
	static double get_GameTime();
	static std::chrono::time_point<std::chrono::steady_clock, std::chrono::milliseconds> viewGameStartTime();
	static std::chrono::time_point<std::chrono::steady_clock, std::chrono::milliseconds> viewGameElapsedTime();
	static void setCurrentFrameStartTime();
	static void setPreviousFrameStartTime();
	static void resetPreviousFrameStartTime();
	static void setFrameDeltaTime();
	static double viewFrameDeltaTime();
	static bool isUpdateDurationOver();
	static void decrementFrameDeltaTime();
	static void runChronoFrameStart();

	// Starts or restarts the Chronometer
	void start();

	// Pauses the Chronometer at the current duration
	void pause();

	// Resumes KeyA paused Chronometer
	void resume();

	// Stops the Chronometer
	void stop();

	// Retrieve the time since start
	double getTime();
	double getTimeInSeconds();

private:
	
	/* The global game clock, a time point created at the start of the game
	and a time point used to capture the current time in get_GameTime */
	static std::chrono::steady_clock game_clock;
	static std::chrono::steady_clock::time_point game_start_time;
	static std::chrono::time_point<std::chrono::steady_clock, std::chrono::milliseconds> GameStartTime;
	static std::chrono::time_point<std::chrono::steady_clock, std::chrono::milliseconds> PreviousFrameStartTime;
	static std::chrono::time_point<std::chrono::steady_clock, std::chrono::milliseconds> CurrentFrameStartTime;
	static std::chrono::milliseconds ElapsedTime;
	static std::chrono::milliseconds LagTime;

	static double DeltaAsDouble;

	std::chrono::steady_clock::time_point now;

	/* Members for Chronometer feature*/
	bool is_paused{ false };
	bool is_started{ true };
	std::chrono::steady_clock::time_point start_time;
	std::chrono::steady_clock::time_point paused_time;
	std::chrono::duration<double, std::milli>  paused_duration;
	std::chrono::duration<double, std::milli>  running_duration;
};

