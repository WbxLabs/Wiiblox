
#pragma once

#include "V8Datamodel/Mouse.h"
#include "V8Datamodel/Workspace.h"
namespace WBX {

	extern const char *const sPlayerMouse;

	class PlayerMouse : public DescribedNonCreatable<PlayerMouse, Mouse, sPlayerMouse>
	{
	private:
		typedef DescribedNonCreatable<PlayerMouse, Mouse, sPlayerMouse> Super;
	public:

		PlayerMouse();
		~PlayerMouse();

		TextureId getIcon() const;
		void setIcon(const TextureId& value);
	};

} // namespace
