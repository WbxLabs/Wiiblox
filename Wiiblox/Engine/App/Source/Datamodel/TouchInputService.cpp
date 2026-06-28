//
//  TouchInputService.cpp
//  App
//
//  Created by Ben Tkacheff on 7/10/15.
//
//
#include "stdafx.h"

#include "v8datamodel/TouchInputService.h"
#include "v8datamodel/UserInputService.h"

namespace WBX
{

const char *const sTouchInputService = "TouchInputService";

TouchInputService::TouchInputService() :
    touchCount(0)
{
    setName(sTouchInputService);
}
    
void TouchInputService::onServiceProvider(ServiceProvider* oldSp, ServiceProvider* newSp)
{
    if (oldSp)
    {
        updateInputConnection.disconnect();
    }
    
    Super::onServiceProvider(oldSp, newSp);
    
    if (newSp)
    {
        if (UserInputService* inputService = wbx::ServiceProvider::find<UserInputService>(newSp))
        {
            updateInputConnection = inputService->updateInputSignal.connect(boost::bind(&TouchInputService::processTouchBuffer, this));
        }
    }
}
    
void TouchInputService::addTouchToBuffer(void* touch, Vector3 WBXLocation, InputObject::UserInputState newState)
{
    boost::mutex::scoped_lock mutex(touchBufferMutex);

    if (newState == wbx::InputObject::INPUT_STATE_BEGIN)
    {
        touchCount++;
        int newTouchCount = touchCount;
        touchBufferMap[newTouchCount].push_back(TouchInfo(WBXLocation, newState));
        touchToCountMap[touch] = newTouchCount;
        countToTouchMap[newTouchCount] = touch;
    }
    else
    {
        if (touchToCountMap.find(touch) == touchToCountMap.end())
        {
            // we have an unaccounted for touch wtf
            WBXASSERT(false);
            return;
        }
        
        touchBufferMap[touchToCountMap[touch]].push_back(TouchInfo(WBXLocation, newState));
        
        if (newState == wbx::InputObject::INPUT_STATE_END)
        {
            touchToCountMap.erase(touch);
        }
    }
}
    
void TouchInputService::processTouchBuffer()
{
    UserInputService* userInputService = wbx::ServiceProvider::find<UserInputService>(this);
    if (!userInputService)
        return;
    
    // grab our current input
    // need a mutex in case underlying platform is simultaneously trying to update map
    TouchBufferMap tmpTouchBuffer;
    {
        boost::mutex::scoped_lock mutex(touchBufferMutex);
        touchBufferMap.swap(tmpTouchBuffer);
    }
    
    for (TouchBufferMap::iterator iter = tmpTouchBuffer.begin(); iter != tmpTouchBuffer.end(); ++iter)
    {
        std::vector<TouchInfo> touches = (*iter).second;
        const int touchId = (*iter).first;
        
        shared_ptr<InputObject> currentInputObj = shared_ptr<InputObject>();
        if (touchIdToInputObjectMap.find(touchId) == touchIdToInputObjectMap.end())
        {
            currentInputObj = wbx::Creatable<wbx::Instance>::create<wbx::InputObject>(wbx::InputObject::TYPE_TOUCH, wbx::InputObject::INPUT_STATE_NONE, wbx::Vector3(0,0,0), wbx::Vector3(0,0,0), wbx::DataModel::get(this));
            touchIdToInputObjectMap[touchId] = currentInputObj;
        }
        else
        {
            currentInputObj = touchIdToInputObjectMap[touchId];
        }
        
        for (std::vector<TouchInfo>::iterator touchIter = touches.begin(); touchIter != touches.end(); ++touchIter)
        {
            wbx::InputObject::UserInputState inputState = (*touchIter).second;
            currentInputObj->setInputState(inputState);
            Vector3 newPos = (*touchIter).first;
            Vector3 newDelta = newPos - currentInputObj->getRawPosition();
            
            if (inputState == wbx::InputObject::INPUT_STATE_CHANGE && newDelta == Vector3::zero())
            {
                // this is an unnecessary touch, do nothing with it
            }
            else
            {
                if (inputState != wbx::InputObject::INPUT_STATE_BEGIN)
                {
                    currentInputObj->setDelta(newDelta);
                }
                currentInputObj->setPosition(newPos);
                
                userInputService->dangerousFireInputEvent(currentInputObj, countToTouchMap[touchId]);
                
                if (currentInputObj->getUserInputState() == wbx::InputObject::INPUT_STATE_END ||
                    currentInputObj->getUserInputState() == wbx::InputObject::INPUT_STATE_CANCEL)
                {
                    // this touch is done now, clean up references to it
                    boost::mutex::scoped_lock mutex(touchBufferMutex);
                    touchIdToInputObjectMap.erase(touchId);
                    countToTouchMap.erase(touchId);
                }
            }
        };
    }
}
    
    
    

} // namespace WBX