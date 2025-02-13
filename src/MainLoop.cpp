#define CAP_FRAMERATE

/* Project*/
#include "MainLoop.h"
#include "DataManager.h"
#include "GraphicsManager.h"
#include "GameStateManager.h"
#include "ControlsManager.h"
#include "Chronometer.h"
#include "CentralDispatch.h"
//#include "PAS_SDLInit.h"
#include "AssetFilePaths.h"
/* STL */
#include <memory>
#include <functional>
#include <filesystem>
#include <chrono>
/* Libraries*/


void MainLoop()
{
	/* Create Managers*/
	CentralDispatch Dispatch;
	DataManager Data;
	GraphicsManager Graphics;
	GameStateManager GameState;
	ControlsManager Controls;

	/* Init SDL */
	Graphics.InitializeSDL();

	/* Connect Central Dispatch to Managers */
	Dispatch.connectManager(&Data);
	Dispatch.connectManager(&Graphics);
	Dispatch.connectManager(&GameState);
	Dispatch.connectManager(&Controls);
	Dispatch.connectManager(&*GameState.accessCommandManager());


	/* Load World and Save Game */
	Data.loadWorldData();
	Data.loadSaveGame();
	//Data.PrintToCmdLine();
	
#ifdef CAP_FRAMERATE 
Chronometer FrameTimer;
double FrameDuration = 1.0 / 60.0;
#endif

	bool PlayerQuit{ false };
	while (!PlayerQuit)
	{
#ifdef CAP_FRAMERATE 
double FrameStart = FrameTimer.getTimeInSeconds(); 
#endif

		// Controls
		Controls.getInputs();
		const std::tuple<EntityEvent, EntityEvent>& ret_CommandsMovementAction = GameState.accessCommandManager()->processFrameInput_V2();

		// Run FrameLogic trough Active Area for all Entities
		GameState.run_FrameLogic_V2(ret_CommandsMovementAction);
	


		/* Send list of Visible Animations to Graphics */
		//GameState.accessGameCamera()->movetoFocus();
		GameState.sendVisibleAnimations(0);

		/* Draw Graphics */
		Graphics.drawClearRenderer();
		Graphics.drawFrame();
		Graphics.sendRendererToScreen();

		/* Frame Rate */



#ifdef CAP_FRAMERATE 
while (FrameTimer.getTimeInSeconds() < FrameStart+FrameDuration) { }
#endif
		/* Testing utility for frame breaks *///
		//SDL_Delay(100);

	}
}