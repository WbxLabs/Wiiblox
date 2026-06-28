#include "stdafx.h"

#include "security/SecurityContext.h"
#include "wbx/Thread.hpp"
#include "wbx/Debug.h"

namespace WBX
{
	namespace Security
	{
		bool Context::isInRole(Identities identity, Permissions p)
		{
			if (p == None)
				return true;

			switch (identity)
			{
			case Anonymous:
			case GameScript_:
				return false;
#if defined(WBX_STUDIO_BUILD)
            case StudioPlugin:
                return p == Plugin;
#endif
			case GameScriptInWiibloxPlace_:
				return                p == WiibloxPlace;
			case WiibloxGameScript_:
				return p == Plugin || p == WiibloxPlace || p == LocalUser ||                      p == WiibloxScript;
			case LocalGUI_:
			case CmdLine_:
				return p == Plugin || p == WiibloxPlace || p == LocalUser;
			case Replicator_:
				return                p == WiibloxPlace ||                   p == WritePlayer  || p == WiibloxScript;
			case COM:
			case WebService:
				return true;
			default:
				WBXASSERT(false);
				return false;
			}
		}

		Context& Context::current()
		{
			Context* t = ptr().get();
			if (!t)
			{
				static Context anonymous(Anonymous);
				t = &anonymous;
				ptr().reset(t);
			}
			return *t;
		}


		void Context::tssCleanup(Context*)
		{

		}

		boost::thread_specific_ptr<Context>& Context::ptr()
		{
			static boost::thread_specific_ptr<Context> value(tssCleanup);
			return value;
		}

		Impersonator::Impersonator(Identities identity)
			  : current(identity)
		{
			previous = Context::ptr().get();
			Context::ptr().reset(&current);
		}

		Impersonator::~Impersonator()
		{
			Context::ptr().reset(previous);
		}


	}
}
