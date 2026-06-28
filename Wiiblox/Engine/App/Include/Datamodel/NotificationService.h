//
//  NotificationService.h
//  Copyright Wiiblox Corp 2014
//
//  Created by Ganesh Agrawal on 5/15/14.
//
//

#pragma once

#include "V8Tree/Instance.h"
#include "V8Tree/Service.h"

namespace WBX
{
	extern const char* const sNotificationService;
	class NotificationService
    : public DescribedNonCreatable<NotificationService, Instance, sNotificationService, Reflection::ClassDescriptor::INTERNAL>
    , public Service
	{
	private:
		typedef DescribedNonCreatable<NotificationService, Instance, sNotificationService, Reflection::ClassDescriptor::INTERNAL> Super;
        
		bool canUseService();
        
	public:
		NotificationService();
        
        wbx::signal<void(int, int, std::string, int)> scheduleNotificationSignal;
        wbx::signal<void(int, int)> cancelNotificationSignal;
        wbx::signal<void(int)> cancelAllNotificationSignal;
        wbx::signal<void(int, boost::function<void(shared_ptr<const Reflection::ValueArray>)>, boost::function<void(std::string)>)> getScheduledNotificationsSignal;

        
        void scheduleNotification(int userId, int alertId, std::string alerMsg, int minutesToFire);
        void cancelNotification(int userId, int alertId);
        void cancelAllNotification(int userId);
        void getScheduledNotifications(int userId, boost::function<void(shared_ptr<const Reflection::ValueArray>)> resumeFunction, boost::function<void(std::string)>	errorFunction);
	};
}