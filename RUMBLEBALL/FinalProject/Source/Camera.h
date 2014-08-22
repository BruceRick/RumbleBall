#ifndef CAMERA_H
#define CAMERA_H

class Camera;

extern Camera* gCamera;

class Camera
{
public:
	Camera();
	~Camera();
	void Initialize();
	Vector3* Position();
	void SetPosition(Vector3* pPosition);

	Matrix* GetPerspective();
	Matrix* GetOrth();
	Matrix* GetViewProjection();

	void SetPerspective(Matrix* pPerspective);
	void SetOrtho(Matrix* pOrtho);
	void SetViewProjection(Matrix* pViewProj);


private:

	Vector3* m_CameraPos;
	Matrix* m_matPerspective;
	Matrix* m_matOrth;
	Matrix* m_matViewProj;
};

#endif
