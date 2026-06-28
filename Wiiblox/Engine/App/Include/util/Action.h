
#pragma once

//#include "Util/SoundWorld.h"
#include <string>


namespace WBX {

	class Action
	{
	public:
		enum ActionType {	NO_ACTION = 0,
									PAUSE_ACTION,
									LOSE_ACTION,
									DRAW_ACTION,
									WIN_ACTION, 
									NUM_ACTION_TYPES };
	private:
		Action();

};

}	// namespace WBX