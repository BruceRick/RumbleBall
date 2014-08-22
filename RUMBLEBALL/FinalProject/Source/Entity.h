#ifndef __ENTITY_H__
#define __ENTITY_H__

class Entity
{
public:
	Entity();
	~Entity();

	virtual void Initialize(MeshObject* pModel, GLuint pTextureID, Shader* pShader);
	
	virtual void Update(double pElapsedTime);
	virtual void Draw();
	void PullShaderHandles();
	void RenderSetup(Matrix* pWVP);
	MeshObject* GetModel();
	Vector3 *mVelocity;
	Vector3 *mPosition;

protected:
	Material* mMaterial;
	MeshObject* mModel;
	Shader* mShader;
	GLuint mTextureID;
	Vector4 *mRotation;
};

#endif
