#ifndef __Player_H__
#define __Player_H__

class Player : Entity
{
public:
	Player(PlayerKeys* pPlayerKeys, const char* pCarTexture, Vector3* pPosition);
	~Player();

	void Initialize();
	void PullShaderHandles();
	void RenderSetup(Matrix* pWVP);
	InputManager* GetInputState();
	Vector3* GetPosition();
	Vector3* GetVelocity();
	Vector4* GetRotation();
	double mTimeElapsed;
	float mRotationRate;
	void SetPosition(Vector3* pPosition);
	void SetVelocity(Vector3* pVelocity);

	virtual void Update(double pElapsedTime);
	virtual void Draw();

private:
	InputManager* mInputState;
	HANDLE mRotateLeftThread;
	HANDLE mRotateRightThread;
	HANDLE mThrottleThread;
	const char* mCarTexture;
};

#endif
