#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

class InputManager
{
public:
	InputManager(PlayerKeys* pPlayerInput);
	~InputManager();
	void Initialize();
	void Update();
	HANDLE getThrottleEvent();
	HANDLE getRotateLeftEvent();
	HANDLE getRotateRightEvent();
	HANDLE getNoKeyDownEvent();
	PlayerKeys* getPlayerInput();

private:
	
	HANDLE mThrottleEvent;
	HANDLE mRotateLeftEvent;
	HANDLE mRotateRightEvent;
	HANDLE mNoKeyDownEvent;
	HANDLE mInputThread;
	PlayerKeys* mPlayerInput;

};

#endif

