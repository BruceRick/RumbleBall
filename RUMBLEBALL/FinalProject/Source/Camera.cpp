#include "CommonHeader.h"

Camera* gCamera = 0;

Camera::Camera()
{
	gCamera = this;
	m_CameraPos = new Vector3(0.0f,0.0f,0.0f);
	m_matOrth = new Matrix();
	m_matPerspective = new Matrix();
	m_matViewProj = new Matrix();
	//m_CameraPos->Set(0.0f , 10.0f, 15.0f);
	//float ratio = (float)gWindow->width()/gWindow->height();
	//m_matPerspective->SetPerspective( 90, ratio, 1.0f, 100000);

}

Camera::~Camera()
{

}


void Camera::Initialize()
{


}


Vector3* Camera::Position()
{
	return m_CameraPos;
}

void Camera::SetPosition(Vector3* pPosition)
{
	m_CameraPos = pPosition;
}

Matrix* Camera::GetPerspective()
{
	return m_matPerspective;
}

Matrix* Camera::GetOrth()
{
	return m_matOrth;
}

Matrix* Camera::GetViewProjection()
{
	return m_matViewProj;
}

void Camera::SetPerspective(Matrix* pPerspective)
{
	m_matPerspective = pPerspective;
}

void Camera::SetOrtho(Matrix* pOrtho)
{
	m_matOrth = pOrtho;
}

void Camera::SetViewProjection(Matrix* pViewProjection)
{
	m_matViewProj = pViewProjection;
}