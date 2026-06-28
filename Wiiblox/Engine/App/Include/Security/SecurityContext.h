
#pragma once
#include "RbxFormat.h"
#include "wbx/boost.hpp"
#include "g3d/format.h"

namespace boost
{
	template<typename T> class thread_specific_ptr;
}

namespace WBX
{
	namespace Security
	{
		typedef enum { 
			Anonymous = 0, 
			LocalGUI_,					// Any action initiated by Wiiblox Studio or the mouse 
			GameScript_,				// Execution of a BaseScript object inside any DataModel
			GameScriptInWiibloxPlace_,	// Execution of a BaseScript object inside any DataModel, if the place was authored by Wiiblox
			WiibloxGameScript_,			// Execution of a BaseScript object written by Wiiblox inside any DataModel
			CmdLine_,					// Any script executed from the Studio command line
#if defined(WBX_STUDIO_BUILD)
			StudioPlugin,				// Any Studio plug-in script
#endif
			COM,						// Scripts executed via the COM API (usually comes from freblx.xyz)
			WebService,					// Scripts executed via the Web Service API (usually comes from freblx.xyz)
			Replicator_,				// Receiving data via replication
			COUNT_Identities            // Not a true identity. Used for enumeration
		} Identities;

		typedef enum { 
			None				=0,     // Any identity can access this feature, including in-game scripts
            Plugin              =1,     // Second-lowest access level, just above in-game script
			WiibloxPlace			=2,     // A Wiiblox place that we own. Therefore scripts are more trusted and we allow
										// preliminary features
			LocalUser			=3,     // non-game permission. Usually for IDE
			WritePlayer			=4,		// Permissions for changing player name, userId, etc.
			WiibloxScript		=5,     // A script, such as a CoreScript, that we run inside a game
			Wiiblox				=6,     // Highest level of permission

#ifdef WBX_TEST_BUILD
			TestLocalUser       =None,  //For exposing Lua functions to the ReleaseTest build
#else
			TestLocalUser       =LocalUser,
#endif
		} Permissions;
        
        // different classes of VM that derive from the permission level
        typedef enum {
            VM_Default = 0,             // most scripts go here
#if defined(WBX_STUDIO_BUILD)
			VM_StudioPlugin,            // Sandbox for studio plugin scripts
#endif
            VM_WiibloxScriptPlus,            // scripts with the permission level of WiibloxScript or higher go here
            COUNT_VM_Classes
        } VMClasses;

		class Impersonator;

		class Context
		{
			friend class Impersonator;
					
		public:
			const Identities identity;
			static Context& current();

			// Throws an exception if the current thread's Context doesn't have the requested Role
			void requirePermission(Permissions permission, const char* operation = 0) const
			{
				if (!isInRole(identity, permission)) {
#ifndef _DEBUG
					// obfuscate error string
					// TODO: Can we obfuscate the code without obfuscating the error? 
					// Daniel: NO
					//if (operation)
					//	throw wbx::runtime_error("s %s", operation);
					//else
					//	throw wbx::runtime_error("s");
					throw std::runtime_error("");
#else
                    if (operation) {
						throw RBX::runtime_error("The current identity (%d) cannot %s (requires %d)", identity, operation, permission);
					} else {
						throw RBX::runtime_error("The current identity (%d) cannot perform the requested operation (requires %d)", identity, permission);
                    }
#endif
                }
			}

			bool hasPermission(Permissions permission)
			{
				return isInRole(identity, permission);
			}

			static bool isInRole(Identities identity, Permissions permission);

			static void tssCleanup(Context*);

		private:
			Context(Identities identity):identity(identity) {}
			static boost::thread_specific_ptr<Context>& ptr();
		};

		// Impersonates an identity for the lifetime of the object
		class Impersonator
		{
            Context current;
			Context* previous;
		public:
			Impersonator(Identities identity);
			~Impersonator();
		};
	}
}


