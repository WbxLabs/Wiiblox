
#pragma once

#include "V8DataModel/MouseCommand.h"

namespace WBX
{
	class ICancelableTool
	{

	public:
		virtual shared_ptr<MouseCommand> onCancelOperation() = 0;

	};


} //namespace