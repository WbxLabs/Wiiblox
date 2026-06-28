#include "stdafx.h"

#include "v8datamodel/WiibloxReplicatedStorage.h"

using namespace WBX;

const char* const wbx::sWiibloxReplicatedStorage = "WiibloxReplicatedStorage";

WiibloxReplicatedStorage::WiibloxReplicatedStorage(void)
{
	setName(sWiibloxReplicatedStorage);
	setWiibloxLocked(true);
}
