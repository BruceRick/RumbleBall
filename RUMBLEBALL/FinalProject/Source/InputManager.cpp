#include "CommonHeader.h"

InputManager::InputManager(PlayerKeys* pPlayerInput)
{
	mPlayerInput = pPlayerInput;
	
	//mThrottleEvent = NULL;
	//CheckInputFPointer = CheckInput;
}

InputManager::~InputManager()
{
	TerminateThread(mInputThread, 0);
}

void InputManager::Update()
{
	//TODO: change input so only using ONE thread intead of One for each key.
	if(glfwGetKey(mPlayerInput->Throttle))
	{
		SetEvent(mThrottleEvent);
	}

	if(glfwGetKey(mPlayerInput->RotateLeft))
	{
		SetEvent(mRotateLeftEvent);
	}

	if(glfwGetKey(mPlayerInput->RotateRight))
	{
		SetEvent(mRotateRightEvent);
	}
}

DWORD WINAPI CheckInput(LPVOID lpParam)
{
	while(true)
	{
		InputManager* input = (InputManager*)lpParam;

		int a = glfwGetKey(input->getPlayerInput()->Throttle);
		if(glfwGetKey(input->getPlayerInput()->Throttle))
		{
			SetEvent(input->getThrottleEvent());
		}

		if(glfwGetKey(input->getPlayerInput()->RotateLeft))
		{
			SetEvent(input->getRotateLeftEvent());
		}

		if(glfwGetKey(input->getPlayerInput()->RotateRight))
		{
			SetEvent(input->getRotateRightEvent());
		}

		if(!glfwGetKey(input->getPlayerInput()->Throttle) &&
		   !glfwGetKey(input->getPlayerInput()->RotateLeft) &&
		   !glfwGetKey(input->getPlayerInput()->RotateRight))
		{
			SetEvent(input->getNoKeyDownEvent());
		}
	}
}

void InputManager::Initialize()
{
	InputManager* input = this;

	mThrottleEvent = CreateEvent(NULL,FALSE,FALSE,NULL);
	mRotateLeftEvent = CreateEvent(NULL,FALSE,FALSE,NULL);
	mRotateRightEvent = CreateEvent(NULL,FALSE,FALSE,NULL);
	mNoKeyDownEvent = CreateEvent(NULL,FALSE,FALSE,NULL);
	//mInputThread = CreateThread(NULL, 0, CheckInput, this, 0, NULL);
}

HANDLE InputManager::getThrottleEvent()
{
	return mThrottleEvent;
}

HANDLE InputManager::getRotateLeftEvent()
{
	return mRotateLeftEvent;
}

HANDLE InputManager::getRotateRightEvent()
{
	return mRotateRightEvent;
}

PlayerKeys *InputManager::getPlayerInput()
{
	return	mPlayerInput;
}

HANDLE InputManager::getNoKeyDownEvent()
{
	return mNoKeyDownEvent;
}
