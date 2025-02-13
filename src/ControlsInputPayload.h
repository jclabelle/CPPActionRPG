#pragma once

enum class UserInput
{
	 KeyW,
	 KeyW_UP,
	 KeyA,
	 KeyA_UP,
	 KeyS,
	 KeyS_UP,
	 KeyD,
	 KeyD_UP,
	 Key1,
	 Key1_R,
	 Key2,
	 Key2_R,
	 Key3,
	 Key3_R,
	 Key4,
	 Key4_R,
	 KeySpace,
	 KeySpace_UP,
	 DpadA,
	 DpadB,
	 DpadX,
	 DpadY,
	 DpadUp,
	 DpadDown,
	 DpadLeft,
	 DpadRight,

	 USERINPUT_MAX
};

struct ControlsInputPayload
{
	ControlsInputPayload();
	ControlsInputPayload(UserInput Input, double TimeStamp);
	//ControlsInputPayload(const ControlsInputPayload&);
	~ControlsInputPayload();
	UserInput Input{ UserInput::USERINPUT_MAX };
	double TimeStamp{ 0.f };
};