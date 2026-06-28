#include "stdafx.h"

#include "v8datamodel/ReplicatedStorage.h"

using namespace WBX;

const char* const wbx::sReplicatedStorage = "ReplicatedStorage";

ReplicatedStorage::ReplicatedStorage(void)
{
	setName(sReplicatedStorage);
}
