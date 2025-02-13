#pragma once
// Project
#include "ControlsInputPayload.h"
#include "SystemManagerBase.h"
// STL
#include <memory>
#include <vector>
#include <functional>
// Libraries
#include "SDL2/SDL.h"

// Values from user controls events
struct ControlsManager : public SystemManagerBase
{ 
	ControlsManager();

	/* System Manager Base Virtual Inheritance */
	virtual void receiveFromCentralDispatch(std::any reqData, const eDataType& reqDataType);
	virtual eManagerType viewMyManagerType();
	virtual std::any searchForData(const eDataType& reqDataType, const std::string& reqDataName);

	void pushFrameInputToCentralDispatch();

	// Gamepad
	SDL_GameController* gamepad = nullptr;

	// Initializes the provided Game Controller at JoystickIndex # (default 0)
	void OpenGameController(SDL_GameController*& GamePad, int JoystickIndex = 0);

	// Reset all flag values to false
	void resetFlags();

	// Grab user input values for the current frame and push to Central Dispatch
	bool getInputs();

	/*Flags for control status in the current frame
	and time stamp of the most recent press*/

	// Keyboard Control
	bool KeyA{ false };
	bool KeyD{ false };
	bool four{ false };
	bool one{ false };
	bool KeyS{ false };
	bool spacebar{ false };
	bool three{ false };
	bool two{ false };
	bool KeyW{ false };
	double four_time{ 0 };
	double one_time{ 0 };
	double spacebar_time{ 0 };
	double three_time{ 0 };
	double two_time{ 0 };
	double w_time{ 0 };
	double a_time{ 0 };
	double s_time{ 0 };
	double d_time{ 0 };
	double w_time_up{ 0 };
	double a_time_up{ 0 };
	double s_time_up{ 0 };
	double d_time_up{ 0 };



	// Game Controller
	bool dpad_a{ false };
	bool dpad_b{ false };
	bool dpad_down{ false };
	bool dpad_left{ false };
	bool dpad_right{ false };
	bool dpad_up{ false };
	bool dpad_x{ false };
	bool dpad_y{ false };
	double dpad_a_time{ 0 };
	double dpad_b_time{ 0 };
	double dpad_down_time{ 0 };
	double dpad_left_time{ 0 };
	double dpad_right_time{ 0 };
	double dpad_up_time{ 0 };
	double dpad_x_time{ 0 };
	double dpad_y_time{ 0 };

	// Get the age of most recent key presses
	double getAgeA();
	double getAgeA_UP();
	double getAgeD();
	double getAgeD_UP();
	double getAgeFour();
	double getAgeOne();
	double getAgeS();
	double getAgeS_UP();
	double getAgeSpacebar();
	double getAgeThree();
	double getAgeTwo();
	double getAgeW();
	double getAgeW_UP();

	double getAgeDpadA();
	double getAgeDpadB();
	double getAgeDpadX();
	double getAgeDpadY();
	double getAgeDpadDown();
	double getAgeDpadLeft();
	double getAgeDpadRight();
	double getAgeDpadUp();


	/* Manager -> Manager Communications */
	//std::function<void(std::vector<std::unique_ptr<ControlsInputPayload>>)> giveFrameInputToCommandManager;

	//void sendFrameInputToCommandManager(std::vector<std::unique_ptr<ControlsInputPayload>> NextFrameInput);
	/* Container for Frame input */
	std::vector<ControlsInputPayload> FrameInputBuffer;

	/* Add to the list of inputs for the frame */
	// Keyboard
	void addKeyW();
	void addKeyW_UP();
	void addKeyA();
	void addKeyA_UP();
	void addKeyS();
	void addKeyS_UP();
	void addKeyD();
	void addKeyD_UP();
	void addKey1();
	void addKey1_Release();
	void addKey2();
	void addKey3();
	void addKey4();
	void addKeySpace();
	void addKeySpace_UP();

	// Game Controller
	void addDpadA();
	void addDpadB();
	void addDpadX();
	void addDpadY();
	void addDpadUp();
	void addDpadDown();
	void addDpadLeft();
	void addDpadRight();

	//#ifdef DEBUG_PAS
	void debug_printControlsStateToCL();
	//#endif

};

