#ifndef __SCREEN_H__
#define __SCREEN_H__

//Abstract Template Class

class Screen
{
public:
	virtual void Update(double pElapsedTime) = 0;
	virtual void Draw() = 0;
};

#endif
