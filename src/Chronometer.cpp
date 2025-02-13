#include "Chronometer.h"
#include "GraphicsConstants.h"

//const std::chrono::milliseconds OneSecond{ 1000 };
//const std::chrono::milliseconds TargetFrameRate{ 60 };
const std::chrono::milliseconds TargetRenderFrequency{ 1000/60 };

/* Static variables and methods */
// The Game Clock
std::chrono::steady_clock::time_point Chronometer::game_start_time = game_clock.now();

/* Static time points initialisation + double for returns */
std::chrono::time_point<std::chrono::steady_clock, std::chrono::milliseconds> Chronometer::GameStartTime = std::chrono::time_point_cast<std::chrono::milliseconds>(game_clock.now());
std::chrono::time_point<std::chrono::steady_clock, std::chrono::milliseconds> Chronometer::CurrentFrameStartTime = std::chrono::time_point_cast<std::chrono::milliseconds>(game_clock.now());
std::chrono::time_point<std::chrono::steady_clock, std::chrono::milliseconds> Chronometer::PreviousFrameStartTime = std::chrono::time_point_cast<std::chrono::milliseconds>(game_clock.now());
std::chrono::milliseconds Chronometer::ElapsedTime = Chronometer::CurrentFrameStartTime - Chronometer::PreviousFrameStartTime;
std::chrono::milliseconds Chronometer::LagTime{};

double Chronometer::DeltaAsDouble = 150.f;


/* Returns a time point taken at the start of the game */
std::chrono::time_point<std::chrono::steady_clock, std::chrono::milliseconds> Chronometer::viewGameStartTime()
{
	return GameStartTime;
}

std::chrono::time_point<std::chrono::steady_clock, std::chrono::milliseconds> Chronometer::viewGameElapsedTime()
{
	return std::chrono::time_point_cast<std::chrono::milliseconds>(game_clock.now()); 
}

void Chronometer::setCurrentFrameStartTime()
{
	CurrentFrameStartTime = std::chrono::time_point_cast<std::chrono::milliseconds>(game_clock.now());
}

void Chronometer::setPreviousFrameStartTime()
{
	PreviousFrameStartTime = CurrentFrameStartTime;
}

void Chronometer::resetPreviousFrameStartTime()
{
	PreviousFrameStartTime = std::chrono::time_point_cast<std::chrono::milliseconds>(game_clock.now());
}

/*
void Chronometer::setFrameDeltaTime()
{ 
	FrameDeltaTime = CurrentFrameStartTime - PreviousFrameStartTime;

	//* Convert to double 
	DeltaAsDouble = double(FrameDeltaTime.count());

	//* Guards to limit delta to 150 ms 
	if (DeltaAsDouble > TargetMaxFrameDeltaTime)
	{
		DeltaAsDouble = TargetMaxFrameDeltaTime;
	}
	///* Convert back to seconds 
	DeltaAsDouble = DeltaAsDouble / MsInSeconds;
}
*/

void Chronometer::setFrameDeltaTime()
{
	ElapsedTime = CurrentFrameStartTime - PreviousFrameStartTime;

	/* Convert to double */
	DeltaAsDouble = double(ElapsedTime.count());
}

double Chronometer::viewFrameDeltaTime()
{ 
	
	return DeltaAsDouble;
}

bool Chronometer::isUpdateDurationOver()
{
	return ElapsedTime >= TargetRenderFrequency;
}

void Chronometer::decrementFrameDeltaTime()
{
	ElapsedTime -= TargetRenderFrequency;
}

void Chronometer::runChronoFrameStart()
{
	CurrentFrameStartTime = std::chrono::time_point_cast<std::chrono::milliseconds>(game_clock.now());
	ElapsedTime = CurrentFrameStartTime - PreviousFrameStartTime;
	PreviousFrameStartTime = CurrentFrameStartTime;
	LagTime += ElapsedTime;
}


// Returns the time since the game started in milliseconds
double Chronometer::get_GameTime()
{
	std::chrono::duration<double, std::milli> game_duration = game_clock.now() - game_start_time;
	return game_duration.count();
}

/* Chronometer class */
Chronometer::Chronometer(bool is_started):
	// For the global clock only
	now{game_clock.now()}, 
	
	// For this Chronometer
	is_started{is_started},
	start_time{game_clock.now()}, 
	paused_time{game_clock.now()},
	running_duration{ std::chrono::duration<double, std::milli>::zero() },
	paused_duration{ std::chrono::duration<double, std::milli>::zero() }
{}



// Sets start_time and paused_time to the current time_point of the game_clock
void Chronometer::start()
{
	// Reset everything
	start_time = game_clock.now();
	paused_time = start_time;
	running_duration = std::chrono::milliseconds::zero();
	paused_duration = running_duration;
	is_started = true;
	is_paused = false;
}

// Pauses the Chronometer and saves the data needed to resume
void Chronometer::pause()
{

	if(is_paused==false && is_started==true)
	{
	paused_time = game_clock.now();
	is_paused = true;
	}
}

void Chronometer::resume()
{
	if(is_paused==true && is_started == true)
	{
	paused_duration += (game_clock.now() - paused_time);
	is_paused = false;
	}
}

// Resets the running time to zero
void Chronometer::stop()
{
	if(is_started==true)
	{
	is_started = false;
	}
}

double Chronometer::getTime()
{
	// Only return time of an active Chronometer
	if (is_started == false)
		return -1;
	
	// Get the total duration ran so far
	running_duration = game_clock.now() - start_time;

	// Return the running time - total duration of pauses if the chronometer is not paused
	if (is_paused==false)
	return running_duration.count() - paused_duration.count();
	
	/* We only update the total duration of pauses on resume so if paused
	   return the running time - total duration of pauses + duration of current as of now */
	if (is_paused == true)
	{
		std::chrono::duration<double, std::milli> current_pause_duration = game_clock.now() - paused_time;
		return running_duration.count() - (paused_duration.count() + current_pause_duration.count());

	}
	return -2;
}

double Chronometer::getTimeInSeconds()
{
	auto time = getTime();
	return time / 1000;
}


