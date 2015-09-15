#pragma once


#include "d3dUtil.h"

class Player
{
public:
	Player(ID3D11Device* device);
	~Player();

	// Get/Set world camera position.
	XMVECTOR GetPositionXM()const;
	XMFLOAT3 GetPosition()const;
	void SetPosition(float x, float y, float z);
	void SetPosition(const XMFLOAT3& v);

	// Get Player basis vectors.
	XMVECTOR GetRightXM()const;
	XMFLOAT3 GetRight()const;
	XMVECTOR GetUpXM()const;
	XMFLOAT3 GetUp()const;
	XMVECTOR GetLookXM()const;
	XMFLOAT3 GetLook()const;






	// Get View/Proj matrices.
	XMMATRIX View()const;
	XMMATRIX Proj()const;
	XMMATRIX ViewProj()const;


	// Strafe/Walk the camera a distance d.
	void Strafe(float d);
	void Walk(float d);
	void Jump(float d);

	void UpdateViewMatrix();


	//not sure if make a pointer will work but we shall see
	ID3D11Device* pdevice;

//	void drawPlayer(ID3D11DeviceContext* dc, const Camera& camera);





private:

	void buildBuffers();


	//Player coordinate system with coords relative to world space
	XMFLOAT3 mPosition;
	XMFLOAT3 mRight;
	XMFLOAT3 mUp;
	XMFLOAT3 mLook;


	




	//buffers
	ID3D11Buffer* mPlayerVB;
	ID3D11Buffer* mPlayerIB;

	int mPlayerVertexOffset;
	UINT mPlayerIndexOffset;
	UINT mPlayerIndexCount;

	//textures
	ID3D11ShaderResourceView* mDiffuseMapSRV;
	Material mPlayerMat;



	XMFLOAT3 mEyePosW;

	XMFLOAT4X4 mTexTransform;
	XMFLOAT4X4 mPlayerWorld;




	// Cache View/Proj matrices.
	XMFLOAT4X4 mView;
	XMFLOAT4X4 mProj;



};

