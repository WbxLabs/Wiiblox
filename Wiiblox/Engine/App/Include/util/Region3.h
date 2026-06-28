#pragma once

#include "G3D/Vector3.h"
#include "G3D/CoordinateFrame.h"

namespace WBX {
	class Extents;

	class Region3 {
	private:
		G3D::CoordinateFrame cframe;
		G3D::Vector3 size;
		void init(const Extents &extents);

	public:
		Region3();
		Region3(const G3D::Vector3& min, const G3D::Vector3& max);
		explicit Region3(const Extents &extents);

		~Region3() {}

		const G3D::CoordinateFrame& getCFrame() const { return cframe; }
		const G3D::Vector3& getSize() const { return size; }

		G3D::Vector3 minPos() const;
		G3D::Vector3 maxPos() const;

		inline bool operator==(const Region3& other) const {
			return (size == other.size) && (cframe == other.cframe);
		}

		inline bool operator!=(const Region3& other) const {
			return !(*this == other);
		}
	};
}