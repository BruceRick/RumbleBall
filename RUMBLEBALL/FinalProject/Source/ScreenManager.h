#ifndef __SCREEN_MANAGER_H__
#define __SCREEN_MANAGER_H__

class ScreenManager;
class Screen;

extern ScreenManager* gScreenManager;

class ScreenManager
{
public:
	ScreenManager();
	~ScreenManager();

	void Initialize();
	void AddScreen(Screen* pScreen);
	void Update(double pElapsedTime);
	void Draw();

private:
	std::vector<Screen*> mActiveScreens;
};

#endif
