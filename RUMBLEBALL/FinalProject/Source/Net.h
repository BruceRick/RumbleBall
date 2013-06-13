#ifndef __NET_H__
#define __NET_H__

class Net : Entity
{
public:
	Net();
	~Net();

	void Initialize(Vector3 *aPosition, const char* aTexture);
	void PullShaderHandles();
	void RenderSetup(Matrix* pWVP);
	MeshObject* GetNetBack();
	MeshObject* GetNetFrame();
	MeshObject* GetNetSide1();
	MeshObject* GetNetSide2();


	void Update(double pElapsedTime);
	void Draw();

private:

	std::vector<MeshObject*> m_NetModels;
	MeshObject* mNetBack;
	MeshObject* mNetFrame;
	MeshObject* mNetSide1;
	MeshObject* mNetSide2;
	MeshObject* mNetTop;

	//MeshObject 


};

#endif