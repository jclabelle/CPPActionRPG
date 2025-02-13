#include "ControlsInputPayload.h"

ControlsInputPayload::ControlsInputPayload(){}
ControlsInputPayload::ControlsInputPayload(UserInput input, double timestamp)
	:Input{input}, TimeStamp{timestamp} {}
ControlsInputPayload::~ControlsInputPayload(){}