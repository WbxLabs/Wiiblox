
#pragma once

#include "Util/Region2.h"
#include "Util/SystemAddress.h"
#include <string>

namespace WBX {

	class SimSendFilter {
	public:
		typedef enum {EditVisit, Client, Server, dPhysClient, dPhysServer} Mode;

		Mode					mode;
		wbx::SystemAddress	networkAddress;
		Region2					region;
		
		SimSendFilter() : mode(Client)
		{}
	};

} // namespace
