#include "CommonHeader.h"

ScreenManager* gScreenManager = 0;

ScreenManager::ScreenManager()
{
}

ScreenManager::~ScreenManager()
{
}

void ScreenManager::Initialize()
{
}

void ScreenManager::AddScreen(Screen* pScreen)
{
	mActiveScreens.push_back(pScreen);
}

void ScreenManager::Update(double pElapsedTime)
{
	for(int i = 0; i < mActiveScreens.size(); i++)
	{
		mActiveScreens.at(i)->Update(pElapsedTime);
	}
}

void ScreenManager::Draw()
{
	for(int i = 0; i < mActiveScreens.size(); i++)
	{
		mActiveScreens.at(i)->Draw();
	}
}
