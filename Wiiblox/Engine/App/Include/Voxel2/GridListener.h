#pragma once

namespace WBX { namespace Voxel2 {

    class Region;
    
    class GridListener
	{
	public:
		virtual ~GridListener() {}

        virtual void onTerrainRegionChanged(const Region& region) = 0;
	};
    
} }
