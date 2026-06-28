#include "stdafx.h"

#include "v8datamodel/ServerStorage.h"
#include "V8DataModel/GameBasicSettings.h"
#include "network/Players.h"

using namespace WBX;

const char* const wbx::sServerStorage = "ServerStorage";

ServerStorage::ServerStorage(void)
{
	setName(sServerStorage);
}

bool ServerStorage::askAddChild(const Instance* instance) const
{
	return wbx::Network::Players::backendProcessing(this) || wbx::GameBasicSettings::singleton().inStudioMode();
}