#ifndef __BALL_H__
#define __BALL_H__

class Ball : Entity
{
public:
	Ball();
	~Ball();

	void Initialize();
	void PullShaderHandles();
	void RenderSetup(Matrix* pWVP);
	Vector3* GetPosition();
	Vector3* GetVelocity();
	Vector4* GetRotation();
	double mTimeElapsed;
	Vector3* mRotationRate;
	void SetPosition(Vector3* pPosition);
	void SetVelocity(Vector3* pVelocity);

	virtual void Update(double pElapsedTime);
	virtual void Draw();
};

#endif
