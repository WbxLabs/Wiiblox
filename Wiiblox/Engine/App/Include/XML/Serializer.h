#ifndef V8XML_SERIALIZER_H
#define V8XML_SERIALIZER_H

#pragma once

#include "SerializerV2.h"

#include "util/SoundService.h"

#include "v8datamodel/Workspace.h"
#include "v8datamodel/Lighting.h"
#include "v8datamodel/ServerStorage.h"
#include "v8datamodel/ReplicatedStorage.h"
#include "v8datamodel/ReplicatedFirst.h"
#include "v8datamodel/PlayerGui.h"
#include "v8datamodel/Hopper.h"
#include "v8datamodel/StarterPlayerService.h"
#include "v8datamodel/ServerScriptService.h"
#include "v8datamodel/CSGDictionaryService.h"

class Serializer : public SerializerV2 
{
public:
	static bool canWriteChild(const shared_ptr<wbx::Instance> instance, wbx::Instance::SaveFilter saveFilter)
	{
		if(!instance->getIsArchivable())
			return false;

		switch(saveFilter)
		{
		case wbx::Instance::SAVE_ALL:
			return true;

		case wbx::Instance::SAVE_WORLD:
			if ( wbx::Instance::fastDynamicCast<wbx::Workspace>(instance.get()) )
				return true;
			if ( wbx::Instance::fastDynamicCast<wbx::Lighting>(instance.get()) )
				return true;
			if ( wbx::Instance::fastDynamicCast<wbx::Soundscape::SoundService>(instance.get()) )
				return true;
			if ( wbx::Instance::fastDynamicCast<wbx::ServerStorage>(instance.get()) )
				return true;
			if ( wbx::Instance::fastDynamicCast<wbx::ReplicatedStorage>(instance.get()) )
				return true;
			if ( wbx::Instance::fastDynamicCast<wbx::CSGDictionaryService>(instance.get()) )
				return true;

			return false;

		case wbx::Instance::SAVE_GAME:
			if ( wbx::Instance::fastDynamicCast<wbx::StarterGuiService>(instance.get()) )
				return true;
			if ( wbx::Instance::fastDynamicCast<wbx::StarterPackService>(instance.get()) )
				return true;
			if ( wbx::Instance::fastDynamicCast<wbx::StarterPlayerService>(instance.get()) )
				return true;
			if ( wbx::Instance::fastDynamicCast<wbx::ServerScriptService>(instance.get()) )
				return true;
			if ( wbx::Instance::fastDynamicCast<wbx::ReplicatedFirst>(instance.get()) )
				return true;

			return false;

		default:
			return true;
		}
	}
};



#endif