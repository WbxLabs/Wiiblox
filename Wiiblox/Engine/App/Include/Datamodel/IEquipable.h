
#pragma once

#include "Util/G3DCore.h"
#include "wbx/Declarations.h"
#include "wbx/boost.hpp"

// Common base class for Tool, Accoutrement
//
//	IEquipable
//		Tool
//		Accoutrement


namespace WBX {

	class Weld;
	class PartInstance;
	class Workspace;

	class WBXInterface IEquipable
	{
	protected:

		// "Backend" properties - only tracked on the server side where all connection/destroy occurs
		shared_ptr<Weld> weld;					// Weld (I create/destroy) - no weld == dropped, UNEQUIPPED

		void buildWeld(
				PartInstance* humanoidPart, 
				PartInstance* gadgetPart,
				const CoordinateFrame& humanoidCoord,
				const CoordinateFrame& gadgetCoord,
				const std::string& name);

		IEquipable();

		virtual ~IEquipable();
	};

} // namespace
