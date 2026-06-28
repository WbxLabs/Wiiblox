//
//  AdService.h
//  Copyright Wiiblox Corp 2014
//
//  Created by Ben Tkacheff on 4/16/14.
//
//

#pragma once

#include "V8Tree/Instance.h"
#include "V8Tree/Service.h"
#include "V8DataModel/UserInputService.h"

namespace WBX
{
	extern const char* const sAdService;
	class AdService
    : public DescribedCreatable<AdService, Instance, sAdService, Reflection::ClassDescriptor::INTERNAL>
    , public Service
	{
	private:
		typedef DescribedCreatable<AdService, Instance, sAdService, Reflection::ClassDescriptor::INTERNAL> Super;

		bool showingVideoAd;

		std::string platformToWebString(const UserInputService::Platform userPlatform);
		bool canUseService();
	protected:
		/*override*/ void onServiceProvider(ServiceProvider* oldProvider, ServiceProvider* newProvider);
	public:
		AdService();
        
        wbx::signal<void(bool)> videoAdClosedSignal;
        wbx::signal<void()> playVideoAdSignal;
        
        wbx::remote_signal<void(int, UserInputService::Platform)> sendServerVideoAdVerification;
		wbx::remote_signal<void(bool, int, std::string)> sendClientVideoAdVerificationResults;

		wbx::remote_signal<void(int, UserInputService::Platform, bool)> sendServerRecordImpression;
        
        void showVideoAd();
        void videoAdClosed(bool didPlay);

		void sendAdImpression(int userId, UserInputService::Platform platform, bool didPlay);

		void verifyCanPlayVideoAdReceivedResponseNoDMLock(const std::string& response, int userId);
		void verifyCanPlayVideoAdReceivedErrorNoDMLock(const std::string& error, int userId);

		void verifyCanPlayVideoAdReceivedError(const std::string& error, int userId);

		void checkCanPlayVideoAd(int userId, UserInputService::Platform userPlatform);
        void receivedServerShowAdMessage(const bool success, int userId, const std::string& errorMessage);
	};
}