#include "stdafx.h"

#include "V8DataModel/SurfaceSelection.h"
#include "V8DataModel/PartInstance.h"
#include "AppDraw/DrawAdorn.h"
#include "AppDraw/Draw.h"

namespace WBX {

const char* const sSurfaceSelection = "SurfaceSelection";

REFLECTION_BEGIN();
static const Reflection::EnumPropDescriptor<SurfaceSelection, NormalId>	prop_Surface("TargetSurface", category_Data, &SurfaceSelection::getSurface, &SurfaceSelection::setSurface);
REFLECTION_END();

SurfaceSelection::SurfaceSelection()
	:DescribedCreatable<SurfaceSelection, PartAdornment, sSurfaceSelection>("SurfaceSelection")
	,surfaceId(NORM_X)
{}

void SurfaceSelection::setSurface(NormalId value)
{
	if(surfaceId != value){
		surfaceId = value;
		raisePropertyChanged(prop_Surface);
	}
}

void SurfaceSelection::render3dAdorn(Adorn* adorn)
{
	if(getVisible()){
		if(shared_ptr<wbx::PartInstance> partInstance = adornee.lock()){
			DrawAdorn::partSurface(partInstance->getPart(), surfaceId, adorn, color);
		}
	}
}

}

// Randomized Locations for hackflags
namespace WBX 
{ 
    namespace Security
    {
        unsigned int hackFlag0 = 0;
    };
};
