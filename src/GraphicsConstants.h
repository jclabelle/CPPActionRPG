#pragma once
/* Screen size constants */
const int PhysicalScreenWidth{ 2560 / 1 };
const int PhysicalScreenHeight{ 1600 /1};
const int ScalingFactor{ 4 };
const int ScalingFactor_SDL{ 1 };
const int ScaledScreenWidth{ PhysicalScreenWidth / ScalingFactor };
const int ScaledScreenHeight{ PhysicalScreenHeight / ScalingFactor };



const double TargetFrameRate{ 60.f };
const double MsInSeconds{ 1000.f };
const double TargetMsPerFrame{ MsInSeconds / TargetFrameRate };
const double PhysicsUpdatesPerSecond{ 30.f };
const double TargetPhysicsFrameDeltaTime{ MsInSeconds / PhysicsUpdatesPerSecond };	// Gives a milliseconds target
const double TargetMaxFrameDeltaTime{ 150 };	// in milliseconds

/* zDepth:
0-99: reserved for background tiles
100-199: reserved for characters and other visuals at "ground-level", ex walking characters 
200-299: reserved for foreground tiles 
300-399: reserved for characters and and other visuals at "rooftop-level" and up, ex: flying characterse */
const int BackgroundTilesDepthMin{ 0 };
const int BackgroundTilesDepthMax{ 99 };
const int GroundCharactersDepthMin{ 100 };
const int GroundCharactersDepthMax{ 199 };
const int ForegroundTilesDepthMin{ 200 };
const int ForegroundTilesDepthMax{ 299 };
const int SkyCharactersDepthMin{ 300 };
const int SkyCharactersDepthMax{ 399 };