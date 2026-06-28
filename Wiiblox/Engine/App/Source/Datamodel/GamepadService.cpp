//
//  GamepadService.cpp
//  App
//
//  Created by Ben Tkacheff on 1/21/15.
//
//


#include "stdafx.h"

#include "V8DataModel/GamepadService.h"
#include "V8DataModel/UserInputService.h"
#include "v8datamodel/GuiService.h"
#include "v8datamodel/GuiObject.h"
#include "v8datamodel/PlayerGui.h"
#include "v8datamodel/Workspace.h"
#include "v8datamodel/TextBox.h"
#include "v8datamodel/ScrollingFrame.h"
#include "v8datamodel/ContextActionService.h"

#define VIRTUAL_CURSOR_DEADZONE 0.14f
#define VIRTUAL_CURSOR_ACCELERATION_TIME 200.0f

#define GUI_SELECTION_INITIAL_REPEAT_TIME 500.0f
#define GUI_SELECTION_REPEAT_TIME 120.0f

namespace WBX
{
    const char* const sGamepadService = "GamepadService";
    
    
    GamepadService::GamepadService() :
		lastGuiSelectionDirection(0,0),
		autoGuiSelectionAllowed(true)
    {
        setName(sGamepadService);
    }
    
    int GamepadService::getGamepadIntForEnum(InputObject::UserInputType gamepadEnum)
	{
		switch (gamepadEnum)
		{
			case InputObject::TYPE_GAMEPAD1:	return 0;
			case InputObject::TYPE_GAMEPAD2:	return 1;
			case InputObject::TYPE_GAMEPAD3:	return 2;
			case InputObject::TYPE_GAMEPAD4:	return 3;
			case InputObject::TYPE_GAMEPAD5:	return 4;
			case InputObject::TYPE_GAMEPAD6:	return 5;
			case InputObject::TYPE_GAMEPAD7:	return 6;
			case InputObject::TYPE_GAMEPAD8:	return 7;
			default:							return -1;
		}
	}

    InputObject::UserInputType GamepadService::getGamepadEnumForInt(int controllerIndex)
    {
        switch (controllerIndex)
        {
            case 0:     return InputObject::TYPE_GAMEPAD1;
            case 1:     return InputObject::TYPE_GAMEPAD2;
            case 2:     return InputObject::TYPE_GAMEPAD3;
            case 3:     return InputObject::TYPE_GAMEPAD4;
			case 4:     return InputObject::TYPE_GAMEPAD5;
			case 5:     return InputObject::TYPE_GAMEPAD6;
			case 6:     return InputObject::TYPE_GAMEPAD7;
			case 7:     return InputObject::TYPE_GAMEPAD8;
            default:    return InputObject::TYPE_NONE;
        }
    }
    
    shared_ptr<wbx::InputObject> GamepadService::createInputObjectForGamepadKeyCode(wbx::KeyCode keyCode, wbx::InputObject::UserInputType gamepadType)
	{
		return Creatable<Instance>::create<InputObject>(gamepadType,
														wbx::InputObject::INPUT_STATE_END,
	  													G3D::Vector3::zero(),
														keyCode,
														wbx::DataModel::get(this));
    }
    
    void GamepadService::createControllerKeyMapForController(int controllerIndex)
    {
        wbx::InputObject::UserInputType gamepadType = getGamepadEnumForInt(controllerIndex);

        Gamepad controllerKeyMap;
        controllerKeyMap[wbx::SDLK_GAMEPAD_BUTTONA] = createInputObjectForGamepadKeyCode(wbx::SDLK_GAMEPAD_BUTTONA, gamepadType);
        controllerKeyMap[wbx::SDLK_GAMEPAD_BUTTONB] = createInputObjectForGamepadKeyCode(wbx::SDLK_GAMEPAD_BUTTONB, gamepadType);
        controllerKeyMap[wbx::SDLK_GAMEPAD_BUTTONX] = createInputObjectForGamepadKeyCode(wbx::SDLK_GAMEPAD_BUTTONX, gamepadType);
        controllerKeyMap[wbx::SDLK_GAMEPAD_BUTTONY] = createInputObjectForGamepadKeyCode(wbx::SDLK_GAMEPAD_BUTTONY, gamepadType);
        controllerKeyMap[wbx::SDLK_GAMEPAD_BUTTONSTART] = createInputObjectForGamepadKeyCode(wbx::SDLK_GAMEPAD_BUTTONSTART, gamepadType);
        controllerKeyMap[wbx::SDLK_GAMEPAD_BUTTONSELECT] = createInputObjectForGamepadKeyCode(wbx::SDLK_GAMEPAD_BUTTONSELECT, gamepadType);
        
        controllerKeyMap[wbx::SDLK_GAMEPAD_BUTTONL1] = createInputObjectForGamepadKeyCode(wbx::SDLK_GAMEPAD_BUTTONL1, gamepadType);
        controllerKeyMap[wbx::SDLK_GAMEPAD_BUTTONR1] = createInputObjectForGamepadKeyCode(wbx::SDLK_GAMEPAD_BUTTONR1, gamepadType);
        
        controllerKeyMap[wbx::SDLK_GAMEPAD_BUTTONL2] = createInputObjectForGamepadKeyCode(wbx::SDLK_GAMEPAD_BUTTONL2, gamepadType);
        controllerKeyMap[wbx::SDLK_GAMEPAD_BUTTONR2] = createInputObjectForGamepadKeyCode(wbx::SDLK_GAMEPAD_BUTTONR2, gamepadType);
        
        controllerKeyMap[wbx::SDLK_GAMEPAD_BUTTONL3] = createInputObjectForGamepadKeyCode(wbx::SDLK_GAMEPAD_BUTTONL3, gamepadType);
        controllerKeyMap[wbx::SDLK_GAMEPAD_BUTTONR3] = createInputObjectForGamepadKeyCode(wbx::SDLK_GAMEPAD_BUTTONR3, gamepadType);
        
        controllerKeyMap[wbx::SDLK_GAMEPAD_DPADUP] = createInputObjectForGamepadKeyCode(wbx::SDLK_GAMEPAD_DPADUP, gamepadType);
        controllerKeyMap[wbx::SDLK_GAMEPAD_DPADDOWN] = createInputObjectForGamepadKeyCode(wbx::SDLK_GAMEPAD_DPADDOWN, gamepadType);
        controllerKeyMap[wbx::SDLK_GAMEPAD_DPADLEFT] = createInputObjectForGamepadKeyCode(wbx::SDLK_GAMEPAD_DPADLEFT, gamepadType);
        controllerKeyMap[wbx::SDLK_GAMEPAD_DPADRIGHT] = createInputObjectForGamepadKeyCode(wbx::SDLK_GAMEPAD_DPADRIGHT, gamepadType);
        
        controllerKeyMap[wbx::SDLK_GAMEPAD_THUMBSTICK1] = createInputObjectForGamepadKeyCode(wbx::SDLK_GAMEPAD_THUMBSTICK1, gamepadType);
        controllerKeyMap[wbx::SDLK_GAMEPAD_THUMBSTICK2] = createInputObjectForGamepadKeyCode(wbx::SDLK_GAMEPAD_THUMBSTICK2, gamepadType);
        
        gamepads[controllerIndex] = controllerKeyMap;
    }
    
    void GamepadService::onServiceProvider(ServiceProvider* oldProvider, ServiceProvider* newProvider)
    {
		if (oldProvider)
		{
			inputChangedConnection.disconnect();
			inputEndedConnection.disconnect();
			updateInputConnection.disconnect();
			cameraCframeUpdateConnection.disconnect();
		}

        Super::onServiceProvider(oldProvider, newProvider);
        
        if (newProvider)
        {
            for (int i = 0; i < WBX_MAX_GAMEPADS; ++i)
            {
                createControllerKeyMapForController(i);
				gamepadNavigationEnabledMap[getGamepadEnumForInt(i)] = true;
            }

			if (UserInputService* inputService = wbx::ServiceProvider::find<UserInputService>(newProvider))
			{
				updateInputConnection = inputService->updateInputSignal.connect(boost::bind(&GamepadService::updateOnInputStep, this));
				inputEndedConnection = inputService->coreInputEndedEvent.connect(boost::bind(&GamepadService::onInputEnded, this, _1));
				inputChangedConnection = inputService->coreInputUpdatedEvent.connect(boost::bind(&GamepadService::onInputChanged, this, _1));
			}

			if (wbx::Workspace* workspace = wbx::ServiceProvider::find<Workspace>(this))
			{
				cameraCframeUpdateConnection = workspace->getCamera()->cframeChangedSignal.connect(boost::bind(&GamepadService::cameraCframeChanged, this, _1));
				workspace->currentCameraChangedSignal.connect(boost::bind(&GamepadService::currentCameraChanged, this, _1));
			}
        }
    }

	void GamepadService::currentCameraChanged(shared_ptr<Camera> newCurrentCamera)
	{
		if (newCurrentCamera)
		{
			cameraCframeUpdateConnection.disconnect();
			cameraCframeUpdateConnection = newCurrentCamera->cframeChangedSignal.connect(boost::bind(&GamepadService::cameraCframeChanged, this, _1));
		}
	}

	//todo: figure out a better way to set arrow cursor for gamepad
	void GamepadService::cameraCframeChanged(CoordinateFrame cframe)
	{
		if (UserInputService* inputService = wbx::ServiceProvider::find<UserInputService>(this))
		{
			InputObject::UserInputType lastType = inputService->getLastInputType();
			if ( getGamepadIntForEnum(lastType) != -1)
			{
				if (UserInputService* inputService = ServiceProvider::find<UserInputService>(this))
				{
					if (DataModel* dm = DataModel::get(this))
					{
						dm->setForceArrowCursor(inputService->isMouseOverGui());
					}
				}
			}
		}
	}

	void GamepadService::updateOnInputStep()
	{
		if (lastGuiSelectionDirection == Vector2::zero())
		{
			return;
		}

		if (repeatGuiSelectionTimer.delta().msec() >= GUI_SELECTION_INITIAL_REPEAT_TIME)
		{
			if (fastRepeatGuiSelectionTimer.delta().msec() >= GUI_SELECTION_REPEAT_TIME)
			{
				fastRepeatGuiSelectionTimer.reset();
				trySelectGuiObject(lastGuiSelectionDirection);
			}
		}
	}

	void GamepadService::onInputEnded(const shared_ptr<Instance>& event)
	{
		if (InputObject* inputObject = Instance::fastDynamicCast<InputObject>(event.get()))
		{
			getGuiSelectionDirection(shared_from(inputObject));
		}
	}
    
    Gamepad GamepadService::getGamepadState(int controllerIndex)
    {
        return gamepads[controllerIndex];
    }

	bool GamepadService::setAutoGuiSelectionAllowed(bool value)
	{
		if (value != autoGuiSelectionAllowed)
		{
			autoGuiSelectionAllowed = value;
			return true;
		}

		return false;
	}

	float getUserIntentThumbstick(float pos)
	{
		if (fabs(pos) < 0.6)
		{
			return 0.0f;
		}
		else if (pos < -0.6f)
		{
			return -1.0f;
		}
		else if (pos > 0.6f)
		{
			return 1.0f;
		}

		return 0.0f;
	}

	Vector2 getCorrectedThumbstickDirection(const shared_ptr<InputObject>& event)
	{
		Vector2 correctGuiDirection = event->get2DPosition();
		correctGuiDirection.x = getUserIntentThumbstick(correctGuiDirection.x);
		correctGuiDirection.y = -getUserIntentThumbstick(correctGuiDirection.y);
		return correctGuiDirection;
	}

	bool GamepadService::isVectorInDeadzone(const Vector2& correctGuiDirection) const
	{
		return fabs(correctGuiDirection.x) < VIRTUAL_CURSOR_DEADZONE && fabs(correctGuiDirection.y) < VIRTUAL_CURSOR_DEADZONE;
	}

	void GamepadService::onInputChanged(const shared_ptr<Instance>& event)
	{
		if (InputObject* inputObject = Instance::fastDynamicCast<InputObject>(event.get()))
		{
			const shared_ptr<InputObject> event = shared_from(inputObject);
			if (event->getKeyCode() == wbx::SDLK_GAMEPAD_THUMBSTICK1)
			{
				Vector2 correctGuiDirection = getCorrectedThumbstickDirection(event);
				if ( isVectorInDeadzone(correctGuiDirection))
				{
					getGuiSelectionDirection(event);
				}
			}
		}
	}

	Vector2 GamepadService::getGuiSelectionDirection(const shared_ptr<InputObject>& event)
	{
		Vector2 inputVector = Vector2::zero();
		if (event->getKeyCode() == wbx::SDLK_GAMEPAD_THUMBSTICK1)
		{
			Vector2 correctGuiDirection = getCorrectedThumbstickDirection(event);

			if ( isVectorInDeadzone(lastGuiSelectionDirection))
			{
				inputVector = correctGuiDirection;
			}

			if (lastGuiSelectionDirection != correctGuiDirection)
			{
				lastGuiSelectionDirection = correctGuiDirection;
				repeatGuiSelectionTimer.reset();
			}
		}
		else if(event->isDPadEvent())
		{
			if (event->getUserInputState() == InputObject::INPUT_STATE_BEGIN)
			{
				switch (event->getKeyCode())
				{
				case SDLK_GAMEPAD_DPADUP:
					inputVector = Vector2(0,-1);
					break;
				case SDLK_GAMEPAD_DPADDOWN:
					inputVector = Vector2(0,1);
					break;
				case SDLK_GAMEPAD_DPADLEFT:
					inputVector = Vector2(-1,0);
					break;
				case SDLK_GAMEPAD_DPADRIGHT:
					inputVector = Vector2(1,0);
					break;
				default:
					break;
				}
			}

			if (inputVector != lastGuiSelectionDirection)
			{
				lastGuiSelectionDirection = inputVector;
				repeatGuiSelectionTimer.reset();
			}
		}


		return inputVector;
	}

	GuiResponse GamepadService::trySelectGuiObject(const Vector2& inputVector)
	{
		CoreGuiService* coreGuiService = wbx::ServiceProvider::find<CoreGuiService>(this);
		GuiResponse response = trySelectGuiObject(inputVector, shared_ptr<InputObject>(), coreGuiService);

		if (!response.wasSunk())
		{
			if (wbx::Network::Players* players = ServiceProvider::find<wbx::Network::Players>(this))
			{
				if (wbx::Network::Player* player = players->getLocalPlayer())
				{
					if (PlayerGui* playerGui = player->findFirstChildOfType<PlayerGui>())
					{
						response = trySelectGuiObject(inputVector, shared_ptr<InputObject>(), playerGui);
					}
				}
			}
		}

		return response;
	}

	GuiResponse GamepadService::trySelectGuiObject(const Vector2& inputVector, const shared_ptr<InputObject>& event, BasePlayerGui* guiToProcess)
	{
		GuiService* guiService = wbx::ServiceProvider::find<GuiService>(this);
		if (!guiService)
		{
			return GuiResponse::notSunk();
		}
        
        GuiObject* selectedObject = NULL;
        if (GuiObject* selectedGuiObject = guiService->getSelectedGuiObject())
        {
            selectedObject = selectedGuiObject;
        }
        
		bool isCoreGuiProcessing = false;
		if (Instance::fastDynamicCast<CoreGuiService>(guiToProcess))
		{
			isCoreGuiProcessing = true;
		}
        
        if (selectedObject && !selectedObject->isCurrentlyVisible() && !(selectedObject->findFirstAncestorOfType<ScrollingFrame>()))
        {
            if (selectedObject->isDescendantOf(guiToProcess))
            {
				if (isCoreGuiProcessing)
				{
					guiService->setSelectedCoreGuiObjectLua(NULL);
				}
				else
				{
					guiService->setSelectedGuiObjectLua(NULL);
				}
            }

			selectedObject = NULL;
        }
        
        if (!selectedObject)
        {
            return GuiResponse::notSunk();
        }

		// don't process core gui selection if we turned off core gui nav (used for radial menus)
		if (isCoreGuiProcessing && !guiService->getCoreGamepadNavEnabled() && selectedObject->isDescendantOf(guiToProcess))
		{
			return GuiResponse::notSunk();
		}

		if ( (!guiService->getGamepadNavEnabled() && !isCoreGuiProcessing) || !selectedObject->isDescendantOf(guiToProcess) )
		{
			return GuiResponse::notSunk();
		}

		if (inputVector == Vector2::zero())
		{
            if ( event && (isNavigationGamepad(event->getUserInputType()) || isCoreGuiProcessing) &&
                (event->getKeyCode() == wbx::SDLK_GAMEPAD_THUMBSTICK1 || event->isDPadEvent()) )
            {
                return GuiResponse::sunk();
            }
			else if( event && event->getUserInputType() == InputObject::TYPE_KEYBOARD && event->isNavigationKey())
			{
				return GuiResponse::sunk();
			}

			if ( event && (event->getKeyCode() == wbx::SDLK_GAMEPAD_THUMBSTICK1 || event->isDPadEvent()) )
			{
				lastGuiSelectionDirection = Vector2::zero();
			}

            return GuiResponse::notSunk();
		}

		CoreGuiService* coreGuiService = wbx::ServiceProvider::find<CoreGuiService>(this);
		if (isCoreGuiProcessing && selectedObject->isDescendantOf(coreGuiService))
		{
            coreGuiService->selectNewGuiObject(inputVector);
		}
		else
		{
			if (wbx::Network::Players* players = ServiceProvider::create<wbx::Network::Players>(this))
			{
				if (wbx::Network::Player* player = players->getLocalPlayer())
				{
					if (PlayerGui* playerGui = player->findFirstChildOfType<PlayerGui>())
                    {
						if (selectedObject->isDescendantOf(playerGui))
						{
                            playerGui->selectNewGuiObject(inputVector);
						}
					}
				}
			}
		}

		return GuiResponse::sunk();
	}

	GuiObject* GamepadService::getRandomShownGuiObject(Instance* object)
	{
		GuiObject* guiObject = NULL;
		if (GuiButton* guiButton = Instance::fastDynamicCast<GuiButton>(object))
		{
			guiObject = guiButton;
		}
        else if(TextBox* textBox = Instance::fastDynamicCast<TextBox>(object))
        {
            guiObject = textBox;
        }

		if (guiObject && guiObject->isCurrentlyVisible() && guiObject->getSelectable())
		{
			return guiObject;
		}

		if (object->numChildren() > 0)
		{
			if (shared_ptr<const Instances> children = object->getChildren2())
			{
				Instances::const_iterator end = children->end();
				for (Instances::const_iterator iter = children->begin(); iter != end; ++iter)
				{
					if ( GuiObject* childObject = getRandomShownGuiObject((*iter).get()) )
					{
						if (childObject->isCurrentlyVisible() && childObject->getSelectable())
						{
							return childObject;
						}
					}
				}
			}
		}

		return NULL;
	}

	GuiResponse GamepadService::autoSelectGui()
	{
		GuiService* guiService = wbx::ServiceProvider::find<GuiService>(this);
		if (!guiService)
		{
			return GuiResponse::notSunk();
		}

		if (GuiObject* selectedGuiObject = guiService->getSelectedGuiObject())
		{
            CoreGuiService* coreGuiService = wbx::ServiceProvider::find<CoreGuiService>(this);
			if (selectedGuiObject->isDescendantOf(coreGuiService))
			{
				return GuiResponse::notSunk();
			}

            guiService->setSelectedGuiObjectLua(NULL);
			return GuiResponse::sunk();
		}
		
		if (wbx::Network::Players* players = ServiceProvider::create<wbx::Network::Players>(this))
		{
			if (wbx::Network::Player* player = players->getLocalPlayer())
			{
				if (PlayerGui* playerGui = player->findFirstChildOfType<PlayerGui>())
				{
					if (GuiObject* guiObject = getRandomShownGuiObject(playerGui))
					{
						guiService->setSelectedGuiObjectLua(guiObject);
						return GuiResponse::sunk();
					}
				}
			}
		}
        
        return GuiResponse::notSunk();
	}

	GuiResponse GamepadService::processCore(const shared_ptr<InputObject>& event)
	{
		UserInputService* userInputService = wbx::ServiceProvider::find<UserInputService>(this);
		if (!userInputService)
		{
			return GuiResponse::notSunk();
		}

		return process(event, wbx::ServiceProvider::find<CoreGuiService>(this));
	}

	GuiResponse GamepadService::processDev(const shared_ptr<InputObject>& event)
	{
		if (autoGuiSelectionAllowed && event->getKeyCode() == wbx::SDLK_GAMEPAD_BUTTONSELECT && event->getUserInputState() == InputObject::INPUT_STATE_BEGIN)
		{
			return autoSelectGui();
		}

		if (wbx::Network::Players* players = ServiceProvider::create<wbx::Network::Players>(this))
		{
			if (wbx::Network::Player* player = players->getLocalPlayer())
			{
				if (PlayerGui* playerGui = player->findFirstChildOfType<PlayerGui>())
				{
					return process(event, playerGui);
				}
			}
		}

		return GuiResponse::notSunk();
	}

	GuiResponse GamepadService::process(const shared_ptr<InputObject>& event, BasePlayerGui* guiToProcess)
	{
		if ( !Instance::fastDynamicCast<CoreGuiService>(guiToProcess) && !isNavigationGamepad(event->getUserInputType()) )
		{
			return GuiResponse::notSunk();
		}

		if (GuiService* guiService = wbx::ServiceProvider::find<GuiService>(this))
		{
			if (wbx::GuiObject* guiObject = guiService->getSelectedGuiObject())
			{
				if (!guiObject->isDescendantOf(guiToProcess))
				{
					return GuiResponse::notSunk();
				}
			}
		}

		return trySelectGuiObject(getGuiSelectionDirection(event), event, guiToProcess);
	}

	void GamepadService::setNavigationGamepad(InputObject::UserInputType gamepadType, bool enabled)
	{
		if ( getGamepadIntForEnum(gamepadType) == -1 )
			return;

		gamepadNavigationEnabledMap[gamepadType] = enabled;
	}

	bool GamepadService::isNavigationGamepad(InputObject::UserInputType gamepadType)
	{
		if (gamepadNavigationEnabledMap.find(gamepadType) == gamepadNavigationEnabledMap.end())
		{
			return false;
		}

		return gamepadNavigationEnabledMap[gamepadType];
	}

    
} // namespace WBX

