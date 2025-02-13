//#define DEBUG_CONTROLS
/* Project */
#include "ControlsManager.h"
#include "Chronometer.h"
/* STL */
#include <iostream>
/* Libraries */
#include "SDL2/SDL.h"


// Reset all control flags
void ControlsManager::resetFlags()
{
	// Keyboard
	KeyW = false;
	KeyS = false;
	KeyA = false;
	KeyD = false;
	one = false;
	two = false;
	three = false;
	four = false;
	spacebar = false;

	// Game Controller
	dpad_up = false;
	dpad_down = false;
	dpad_left = false;
	dpad_right = false;
	dpad_a = false;
	dpad_b = false;
	dpad_x = false;
	dpad_y = false;
}
ControlsManager::ControlsManager()
{}

void ControlsManager::receiveFromCentralDispatch(std::any reqData, const eDataType& reqDataType)
{
	/*TODO: Code this*/
	reqData = reqDataType;
}

eManagerType ControlsManager::viewMyManagerType()
{
	return eManagerType(eManagerType::CONTROLS);
}

#pragma warning (disable: 4100)
std::any ControlsManager::searchForData(const eDataType& reqDataType, const std::string& reqDataName)
{
	return std::any(0);
}

void ControlsManager::pushFrameInputToCentralDispatch()
{
	for (auto Input : FrameInputBuffer)
	{
		pushToCentralDispatch(eManagerType::COMMANDS, eDataType::CONTROLSINPUT, Input);
	}
}

bool ControlsManager::getInputs()
{
	// Reset the controls state 
	resetFlags();

	// Input capture
	SDL_Event input;

	// Turns true if we enter the SDL_PollEvent loop
	bool FrameHadInputEvent{ false };

	// Loop until there are no more pending events
	while (SDL_PollEvent(&input))
	{
		// This frame has minimum 1 event
		FrameHadInputEvent = true;
		// If the player pressed KeyA keyboard key, turn on the appropriate flag
		if (input.type == SDL_KEYDOWN)
		{
			switch (input.key.keysym.scancode)
			{
			case SDL_SCANCODE_W:
				KeyW = true; w_time = Chronometer::get_GameTime(); addKeyW(); break;
			case SDL_SCANCODE_S:
				KeyS = true; s_time = Chronometer::get_GameTime(); addKeyS(); break;
			case SDL_SCANCODE_A:
				KeyA = true; a_time = Chronometer::get_GameTime(); addKeyA(); break;
			case SDL_SCANCODE_D:
				KeyD = true; d_time = Chronometer::get_GameTime(); addKeyD(); break;
			case SDL_SCANCODE_1:
				one = true; one_time = Chronometer::get_GameTime(); addKey1(); break;
			case SDL_SCANCODE_2:
				two = true; two_time = Chronometer::get_GameTime(); addKey2(); break;
			case SDL_SCANCODE_3:
				three = true; three_time = Chronometer::get_GameTime(); addKey3(); break;
			case SDL_SCANCODE_4:
				four = true; four_time = Chronometer::get_GameTime(); addKey4(); break;
			case SDL_SCANCODE_SPACE:
				spacebar = true; spacebar_time = Chronometer::get_GameTime(); addKeySpace(); break;
			case SDL_SCANCODE_X:	// Exit main loop
				return true;
			}
		}
		if (input.type == SDL_CONTROLLERBUTTONDOWN)
		{
			switch (input.cbutton.button)
			{
			case SDL_CONTROLLER_BUTTON_DPAD_UP:
				dpad_up = true; dpad_up_time = Chronometer::get_GameTime(); addDpadUp();  break;
			case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
				dpad_down = true; dpad_down_time = Chronometer::get_GameTime(); addDpadDown(); break;
			case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
				dpad_left = true; dpad_left_time = Chronometer::get_GameTime(); addDpadLeft(); break;
			case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
				dpad_right = true; dpad_right_time = Chronometer::get_GameTime(); addDpadRight(); break;
			case SDL_CONTROLLER_BUTTON_A:
				dpad_a = true; dpad_a_time = Chronometer::get_GameTime(); addDpadA(); break;
			case SDL_CONTROLLER_BUTTON_B:
				dpad_b = true; dpad_b_time = Chronometer::get_GameTime(); addDpadB(); break;
			case SDL_CONTROLLER_BUTTON_X:
				dpad_x = true; dpad_x_time = Chronometer::get_GameTime(); addDpadX(); break;
			case SDL_CONTROLLER_BUTTON_Y:
				dpad_y = true; dpad_y_time = Chronometer::get_GameTime(); addDpadY(); break;
			}
		}

		if (input.type == SDL_KEYUP)
		{
			switch (input.key.keysym.scancode)
			{
			case SDL_SCANCODE_W:
				KeyW = false; addKeyW_UP(); break;
			case SDL_SCANCODE_S:
				KeyS = false; addKeyS_UP(); break;
			case SDL_SCANCODE_A:
				KeyA = false; addKeyA_UP(); break;
			case SDL_SCANCODE_D:
				KeyD = false; addKeyD_UP(); break;
			case SDL_SCANCODE_SPACE:
				spacebar = false; addKeySpace_UP(); break;
			case SDL_SCANCODE_1:
				one = false; addKey1_Release(); break;
			}
		}
	}

#ifdef DEBUG_CONTROLS
	/* Add a bogus KeyW input to the FrameInputBuffer */
	addKeyD();
	FrameHadInputEvent = true;
#endif

	/* Skip updates to the Command Manager for frames with no valid input events */
	if (FrameHadInputEvent==true)
	{
		pushFrameInputToCentralDispatch();

	/* Empty the buffer to have a clean container for the next frame */
	FrameInputBuffer.clear();
	}
	return false;
}


double ControlsManager::getAgeA()
{
	return Chronometer::get_GameTime() - a_time;
}

double ControlsManager::getAgeA_UP()
{
	return Chronometer::get_GameTime() - a_time_up;
}

double ControlsManager::getAgeD()
{
	return  Chronometer::get_GameTime() - d_time;
}

double ControlsManager::getAgeD_UP()
{
	return  Chronometer::get_GameTime() - d_time_up;
}

double ControlsManager::getAgeFour()
{
	return  Chronometer::get_GameTime() - four_time;
}

double ControlsManager::getAgeOne()
{
	return  Chronometer::get_GameTime() - one_time;
}

double ControlsManager::getAgeS()
{
	return  Chronometer::get_GameTime() - s_time;
}

double ControlsManager::getAgeS_UP()
{
	return  Chronometer::get_GameTime() - s_time_up;
}

double ControlsManager::getAgeSpacebar()
{
	return  Chronometer::get_GameTime() - spacebar_time;
}

double ControlsManager::getAgeThree()
{
	return  Chronometer::get_GameTime() - three_time;
}

double ControlsManager::getAgeTwo()
{
	return  Chronometer::get_GameTime() - two_time;
}

double ControlsManager::getAgeW()
{
	return  Chronometer::get_GameTime() - w_time;
}

double ControlsManager::getAgeW_UP()
{
	return  Chronometer::get_GameTime() - w_time_up;
}

double ControlsManager::getAgeDpadA()
{
	return  Chronometer::get_GameTime() - dpad_a_time;
}

double ControlsManager::getAgeDpadB()
{
	return  Chronometer::get_GameTime() - dpad_b_time;
}

double ControlsManager::getAgeDpadX()
{
	return  Chronometer::get_GameTime() - dpad_x_time;
}

double ControlsManager::getAgeDpadY()
{
	return  Chronometer::get_GameTime() - dpad_y_time;
}

double ControlsManager::getAgeDpadDown()
{
	return  Chronometer::get_GameTime() - dpad_down_time;
}

double ControlsManager::getAgeDpadLeft()
{
	return  Chronometer::get_GameTime() - dpad_left_time;
}

double ControlsManager::getAgeDpadRight()
{
	return  Chronometer::get_GameTime() - dpad_right_time;
}

double ControlsManager::getAgeDpadUp()
{
	return  Chronometer::get_GameTime() - dpad_up_time;
}

#ifdef DEBUG_CONTROLS
void ControlsManager::debug_printControlsStateToCL()
{
	// Keyboard
	if (KeyA)std::cout << "W" << '\n';
	if (KeyS)std::cout << "S" << '\n';
	if (KeyA)std::cout << "A" << '\n';
	if (KeyD)std::cout << "D" << '\n';
	if (spacebar)std::cout << "SPACEBAR" << '\n';
	if (one)std::cout << "1" << '\n';
	if (two)std::cout << "2" << '\n';
	if (three)std::cout << "3" << '\n';
	if (four)std::cout << "4" << '\n';

	// Game Controller
	if (dpad_up)std::cout << "DPAD UP" << '\n';
	if (dpad_down)std::cout << "DPAD DOWN" << '\n';
	if (dpad_left)std::cout << "DPAD LEFT" << '\n';
	if (dpad_right)std::cout << "DPAD RIGHT" << '\n';
	if (dpad_a)std::cout << "DPAD A" << '\n';
	if (dpad_b)std::cout << "DPAD B" << '\n';
	if (dpad_x)std::cout << "DPAD X" << '\n';
	if (dpad_y)std::cout << "DPAD Y" << '\n';
}
#endif

// Keyboard
void ControlsManager::addKeyW() 
{
	auto KeyWFlag = ControlsInputPayload(UserInput::KeyW, Chronometer::get_GameTime());
	FrameInputBuffer.push_back(KeyWFlag);
	//std::cout << "W INPUT" << '\n';
}
void ControlsManager::addKeyW_UP()
{
	auto Input = ControlsInputPayload(UserInput::KeyW_UP, Chronometer::get_GameTime());
	FrameInputBuffer.push_back(Input);
}
void ControlsManager::addKeyA()
{
	auto KeyAFlag = ControlsInputPayload(UserInput::KeyA, Chronometer::get_GameTime());
	FrameInputBuffer.push_back(KeyAFlag);
	//std::cout << "A INPUT" << '\n';
}
void ControlsManager::addKeyA_UP()
{
	auto Input = ControlsInputPayload(UserInput::KeyA_UP, Chronometer::get_GameTime());
	FrameInputBuffer.push_back(Input);
}
void ControlsManager::addKeyS()
{
	auto KeySFlag = ControlsInputPayload(UserInput::KeyS, Chronometer::get_GameTime());
	FrameInputBuffer.push_back(KeySFlag);
	//std::cout << "S INPUT" << '\n';
}
void ControlsManager::addKeyS_UP()
{
	auto Input = ControlsInputPayload(UserInput::KeyS_UP, Chronometer::get_GameTime());
	FrameInputBuffer.push_back(Input);
}
void ControlsManager::addKeyD()
{
	auto KeyDFlag = ControlsInputPayload(UserInput::KeyD, Chronometer::get_GameTime());
	FrameInputBuffer.push_back(KeyDFlag);
	//std::cout << "D INPUT" << '\n';
}
void ControlsManager::addKeyD_UP()
{
	auto Input = ControlsInputPayload(UserInput::KeyD_UP, Chronometer::get_GameTime());
	FrameInputBuffer.push_back(Input);
}
void ControlsManager::addKey1()
{
	auto Key1Flag = ControlsInputPayload(UserInput::Key1, Chronometer::get_GameTime());
	FrameInputBuffer.push_back(Key1Flag);
}
void ControlsManager::addKey1_Release()
{
	auto Input = ControlsInputPayload(UserInput::Key1_R, Chronometer::get_GameTime());
	FrameInputBuffer.push_back(Input);
}
void ControlsManager::addKey2()
{
	auto Key2Flag = ControlsInputPayload(UserInput::Key2, Chronometer::get_GameTime());
	FrameInputBuffer.push_back(Key2Flag);
}
void ControlsManager::addKey3()
{
	auto Key3Flag = ControlsInputPayload(UserInput::Key3, Chronometer::get_GameTime());
	FrameInputBuffer.push_back(Key3Flag);
}
void ControlsManager::addKey4() 
{
	auto Key4Flag = ControlsInputPayload(UserInput::Key4, Chronometer::get_GameTime());
	FrameInputBuffer.push_back(Key4Flag);
}
void ControlsManager::addKeySpace()
{
	auto KeySpaceFlag = ControlsInputPayload(UserInput::KeySpace, Chronometer::get_GameTime());
	FrameInputBuffer.push_back(KeySpaceFlag);
}
void ControlsManager::addKeySpace_UP()
{
	auto KeySpaceFlag = ControlsInputPayload(UserInput::KeySpace_UP, Chronometer::get_GameTime());
	FrameInputBuffer.push_back(KeySpaceFlag);
}
// Game Controller
void ControlsManager::addDpadA()
{
	auto DpadAFlag = ControlsInputPayload(UserInput::DpadA, Chronometer::get_GameTime());
	FrameInputBuffer.push_back(DpadAFlag);
}
void ControlsManager::addDpadB() 
{
	auto DpadBFlag = ControlsInputPayload(UserInput::DpadB, Chronometer::get_GameTime());
	FrameInputBuffer.push_back(DpadBFlag);
}
void ControlsManager::addDpadX()
{
	auto DpadXFlag = ControlsInputPayload(UserInput::DpadX, Chronometer::get_GameTime());
	FrameInputBuffer.push_back(DpadXFlag);
}
void ControlsManager::addDpadY()
{
	auto DpadYFlag = ControlsInputPayload(UserInput::DpadY, Chronometer::get_GameTime());
	FrameInputBuffer.push_back(DpadYFlag);
}
void ControlsManager::addDpadUp()
{
	auto DpadUpFlag = ControlsInputPayload(UserInput::DpadUp, Chronometer::get_GameTime());
	FrameInputBuffer.push_back(DpadUpFlag);
}
void ControlsManager::addDpadDown()
{
	auto DpadDownFlag = ControlsInputPayload(UserInput::DpadDown, Chronometer::get_GameTime());
	FrameInputBuffer.push_back(DpadDownFlag);
}
void ControlsManager::addDpadLeft() 
{
	auto DpadLeftFlag = ControlsInputPayload(UserInput::DpadLeft, Chronometer::get_GameTime());
	FrameInputBuffer.push_back(DpadLeftFlag);
}
void ControlsManager::addDpadRight() 
{
	auto DpadRightFlag = ControlsInputPayload(UserInput::DpadRight, Chronometer::get_GameTime());
	FrameInputBuffer.push_back(DpadRightFlag);
}

void ControlsManager::OpenGameController(SDL_GameController*& GamePad, int Joystick_Index)
{
	// Verify GameControllerEvents and activate them if needed
	if (SDL_GameControllerEventState(SDL_QUERY) == SDL_IGNORE)
		SDL_GameControllerEventState(SDL_ENABLE);

	// Abord if there are no joysticks connecteed
	if (SDL_NumJoysticks <= 0)
	{
		std::cout << "Warning: no Joysticks or GamePad detected\n";
		return;
	}

	// Abord if the controller at the provided index is not KeyA gamepad
	if (!SDL_IsGameController(Joystick_Index))
	{
		std::cout << "Warning: Joystick Index #" << Joystick_Index << "is not a Game Controller\n";
		return;
	}

	// Open the Controller
	GamePad = SDL_GameControllerOpen(0);
	if (GamePad == nullptr)
		printf("Warning: Unable to open game controller! SDL Error: %s\n", SDL_GetError());
}