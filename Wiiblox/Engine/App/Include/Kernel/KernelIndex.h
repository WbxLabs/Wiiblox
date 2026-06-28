
#pragma once

#include "wbx/Debug.h"

namespace WBX {

	class KernelIndex {
	protected:
		int		kernelIndex;

	public:
		bool indexInKernel() const {
			return (kernelIndex != -1);
		}

		KernelIndex() : kernelIndex(-1)
		{}

		~KernelIndex() {
			RBXASSERT(!indexInKernel());
		}
	};

} // namespace