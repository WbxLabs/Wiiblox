 /* Copyright 2003-2005 Wiiblox Corporation, All Rights Reserved */

#pragma once

#include "V8World/SpatialHashMultiRes.h"

namespace WBX
{
	class Primitive;
	class Contact;
	class ContactManager;
	class World;
	class Assembly;
	
#define CONTACTMANAGER_MAXLEVELS 4
	class ContactManagerSpatialHash : public SpatialHash<Primitive, Contact, ContactManager, CONTACTMANAGER_MAXLEVELS>
	{
	public:
		ContactManagerSpatialHash(World* world, ContactManager* contactManager);
	};

}


