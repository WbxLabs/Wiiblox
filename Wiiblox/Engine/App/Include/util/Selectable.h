/* 
  Copyright 2003-2006 Wiiblox Corporation, All Rights Reserved 
*/

#pragma once

namespace WBX 
{
	// Base class to control all selection functionality
	class RBXBaseClass Selectable
	{
	public:
		virtual bool isSelectable3d() { return true; }
	};
} // namespace
