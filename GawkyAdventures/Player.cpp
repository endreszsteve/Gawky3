
#include "Player.h"
#include "LightHelper.h"
#include "Effects.h"
#include "Vertex.h"
#include "GeometryGenerator.h"
#include "Camera.h"


Player::Player(ID3D11Device* device)
{
	XMMATRIX I = XMMatrixIdentity();
	XMStoreFloat4x4(&mPlayerWorld, I);
	XMStoreFloat4x4(&mTexTransform, I);
	XMStoreFloat4x4(&mView, I);
	XMStoreFloat4x4(&mProj, I);



	XMMATRIX PlayerScale = XMMatrixScaling(3.0f, 1.0f, 3.0f);
	XMMATRIX PlayerOffset = XMMatrixTranslation(0.0f, 5.0f, 0.0f);
	XMStoreFloat4x4(&mPlayerWorld, XMMatrixMultiply(PlayerScale, PlayerOffset));

	mPlayerMat.Ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	mPlayerMat.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	mPlayerMat.Specular = XMFLOAT4(0.6f, 0.6f, 0.6f, 16.0f);


	HR(D3DX11CreateShaderResourceViewFromFile(device,
		L"Textures/WoodCrate02.dds", 0, 0, &mDiffuseMapSRV, 0));

	pdevice = device;

	buildBuffers();


}


Player::~Player()
{

	ReleaseCOM(mPlayerVB);
	ReleaseCOM(mPlayerIB);
	ReleaseCOM(mDiffuseMapSRV);


	Effects::DestroyAll();
	InputLayouts::DestroyAll();

}


void Player::buildBuffers()
{

	GeometryGenerator::MeshData thePlayer;

	GeometryGenerator geoGen;
	geoGen.CreateBox(1.0f, 1.0f, 1.0f, thePlayer);

	// Cache the vertex offsets to each object in the concatenated vertex buffer.
	mPlayerVertexOffset = 0;

	// Cache the index count of each object.
	mPlayerIndexCount = thePlayer.Indices.size();

	// Cache the starting index for each object in the concatenated index buffer.
	mPlayerIndexOffset = 0;

	UINT totalVertexCount = thePlayer.Vertices.size();

	UINT totalIndexCount = mPlayerIndexCount;

	//
	// Extract the vertex elements we are interested in and pack the
	// vertices of all the meshes into one vertex buffer.
	//

	std::vector<Vertex::Basic32> vertices(totalVertexCount);

	UINT k = 0;
	for (size_t i = 0; i < thePlayer.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = thePlayer.Vertices[i].Position;
		vertices[k].Normal = thePlayer.Vertices[i].Normal;
		vertices[k].Tex = thePlayer.Vertices[i].TexC;
	}

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex::Basic32) * totalVertexCount;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &vertices[0];
	HR(pdevice->CreateBuffer(&vbd, &vinitData, &mPlayerVB));

	//
	// Pack the indices of all the meshes into one index buffer.
	//

	std::vector<UINT> indices;
	indices.insert(indices.end(), thePlayer.Indices.begin(), thePlayer.Indices.end());

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * totalIndexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &indices[0];
	HR(pdevice->CreateBuffer(&ibd, &iinitData, &mPlayerIB));




}

/*
void Player::drawPlayer(ID3D11DeviceContext* dc, const Camera& camera)
{


	UINT stride = sizeof(Vertex::Basic32);
	UINT offset = 0;

	XMMATRIX view = XMLoadFloat4x4(&mView);
	XMMATRIX proj = XMLoadFloat4x4(&mProj);
	XMMATRIX viewProj = view*proj;

	
	
	

	ID3DX11EffectTechnique* activeTech = Effects::BasicFX->Light2TexTech;

	D3DX11_TECHNIQUE_DESC techDesc;
	activeTech->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		dc->IASetVertexBuffers(0, 1, &mPlayerVB, &stride, &offset);
		dc->IASetIndexBuffer(mPlayerIB, DXGI_FORMAT_R32_UINT, 0);

		// Draw the Player
		XMMATRIX world = XMLoadFloat4x4(&mPlayerWorld);
		XMMATRIX worldInvTranspose = MathHelper::InverseTranspose(world);
		XMMATRIX worldViewProj = world*view*proj;

		Effects::BasicFX->SetWorld(world);
		Effects::BasicFX->SetWorldInvTranspose(worldInvTranspose);
		Effects::BasicFX->SetWorldViewProj(worldViewProj);
		Effects::BasicFX->SetTexTransform(XMLoadFloat4x4(&mTexTransform));
		Effects::BasicFX->SetMaterial(mPlayerMat);
		Effects::BasicFX->SetDiffuseMap(mDiffuseMapSRV);

		activeTech->GetPassByIndex(p)->Apply(0, dc);
		dc->DrawIndexed(mPlayerIndexCount, mPlayerIndexOffset, mPlayerVertexOffset);
	}

	
	

}
*/



XMVECTOR Player::GetRightXM()const
{

	return XMLoadFloat3(&mRight);
}


XMFLOAT3 Player::GetRight()const
{

	return mRight;

}

XMVECTOR Player::GetUpXM()const
{
	return XMLoadFloat3(&mUp);

}
XMFLOAT3 Player::GetUp()const
{

	return mUp;

}

XMVECTOR Player::GetLookXM()const
{

	return XMLoadFloat3(&mLook);
}
XMFLOAT3 Player::GetLook()const
{

	return mLook;
}