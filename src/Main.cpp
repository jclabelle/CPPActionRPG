#define SDL_MAIN_HANDLED
//#define PROTOTYPING




#ifndef PROTOTYPING
#include "MainLoop.h"
int main()
{
	MainLoop();

//	return 0;
}
#endif

#ifdef PROTOTYPING
#include "TestMath.h"
int main()
{
	TestMath();
}
#endif





/*
Potential issues:
- Tileson does not play well with c++20. Compiling requires lots of #pragma warning disables spread through the tileson files. 

*/