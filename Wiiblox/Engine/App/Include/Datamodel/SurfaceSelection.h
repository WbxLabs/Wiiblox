
#pragma once

#include "V8DataModel/Adornment.h"
#include "GfxBase/IAdornable.h"

namespace WBX
{
	extern const char* const sSurfaceSelection;

	class SurfaceSelection
		: public DescribedCreatable<SurfaceSelection, PartAdornment, sSurfaceSelection>
	{
	public:
		SurfaceSelection();

		////////////////////////////////////////////////////////////////////////////////////
		// 
		// IAdornable
		/*override*/ void render3dAdorn(Adorn* adorn);

		NormalId getSurface() const { return surfaceId; }
		void setSurface(NormalId value);

	private:
		NormalId surfaceId;

	};

}


