//
//  TouchInputService.h
//  App
//
//  Copyright 2015 Wiiblox Corporation, All Rights Reserved
//  Created by Ben Tkacheff on 7/10/15.
//
//
#pragma once

#include <boost/unordered_map.hpp>

#include "V8Tree/Service.h"
#include "V8DataModel/InputObject.h"

namespace WBX
{
    extern const char *const sTouchInputService;
    
    typedef std::pair<wbx::Vector3, wbx::InputObject::UserInputState> TouchInfo;
    typedef boost::unordered_map<int, std::vector<TouchInfo> > TouchBufferMap;
    
    class TouchInputService
        : public DescribedNonCreatable<TouchInputService, Instance, sTouchInputService>
        , public Service
    {
    private:
        typedef DescribedNonCreatable<TouchInputService, Instance, sTouchInputService> Super;
        
        boost::mutex touchBufferMutex;
        int touchCount;
        TouchBufferMap touchBufferMap;
        boost::unordered_map<int, void*> countToTouchMap;
        boost::unordered_map<void*, int> touchToCountMap;
        boost::unordered_map<int, shared_ptr<InputObject> > touchIdToInputObjectMap;
        
        wbx::signals::scoped_connection updateInputConnection;
        
        void processTouchBuffer();
    protected:
        /*override*/ void onServiceProvider(ServiceProvider* oldSp, ServiceProvider* newSp);
    public:
        TouchInputService();
        
        void addTouchToBuffer(void* touch, Vector3 WBXLocation, InputObject::UserInputState newState);
    };
}