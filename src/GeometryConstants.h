#pragma once
// Project
#include "GeometryStructures.h"
#include "EntityStates.h"

// STL
#include <numeric>
#include <cmath>

// Directions in degrees
const double Degrees_Up{ Vector2D{0,-1}.Normalized().Degrees() };
const double Degrees_Up_ForceNegative{ Vector2D{-0.000000000001,-1}.Normalized().Degrees() };
const double Degrees_Down{ Vector2D{0,1}.Normalized().Degrees() };
const double Degrees_Left{ Vector2D{-1,0}.Normalized().Degrees() };
const double Degrees_Right{ Vector2D{1,0}.Normalized().Degrees() };
const double Degrees_UpLeft{ Vector2D{-1,-1}.Normalized().Degrees() };
const double Degrees_UpRight{ Vector2D{1,-1}.Normalized().Degrees() };
const double Degrees_DownLeft{ Vector2D{-1,1}.Normalized().Degrees() };
const double Degrees_DownRight{ Vector2D{1,1}.Normalized().Degrees() };

const double Midpoint_Degrees_Up_UpRight{ std::midpoint(Degrees_Up, Degrees_UpRight) };
const double Midpoint_Degrees_UpRight_Right{ std::midpoint(Degrees_UpRight, Degrees_Right) };
const double Midpoint_Degrees_Right_DownRight{ std::midpoint(Degrees_Right, Degrees_DownRight) };
const double Midpoint_Degrees_DownRight_Down{ std::midpoint(Degrees_DownRight, Degrees_Down) };
const double Midpoint_Degrees_Down_DownLeft{ std::midpoint(Degrees_Down, Degrees_DownLeft) };
const double Midpoint_Degrees_DownLeft_Left{ std::midpoint(Degrees_DownLeft, Degrees_Left) };
const double Midpoint_Degrees_Left_UpLeft{ std::midpoint(Degrees_Left, Degrees_UpLeft) };
const double Midpoint_Degrees_UpLeft_Up{ std::midpoint(Degrees_UpLeft, Degrees_Up_ForceNegative) };

const double Minimum_Distance_To_AStarNode{ 1 };
