//
//  LoginService.h
//  App
//
//  Created by Ben Tkacheff on 5/1/13.
//
//
#include "V8Tree/Instance.h"
#include "V8Tree/Service.h"

namespace WBX
{
    
	extern const char* const sLoginService;
	class LoginService
    : public DescribedNonCreatable<LoginService, Instance, sLoginService>
    , public Service
    
	{
	public:
		LoginService();
        
        wbx::signal<void(std::string)> loginSucceededSignal;
        wbx::signal<void(std::string)> loginFailedSignal;
        
        wbx::signal<void()> promptLoginSignal;
        wbx::signal<void()> promptLogoutSignal;
        
        void promptSignup();
        void promptLogin();
        void logout();
	};
    
}