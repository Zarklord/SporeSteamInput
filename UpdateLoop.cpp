#include "stdafx.h"
#include "UpdateLoop.h"


namespace UpdateLoop {
	long AttachDetours() {
		long result = 0;
		result |= Update__detour::attach(GetAddress(App::cCameraManager, Update));

		return result;
	}
}