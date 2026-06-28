
#pragma once

#include "Util/Average.h"

namespace WBX {

	class Workspace;

	namespace Network {
		class Player;
	}


	class PhysicsInstructions 
	{
	private:
		Average<double> averageCyclicDt;
		Average<double> averageDt;
		Average<double> averageDutyDt;
		double timeSinceLastRadiusChange;

		double throttleTimer;
		double throttleAdjustTime;

		void changeSimulationRadius(wbx::Network::Player* dPhysPlayer, float change);
		void changeMaxSimulationRadius(wbx::Network::Player* dPhysPlayer, float change);

	public:
		double requestedDutyPercent;
		bool bandwidthExceeded;
		double networkBufferHealth;

		PhysicsInstructions();

		void setThrottles(wbx::Network::Player* dPhysPlayer, Workspace* workspace, double dt, double dutyDt);
		void setCyclicThrottles(wbx::Network::Player* dPhysPlayer, Workspace* workspace, double cyclicDt, double dt, double dutyDt);
		void setThrottlesBase( wbx::Network::Player* dPhysPlayer, Workspace* workspace, bool realTimePerfOK, bool dutyPerfOK, double avgDutyPercent, double dt); 

		static double visitSoloDutyPercent()			{return 0.50;}
		static double regularServerDutyPercent()		{return 0.25;}
		static double zeroDutyPercent()				{return 0.0;}
		static double dPhysicsServerDutyPercent();
		static double dPhysicsClientDutyPercent();
		static double dPhysicsClientEThrottleDutyPercent();
	};		// class
}		// namespace;
