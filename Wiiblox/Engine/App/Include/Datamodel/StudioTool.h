
#pragma once
#include "V8Tree/Instance.h"
#include "V8Tree/Verb.h"

namespace WBX {
	class Workspace;
	class Mouse;

	extern const char *const sStudioTool;

	class StudioTool 
		: public DescribedNonCreatable<StudioTool, Instance, sStudioTool>
	{
	protected:
		shared_ptr<Mouse> onEquipping(Workspace* workspace);
		bool enabled;
	public:
		StudioTool();

		bool getEnabled() const { return enabled; }
		void setEnabled(bool);

		void activate();
		void deactivate();

		void equip(Workspace*);
		void unequip();

		wbx::signal<void(shared_ptr<Instance>)> equippedSignal;
		wbx::signal<void()> activatedSignal;
		wbx::signal<void()> unequippedSignal;
		wbx::signal<void()> deactivatedSignal;
	};
}
