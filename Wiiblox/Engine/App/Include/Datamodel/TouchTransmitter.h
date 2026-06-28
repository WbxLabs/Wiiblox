#pragma once


#include "V8Tree/Instance.h"

namespace WBX {

	class PartInstance;
	class TouchDebouncer;

	extern const char *const sTouchTransmitter;

	class TouchTransmitter : public DescribedCreatable<TouchTransmitter, Instance, sTouchTransmitter, Reflection::ClassDescriptor::INTERNAL>
			, public Diagnostics::Countable<TouchTransmitter>
	{
	private:
		typedef DescribedCreatable<TouchTransmitter, Instance, sTouchTransmitter, Reflection::ClassDescriptor::INTERNAL> Super;
		boost::scoped_ptr<TouchDebouncer> touchDebouncer;
	public:
		TouchTransmitter();
		~TouchTransmitter();
		bool checkTouch(const shared_ptr<PartInstance>& other);
		bool checkUntouch(const shared_ptr<PartInstance>& other);
	};

} // namespace
